#pragma once
#include "base_singleton.h"
#include "engine_define.h"
#include <vector>
#include "EnginHelper.h"

class ILogicCallback
{
public:
	virtual void OnLazyInitResponse(int nCode) = 0;
	virtual void OnLoginPinResponse(int nCode) = 0;
	virtual void OnPinImageArrive(int nCode) = 0;
	virtual void OnLoginKeyResponse(int nCode) = 0;
	virtual void OnKeyImageArrive(int nCode) = 0;
	virtual void OnGetGroupLineResponse(int nCode) = 0;
	virtual void OnDelayResponse(int nGameID, int nAreaID) = 0;
	virtual void OnGetAccInfoResponse(int nCode) = 0;
	virtual void OnEngineLog(int, string) = 0;
	virtual void OnEnginAccResult(int, string) = 0;
	virtual void OnEnginData(uint32_t nFlush, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate) = 0;
	virtual void OnExpireed(int nSrc) = 0;
};

class CAccLogicCenter : public virtual nbase::SupportWeakCallback
{
public:
	CAccLogicCenter();
	~CAccLogicCenter();
	void SetCallBack(ILogicCallback * pCallee) { m_pCallBack = pCallee; }

	void Init();				//初始化一些静态设置，配置文件缓存等。
	void LazyInit();			//获取一些协议数据，最好在工作线程中调用
	void UpdateToken();
	void StartLog();
	void EndLog();
	void NetWorkFix();			//网络修复
	void ClearBuffer();			//清理缓存
	void VirtualCardFix();		//修复虚拟网卡
	STGameListConfigration & GetCinfigIni() { return m_stGameListConfigration; } 
	STGameList & GetGameListChn() { return m_stGameListChn; }
	STGameList & GetGameListFor() { return m_stGameListFor; }
	bool FindGameOnID(int nGameID, STGame & stGame);
	STGeoIP & GetGeoIP() { return m_stGeoIP; };
	STParamLoginPinResponse& GetPinRes() { return m_stPinRes; }
	STLineGroupStrore & FindLineGroup(int nGameID, int nAreaID);
	STGroupItem * FindGroupItem(int nGameID, int nAreaID, wstring wsProv);
	STGamePlat & GetGamePlat() { return m_stGamePlat;	}
	STProvice & GetProvince() {return m_stProvince; } 
	STACCInfo & GetAccInfo() { return m_stAccInfo; } 
	STSvrSet & GetSvrSetInfo() { return m_stSvrSet; }
	STUpdateTokenResponse & GetUpdateToken() { return m_stUpdateTokenResponse;  } 
	bool GetLineData(int nSvrId, STLineItem & rfData);
	void FindGameOnKeyWord(vector<STGame> & vctGot, wstring wsKey);
	bool AutoLogin();           //根据本地PIN或者KEY自动登录
	bool GetLineGroup(int nGameID, int nAreaID, wstring wsProvName);		//获取游戏线路和分组
	bool StartAcc(int nGameID, int nSvrID);
	bool StopAcc();
	void OnEnginLog(int flag, const char* message);
	void SaveModeIni();				//保存配置
	bool IsLogined();				//是否登录成功
	bool IsProcessOnRun();
	static void InstallFont();
	void ThreadCallInit();
private:
	void UpdateWithCheck();		//根据本地ini版本号，比对是否需要升级替换。
	void LoadGameListIni();
	void PraseProvince();
	void CreateModeSS();
	void CreateEntry();
	void CreateEnteryOnProtocal(int nProto);
	void OnEnginTimer();
	void LoadGameListFast();

	STGameListConfigration m_stGameListConfigration;
	STGameList m_stGameListChn;
	STGameList m_stGameListFor;
	STGameList m_stGamePool;
	STGeoIP m_stGeoIP;
	STGamePlat m_stGamePlat;
	STProvice m_stProvince;
	STACCInfo m_stAccInfo;
	STSvrSet m_stSvrSet;
	STUpdateTokenResponse m_stUpdateTokenResponse;
	vector<STLineGroupStrore> m_vctLineGroups;
	STParamLoginPinResponse  m_stPinRes;
	ILogicCallback * m_pCallBack = NULL;
	IEnginInterface * m_pEnginModeSS = NULL;
	IEnginInterface * m_pEnginEntry = NULL;
	IEnginInterface * m_pCurrentEngin = NULL;
	int m_nGameID = 0;
	int m_nAreaID = 0;
	int m_nSvrID = 0;
	uint32_t m_uRunTimes = 0;
	bool m_bLogin = false;
	int m_nEnginFlag = 0;
	bool m_bTimeOnRun = false;
	void * m_pEnginHandle = NULL;
	vpn_info m_accData;
	int m_nSendBY = 0;
	int m_nRecvBY = 0;
	uint32_t m_nTotalSendBY = 0;
	uint32_t m_nTotalRecvBY = 0;
};



#define LOGIC_CENTER		CSingleton<CAccLogicCenter>::instance
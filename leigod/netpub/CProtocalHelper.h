#pragma once
#include <string>
#include "base_singleton.h"
#include "engine_define.h"
#include <stdint.h>

using namespace  std;
namespace leigod_netpub
{

class CProtocalHelper
{
public:
	CProtocalHelper();
	~CProtocalHelper();
	bool GetGeoIP();
	bool LoginPin(STParamLoginPin & params);
	bool LoginKey(STParamLoginKey & params);
	bool GetSvrSet(STParamGetSvrSetting & params);
	bool GetLineGroup(STParamLineGroup & params, STLineGroup & rfData);
	bool GetLineList(STParamLineList & params);
	bool GetStartAccInfo(STParamStartAccInfo & params);
	bool UpdateToken(STUpdateToken & params);
	bool StartAccLog(STParamLogStart & params);
	bool EndAccLog(STParamLogEnd & params);
	bool DownLoadFile(string url, wstring path); 
	bool GetGameListChn();
	bool GetGameList();
	bool GetPlatList();
	bool GetGamePlatList();
	static wstring ConvertTailPath(string ssUrl);
private:
	bool IsGamePicExist(string ssUrl, bool bChn);
	wstring ConvertGamePath(string ssUrl, bool bChn);

	string getHardHashCode();
};

/*打包参数*/
bool PraseBase(string &, STProtoBase &);
bool PraseBase(string &, STGeoIP &);


/*解包参数*/
bool UnPraseBase(string &, STParamLoginPinResponse&);
bool UnPraseBase(string &, STGameList &);
bool UnPraseBaseEx(string &, STGameList &, string ssLastEdit);
bool UnPraseBase(string &, STCountryList &);
bool UnPraseBase(string &, STLineGroup &, wstring & wsProv);
bool UnPraseBase(string &, STLineList &);
bool UnPraseBase(string &, STACCInfo &);
bool UnPraseBase(string &, STSvrSet &);
bool UnPraseBase(string &, STAreaList &);
bool UnPraseBase(string &, STGamePlat &);
bool UnPraseBase(string &, STUpdateTokenResponse &);

bool UnPraseUrl(wstring &, int & nSubID, int & nHour, int & nMinute, int & nTotalMins, int & nWXID);
bool UnPraseCDK(wstring &, wstring &);

string PacketParam(STParamLoginPin & params);
string PacketParam(STParamLoginKey & params);
string PacketParam(STParamGetSvrSetting & params);
string PacketParam(STParamLineGroup & params);
string PacketParam(STParamLineList & params);
string PacketParam(STParamStartAccInfo & params);
string PacketParam(STUpdateToken & params);
string PacketParam(STParamLogStart & params);
string PacketParam(STParamLogEnd & params);

void LOG_FILE(wstring & path, string & content);
bool LoadGameListLocal(wstring &, STGameList &);
bool LoadGameListLocalPool(wstring &, STGameList &, std::set<int> & setPool);
string READ_FILE(wstring  path);
void * READ_FILE(wstring  path, int & len);

int GET_SUBID(wstring & wsResponse, wstring * pErr = NULL);
void GET_H_M(wstring & wsContent, int & nHour, int & nMin, int & nTotalMins, wstring & nWXID);

void SAVE_INI(string folder, string key, string value);
void SAVE_INI(string folder, string key, int value);
uint32_t GET_AVG_DELAY(string url, int nTime = 1, int * nLoss = NULL);

bool ImageValid(wstring wsPath);
bool ImageValid(string ssPath);
}


#define PROTO_HELPER	CSingleton<leigod_netpub::CProtocalHelper>::instance

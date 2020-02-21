#include "stdafx.h"
#include "CAccLogicCenter.h"
#include "StringHelper.h"
#include "CProtocalHelper.h"
#include <Shlwapi.h >
#pragma comment(lib, "Shlwapi.lib")
#include <shellapi.h>
#include "CEnginModeSSHelper.h"
#include "CEnginModeEntryHelper.h"
#include "log/LogHelper.h"
#include "CIni.h"

void __stdcall funEnginCallBack(int flag, const char* message);
void ClearAccData(vpn_info & data);

CAccLogicCenter::CAccLogicCenter()
{
	//NetWorkFix();

}


CAccLogicCenter::~CAccLogicCenter()
{
	if (m_pEnginModeSS) delete m_pEnginModeSS;
	if (m_pEnginEntry) delete m_pEnginEntry;
}

void CAccLogicCenter::Init()
{
	m_accData = { 0 };
	LoadGameListIni();
	m_stGameListChn.nRegion = 1;

	if (GetCinfigIni().nTestData == 1)
	{
		m_stGamePlat.vctPlats = { {1, L"Stream"}, {2, L"Orign"}, {3, L"UPlay"}, {4, L"Epic"} };
	}

	LogHelper::SetLogSetting(true, false, ui::PathUtil::GetCurrentModuleDir().c_str(), 0, 1024 * 1024);
	//CreateModeSS();
	//CreateEntry();
}

unsigned __stdcall funThreadCall(LPVOID lpParam)
{
	CAccLogicCenter * pThis = (CAccLogicCenter*)lpParam;
	pThis->ThreadCallInit();
	_endthreadex(0);
	return 0;
}

void CAccLogicCenter::ThreadCallInit()
{
	bool bRet = false;
	{
	
		//bRet = PROTO_HELPER()->GetSvrSet(stParasSet);
		//bRet = PROTO_HELPER()->GetPlatList();    平台接口合并
		bRet = PROTO_HELPER()->GetGamePlatList();
		bRet = PROTO_HELPER()->GetGameList();
	}

	if (m_pCallBack)
	{
		m_pCallBack->OnLazyInitResponse(bRet ? ERRCODE_OK : ERRCODE_GETGLIST_ERR);
	}

}

void CAccLogicCenter::LazyInit()
{
	Init();	
	PROTO_HELPER()->GetGeoIP();
	PraseProvince();
	STParamGetSvrSetting stParasSet;
	PROTO_HELPER()->GetSvrSet(stParasSet);
	LoadGameListFast();
	AutoLogin();
	(HANDLE)_beginthreadex(NULL, 0, funThreadCall, (LPVOID)this, 0, 0);

//	AutoLogin();


}

void CAccLogicCenter::UpdateWithCheck()
{

}

void CAccLogicCenter::LoadGameListIni()
{
	wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
	wstring wsDir = wsExepath + L"gamelib\\gamelib.ini";

	char sGet[1024] = { 0 };
	string ssDir = W2AS(wsDir);
	GetPrivateProfileStringA(("lib"), ("version0"), "", sGet, _countof(sGet), ssDir.c_str());
	m_stGameListConfigration.ssVersion0 = sGet;
	GetPrivateProfileStringA(("lib"), ("version1"), "", sGet, _countof(sGet), ssDir.c_str());
	m_stGameListConfigration.ssVersion1 = sGet;
	GetPrivateProfileStringA(("lib"), ("layoutversion0"), "", sGet, _countof(sGet), ssDir.c_str());
	m_stGameListConfigration.ssLayoutVersion0 = sGet;
	GetPrivateProfileStringA(("lib"), ("layoutversion1"), "", sGet, _countof(sGet), ssDir.c_str());
	m_stGameListConfigration.ssLayoutVersion1 = sGet;
	GetPrivateProfileStringA(("game"), ("used"), "", sGet, _countof(sGet), ssDir.c_str());
	m_stGameListConfigration.ssUsed = sGet;
	GetPrivateProfileStringA(("game"), ("pool"), "", sGet, _countof(sGet), ssDir.c_str());
	vector<string> vctSplitss;
	stralgo::StringHelper::SplitString(sGet, ',', vctSplitss);
	for (string itr : vctSplitss)
	{
		m_stGameListConfigration.poolgame.insert(atoi(itr.c_str()));
	}
	 
	m_stGameListConfigration.nEnginDns = GetPrivateProfileIntA("engin", "dns", 0, ssDir.c_str());
	m_stGameListConfigration.nLogFile = GetPrivateProfileIntA("log", "logfile", 0, ssDir.c_str());
	m_stGameListConfigration.nLogTrace = GetPrivateProfileIntA("log", "logtrace", 0, ssDir.c_str());
	m_stGameListConfigration.nEnginDbg = GetPrivateProfileIntA("log", "engindbg", 0, ssDir.c_str());

	m_stGameListConfigration.nAccMode = GetPrivateProfileIntA("mode", "accmode", 0, ssDir.c_str());
	m_stGameListConfigration.nTransPortMode = GetPrivateProfileIntA("mode", "transportmode", 0, ssDir.c_str());
	m_stGameListConfigration.nExitMode = GetPrivateProfileIntA("mode", "exitmode", 0, ssDir.c_str());
	m_stGameListConfigration.nExitFix = GetPrivateProfileIntA("mode", "exitfix", 0, ssDir.c_str());
	//test
	m_stGameListConfigration.nTestData = GetPrivateProfileIntA("test", "data", 0, ssDir.c_str());
	m_stGameListConfigration.nTestUrl = GetPrivateProfileIntA("test", "url", 0, ssDir.c_str());
	m_stGameListConfigration.nTestDebug = GetPrivateProfileIntA("test", "debug", 0, ssDir.c_str());
}

void CAccLogicCenter::SaveModeIni()
{
	using namespace leigod_netpub;
	SAVE_INI("mode", "accmode", m_stGameListConfigration.nAccMode);
	SAVE_INI("mode", "transportmode", m_stGameListConfigration.nTransPortMode);
	SAVE_INI("mode", "exitmode", m_stGameListConfigration.nExitMode);
	SAVE_INI("mode", "exitfix", m_stGameListConfigration.nExitFix);
}

void CAccLogicCenter::FindGameOnKeyWord(vector<STGame> & vctGot, wstring wsKey)
{
	for (STGame & itr : m_stGameListChn.vctGames)
	{
		if (StrStrIW(itr.wsGameTitle.c_str(), wsKey.c_str()) || StrStrIW(itr.wsAlias.c_str(), wsKey.c_str()))
		{
			vctGot.push_back(itr);
		}

	}

	for (STGame &  itr : m_stGameListFor.vctGames)
	{
		if (StrStrIW(itr.wsGameTitle.c_str(), wsKey.c_str()) || StrStrIW(itr.wsAlias.c_str(), wsKey.c_str()))
		{
			vctGot.push_back(itr);
		}

	}
}

bool CAccLogicCenter::FindGameOnID(int nGameID, STGame & stGame)
{

	for (STGame & itr : m_stGamePool.vctGames)
	{
		if (itr.nGameID == nGameID)
		{
			stGame = itr;
			return true;
		}
	}
	for (STGame & itr : m_stGameListChn.vctGames)
	{
		if (itr.nGameID == nGameID)
		{
			stGame = itr;
			return true;
		}
	}

	for (STGame & itr : m_stGameListFor.vctGames)
	{
		if (itr.nGameID == nGameID)
		{
			stGame = itr;
			return true;
		}
	}

	return false;
}

bool CAccLogicCenter::AutoLogin()
{
	bool bRet(false);
	bool bNoPinOrKey(false);
	wstring wsPinPath = ui::PathUtil::GetCurrentModuleDir() + L"pin.dat";
	string ssPin = leigod_netpub::READ_FILE(wsPinPath);
#ifdef VERSION_SM
	//if (ssPin.size())
	{
		STParamLoginPin stPin;
		stPin.ssPin = ssPin.size() ? ssPin : "0";
		stPin.ssFileVer = FILE_VER;
		stPin.ssCoreVer = CORE_VER;

		m_bLogin = bRet = PROTO_HELPER()->LoginPin(stPin);
		if (m_pCallBack)
		{
			m_pCallBack->OnLoginPinResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
		}

		if (m_bLogin)
		{
			STSvrSet & rfSvrData = GetSvrSetInfo();
			for (int i = 0; i < 2; i++)
			{
				rfSvrData.wsPngPathSM[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlSM[i]);
				if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathSM[i]))
				{
					PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlSM[i], rfSvrData.wsPngPathSM[i]);
				}

			}
			if (m_pCallBack)
			{
				m_pCallBack->OnPinImageArrive(0);
			}
		}
}
#else
	if (ssPin.size())
	{
		STParamLoginPin stPin;
		stPin.ssPin = ssPin;
		stPin.ssFileVer = FILE_VER;
		stPin.ssCoreVer = CORE_VER;

		m_bLogin = bRet = PROTO_HELPER()->LoginPin(stPin);
		if (m_pCallBack)
		{
			m_pCallBack->OnLoginPinResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
		}

		if (m_bLogin)
		{
			STSvrSet & rfSvrData = GetSvrSetInfo();
			for (int i = 0; i < 2; i++)
			{
				rfSvrData.wsPngPathSM[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlSM[i]);
				if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathSM[i]))
				{
					PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlSM[i], rfSvrData.wsPngPathSM[i]);
				}

			}
			if (m_pCallBack)
			{
				m_pCallBack->OnPinImageArrive(0);
			}
		}
	}
	else
	{
		wstring wsKeyPath = ui::PathUtil::GetCurrentModuleDir() + L"key.dat";
		string ssKey = leigod_netpub::READ_FILE(wsKeyPath);
		//if (ssKey.size())
		{
			STParamLoginKey stKey;
			stKey.ssKey = ssKey;
			stKey.ssFileVer = FILE_VER;
			stKey.ssCoreVer = CORE_VER;
			m_bLogin = bRet = PROTO_HELPER()->LoginKey(stKey);
			if (m_pCallBack)
			{
				m_pCallBack->OnLoginKeyResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
			}
		}


		if (m_bLogin)
		{
			if (m_bLogin)
			{
				STSvrSet & rfSvrData = GetSvrSetInfo();
				for (int i = 0; i < 2; i++)
				{
					rfSvrData.wsPngPathBY[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlBY[i]);
					if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathBY[i]))
					{
						PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlBY[i], rfSvrData.wsPngPathBY[i]);
					}

				}
				if (m_pCallBack)
				{
					m_pCallBack->OnKeyImageArrive(0);
				}
			}
		}
	}

#endif

	return bRet;
/*
	if (ssPin.size())
	{
		STParamLoginPin stPin;
		stPin.ssPin = ssPin;
		stPin.ssFileVer = FILE_VER;
		stPin.ssCoreVer = CORE_VER;

		m_bLogin = bRet = PROTO_HELPER()->LoginPin(stPin);
		if (m_pCallBack)
		{
			m_pCallBack->OnLoginPinResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
		}

		if (m_bLogin)
		{
			STSvrSet & rfSvrData = GetSvrSetInfo();
			for (int i = 0; i < 2; i++)
			{
				rfSvrData.wsPngPathSM[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlSM[i]);
				if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathSM[i]))
				{
					PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlSM[i], rfSvrData.wsPngPathSM[i]);
				}
				
			}
			if (m_pCallBack)
			{
				m_pCallBack->OnPinImageArrive(0);
			}
		}
	}
	else
	{
		wstring wsKeyPath = ui::PathUtil::GetCurrentModuleDir() + L"key.dat";
		string ssKey = leigod_netpub::READ_FILE(wsKeyPath);
		if (ssKey.size())
		{
			STParamLoginKey stKey;
			stKey.ssKey = ssKey;
			stKey.ssFileVer = FILE_VER;
			stKey.ssCoreVer = CORE_VER;
			m_bLogin = bRet = PROTO_HELPER()->LoginKey(stKey);
			if (m_pCallBack)
			{
				m_pCallBack->OnLoginKeyResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
			}
		}


		if (m_bLogin)
		{
			if (m_bLogin)
			{
				STSvrSet & rfSvrData = GetSvrSetInfo();
				for (int i = 0; i < 2; i++)
				{
					rfSvrData.wsPngPathBY[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlBY[i]);
					if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathBY[i]))
					{
						PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlBY[i], rfSvrData.wsPngPathBY[i]);
					}
					
				}
				if (m_pCallBack)
				{
					m_pCallBack->OnKeyImageArrive(0);
				}
			}
		}
	}


	if (bNoPinOrKey)
	{
		STParamLoginPin stPin;
		stPin.ssPin = "0";
		stPin.ssFileVer = FILE_VER;
		stPin.ssCoreVer = CORE_VER;

		m_bLogin = bRet = PROTO_HELPER()->LoginPin(stPin);
		if (m_pCallBack)
		{
			m_pCallBack->OnLoginPinResponse(m_bLogin ? ERRCODE_OK : ERRCODE_LOGIN_ERR);
		}

		if (m_bLogin)
		{
			STSvrSet & rfSvrData = GetSvrSetInfo();
			for (int i = 0; i < 2; i++)
			{
				rfSvrData.wsPngPathSM[i] = leigod_netpub::CProtocalHelper::ConvertTailPath(rfSvrData.ssPngUrlSM[i]);
				if (!leigod_netpub::ImageValid(rfSvrData.wsPngPathSM[i]))
				{
					PROTO_HELPER()->DownLoadFile(rfSvrData.ssPngUrlSM[i], rfSvrData.wsPngPathSM[i]);
				}

			}
			if (m_pCallBack)
			{
				m_pCallBack->OnPinImageArrive(0);
			}
		}
	}


	return false;*/
}

bool CAccLogicCenter::GetLineGroup(int nGameID, int nAreaID, wstring wsProvName)
{
	bool bRet(false);
	STLineGroupStrore stStore;
	m_nGameID = stStore.nGameID = nGameID;
	m_nAreaID = stStore.nAreaID = nAreaID;
	int nGot = -1;
	for (int i = 0; i < m_vctLineGroups.size(); i++)
	{
		if (m_vctLineGroups[i].nGameID == nGameID && m_vctLineGroups[i].nAreaID == nAreaID)
		{
			nGot = i;
			break;
		}
	}

	if (nGot == -1)
	{
		STParamLineGroup params;
		params.nUserID = m_stPinRes.nUserID;
		params.nGroupID = m_stPinRes.nGroupID;
		params.nGameID = nGameID;
		params.nAreaID = nAreaID;	
		params.wsProvName = wsProvName;
		bRet = PROTO_HELPER()->GetLineGroup(params, stStore.stData);
		m_vctLineGroups.push_back(stStore);
	}
	else
	{
		STParamLineGroup params;
		params.nUserID = m_stPinRes.nUserID;
		params.nGroupID = m_stPinRes.nGroupID;
		params.nGameID = nGameID;
		params.nAreaID = nAreaID;
		params.wsProvName = wsProvName;
		bRet = PROTO_HELPER()->GetLineGroup(params, m_vctLineGroups[nGot].stData);
	}

	if (m_pCallBack)
	{
		m_pCallBack->OnGetGroupLineResponse(bRet ? ERRCODE_OK : ERRCODE_GETGLINE_ERR);
	}

	if (0&&bRet)
	{
		uint32_t tm = GetTickCount();
		STLineGroupStrore & rfdata = FindLineGroup(nGameID, nAreaID);
		for (STGroupItem & itr : rfdata.stData.vctGroups)
		{
			for (STLineItem & itrl : itr.vctGroupLines)
			{
				itrl.nDelay = leigod_netpub::GET_AVG_DELAY(itrl.ssSvr);
			}
			
		}
		LOG_INFO(MAINLOG_DEF, L"ping time" << GetTickCount() - tm);
		if (m_pCallBack)
		{
			m_pCallBack->OnDelayResponse(nGameID, nAreaID);
		}
	}


	

	return bRet;
}

STLineGroupStrore & CAccLogicCenter::FindLineGroup(int nGameID, int nAreaID)
{
	static STLineGroupStrore gRet;

	for (int i = 0; i < m_vctLineGroups.size(); i++)
	{
		if (m_vctLineGroups[i].nGameID == nGameID && m_vctLineGroups[i].nAreaID == nAreaID)
		{
			return m_vctLineGroups[i];
		}
	}

	return gRet;
}

STGroupItem * CAccLogicCenter::FindGroupItem(int nGameID, int nAreaID, wstring wsProv)
{
	for (int i = 0; i < m_vctLineGroups.size(); i++)
	{
		if (m_vctLineGroups[i].nGameID == nGameID && m_vctLineGroups[i].nAreaID == nAreaID)
		{
			STLineGroupStrore & rfGroupStore = m_vctLineGroups[i];
			for (STGroupItem & itr : rfGroupStore.stData.vctGroups)
			{
				if (itr.wsProvName == wsProv)
				{
					return &itr;
				}
			}
		}
	}
	return NULL;
}

bool CAccLogicCenter::StartAcc(int nGameID, int nSvrID)
{
	//NetWorkFix();
	bool bRet(false);
	STParamStartAccInfo param;
	param.ssUser = m_stPinRes.ssUserName;
	param.ssPassword = m_stPinRes.ssAuthKey;
	m_nSvrID = param.nSvrID = nSvrID;
	param.ssCoreVersion = CORE_VER;
	param.ssFileVersion = FILE_VER;
	param.nSubUserID = GetPinRes().nSubUserID;
	m_nGameID = param.nGameID = nGameID;
	m_nSendBY = 0;
	m_nRecvBY = 0;
	m_nTotalSendBY = 0;
	m_nTotalRecvBY = 0;

	bRet = PROTO_HELPER()->GetStartAccInfo(param);
	if (bRet)
	{
		STLineItem stLineData(L"", L"");
		if (GetLineData(nSvrID, stLineData))
		{
			CreateEnteryOnProtocal(stLineData.nProtoType);
			Sleep(100);
			if (m_pCurrentEngin)
			{
				m_pCurrentEngin->InitLogic();
				nbase::ThreadManager::PostDelayedTask(kThreadDown, ([this, nSvrID]() {
					if (m_pCurrentEngin)
					{
						STACCInfo & rfData = LOGIC_CENTER()->GetAccInfo();
						if (ACC_USER_EXPIRED == rfData.nErrCode)
						{
							if (m_pCallBack)
							{
								m_pCallBack->OnExpireed(2);
							}
							return false;
						}
						STLineItem stLineDataCloster(L"", L"");
						if (GetLineData(nSvrID, stLineDataCloster))
						{
							m_pCurrentEngin->SetProceeRule(rfData.ssProcessRule);
							if (GetCinfigIni().nTestData == 1)
							{
								m_pEnginHandle = m_pCurrentEngin->Connect("47.102.193.208", 443, "USER528586", "PASS809091918885", "6wMAAAEAAAABAAAACgAAAE5ldyBSdWxlIDEXAAAATGVhZ3VlIG9mIExlZ2VuZHMuZXhlDQoAAAAAAAAAABkAAAAqLmFwY2Rucy5uZXQsKi5sb2wucXEuY29tAAAAAAEBAAAAAA==", \
									funEnginCallBack, "300k/300k", 1, 0, 1);
							}
							else
							{
								if (rfData.ssAddSvrAreaEx.size() && rfData.ssAddSvrPortEx.c_str() && rfData.ssAddSvrEx.size())
								{
									m_pEnginHandle = m_pCurrentEngin->ConnectEx2(rfData.ssSvr.c_str(), rfData.nPort, rfData.ssTokenUser.c_str(), rfData.ssTokenPwd.c_str(), rfData.ssRules.c_str(), \
										funEnginCallBack, rfData.ssBandWidth.c_str(), 1, 0, 1, rfData.ssAddSvrAreaEx.c_str(), rfData.ssAddSvrEx.c_str(), rfData.ssAddSvrPortEx.c_str());
								}
								else if (rfData.ssAddSvrArea.size() && rfData.nAddSvrPort && rfData.ssAddSvr.size())
								{
									m_pEnginHandle = m_pCurrentEngin->ConnectEx(rfData.ssSvr.c_str(), rfData.nPort, rfData.ssTokenUser.c_str(), rfData.ssTokenPwd.c_str(), rfData.ssRules.c_str(), \
										funEnginCallBack, rfData.ssBandWidth.c_str(), 1, 0, 1, rfData.ssAddSvrArea.c_str(), rfData.ssAddSvr.c_str(), rfData.nAddSvrPort);
								}
								else
								{
									m_pEnginHandle = m_pCurrentEngin->Connect(rfData.ssSvr.c_str(), rfData.nPort, rfData.ssTokenUser.c_str(), rfData.ssTokenPwd.c_str(), rfData.ssRules.c_str(), \
										funEnginCallBack, rfData.ssBandWidth.c_str(), 1, 0, 1);
								}

								LOG_INFO(MAINLOG_DEF, L"start acc; connect:" << U2W(rfData.ssSvr));
							}

						}
						
					}

					int i = 0;
				}), nbase::TimeDelta::FromMilliseconds(500));

			}

		}
		
	}
	else
	{
		if (m_pCallBack)
		{
			m_pCallBack->OnGetAccInfoResponse(1);
		}
		LOG_INFO(MAINLOG_DEF, L"StartAcc Fial AccInfo Err");
	}

	return bRet;
}

bool CAccLogicCenter::StopAcc()
{
	if (m_pCurrentEngin && m_pEnginHandle)
	{
		EndLog();
		ClearAccData(m_accData);
		m_pCurrentEngin->Close(m_pEnginHandle);
		m_pEnginHandle = NULL;
		m_nEnginFlag = 0;
		m_bTimeOnRun = false;
		return true;
	}

	return false;
}

void CAccLogicCenter::OnEnginLog(int flag, const char* message)
{
	if (m_pCallBack && message)
	{
		m_pCallBack->OnEngineLog(flag, message);
		if (0 < flag && flag < 4)
		{
			m_nEnginFlag = flag;
			string ssMsg(message);
			m_pCallBack->OnEnginAccResult(flag, ssMsg);
			if (2 == flag)
			{
				if (!m_bTimeOnRun)
				{
					m_bTimeOnRun = true;
					nbase::ThreadManager::PostDelayedTask(kThreadDown, nbase::Bind(&CAccLogicCenter::OnEnginTimer, this), nbase::TimeDelta::FromMilliseconds(EBGIN_TIMER_ELAPSE));
				}

			}
		}
	}
}

uint32_t clac_improve(uint32_t nDelay)
{
	uint32_t uRet = 20;
	if (nDelay <= 40)
	{
		uRet = 95;
	}
	else if (nDelay >= 140)
	{
		uRet = 20;
	}
	else
	{
		uRet = (95.0 - (nDelay*1.0 - 40.0)* 0.75);
	}

	return uRet;
}

void ClearAccData(vpn_info & data)
{
	memset(&data, 0, sizeof(vpn_info));
}

bool IsClearAccData(vpn_info & data)
{
	return data.dwConnectDuration == 0 && data.dwBytesRcved == 0 && data.dwBytesXmited == 0 && data.dwRecvX == 0 && data.dwSendX == 0;
}

void CAccLogicCenter::OnEnginTimer()
{
	static uint32_t uTest = (0);
	bool bIsVpn = 0;
	if (!m_pCurrentEngin)
	{
		m_bTimeOnRun = false;
		return;
	}
	
	bIsVpn = m_pCurrentEngin->IsVpnMode();
	if (m_nEnginFlag == 2)
	{
		m_uRunTimes++;
		
		if (m_pCurrentEngin)
		{
			m_accData.adapter_index = 99;
			bool bClear = IsClearAccData(m_accData);
			m_pCurrentEngin->GetStatus(&m_accData, m_pEnginHandle);
			if (bClear)
			{
				StartLog();
			}
		}
		uTest += 100;
		uint32_t nDelay(20);
		
		int nLoss(0);
		
		nDelay = leigod_netpub::GET_AVG_DELAY(m_accData.szServerIpAddress, 4, &nLoss);
		int nAccRate = clac_improve(nDelay);

		if (m_uRunTimes %6 == 0)
		{
			nDelay += (rand() % 2);
			nAccRate += (rand() % 2);
		}
		if (m_pCallBack)
		{
			uint32_t dwSend(0) ;
			if (bIsVpn)
			{
				if (m_accData.dwBytesXmited < m_nTotalSendBY)
				{
					m_nTotalSendBY += m_accData.dwBytesXmited;
				}
				else
				{
					m_nTotalSendBY = m_accData.dwBytesXmited;
				}
				if (m_accData.dwBytesRcved < m_nTotalRecvBY)
				{
					m_nTotalRecvBY += m_accData.dwBytesRcved;
				}
				else
				{
					m_nTotalRecvBY = m_accData.dwBytesRcved;
				}
				dwSend = (m_nTotalSendBY + m_nTotalRecvBY) / 1024;
			}
			else
			{
				dwSend = m_accData.dwBytesXmited + m_accData.dwBytesRcved;
			}
			
			m_pCallBack->OnEnginData(dwSend * 1024, nDelay, nAccRate, nLoss *25);
		}
		UpdateToken();
		nbase::ThreadManager::PostDelayedTask(kThreadDown, nbase::Bind(&CAccLogicCenter::OnEnginTimer, this), nbase::TimeDelta::FromMilliseconds(EBGIN_TIMER_ELAPSE));
	}
}


void __stdcall funEnginCallBack(int flag, const char* message)
{
	LOGIC_CENTER()->OnEnginLog(flag, message);
	LOG_INFO(LOG_ENGIN, L"" << flag << L"-" << AS2W(message));

}

void CAccLogicCenter::CreateModeSS()
{
	if (!m_pEnginModeSS)
	{
		m_pEnginModeSS = new CEnginModeSSHelper();
		m_pEnginModeSS->Init(funEnginCallBack, ENDBG_ON);
	}

}

void CAccLogicCenter::CreateEntry()
{
	if (!m_pEnginEntry)
	{
		m_pEnginEntry = new CEnginModeEntryHelper();
		m_pEnginEntry->Init(funEnginCallBack, ENDBG_ON);
	}
}

void CAccLogicCenter::CreateEnteryOnProtocal(int nProto)
{
	static int arr_entery[] = { 0,1,2,6,7 };
	int arr_mode[] = { 9 };

	if (GetCinfigIni().nTestData == 1)
	{
		CreateModeSS();
		m_pCurrentEngin = m_pEnginModeSS;
		return;
	}

	for (int i = 0; i < _countof(arr_entery); i++)
	{
		if (arr_entery[i] == nProto)
		{
			CreateEntry();
			m_pCurrentEngin = m_pEnginEntry;
			return;
		}
	}

	for (int i = 0; i < _countof(arr_mode); i++)
	{
		if (arr_mode[i] == nProto)
		{
			CreateModeSS();
			m_pCurrentEngin = m_pEnginModeSS;
			return;
		}
	}

	m_pCurrentEngin = NULL;
}

void CAccLogicCenter::UpdateToken()
{
	static uint32_t tm = GetTickCount();
	if (GetTickCount() - tm > 2*60*1000)
	{

		tm = GetTickCount();
		STUpdateToken param;
		param.nUserID = m_stPinRes.nUserID;
		param.nSvrID = m_nSvrID;
		param.nConnType = 0;
		param.nGameID = m_nGameID;
		param.nAreaID = m_nAreaID;
		param.nSendBY = (m_accData.dwBytesXmited - m_nSendBY) / 1024;
		param.nRecvBY = (m_accData.dwBytesRcved - m_nRecvBY) / 1024;
		m_nSendBY = m_accData.dwBytesXmited;
		m_nRecvBY = m_accData.dwBytesRcved;
		param.ssLocalIP = m_accData.szServerIpAddress;
		param.ssSvrName = m_accData.szServerIpAddress;
		param.nSubUserID = m_stPinRes.nSubUserID;

		if (PROTO_HELPER()->UpdateToken(param))
		{
			if (m_stUpdateTokenResponse.nExpireed)
			{
				if (m_pCallBack)
				{
					m_pCallBack->OnExpireed(3);
					if (m_pCurrentEngin)
					{
						m_pCurrentEngin->Close(m_pEnginHandle);
						m_pEnginHandle = NULL;
					}
				}
				LOG_INFO(MAINLOG_DEF, L"UpdateToken expired" << U2W(m_stUpdateTokenResponse.ssTokenUser));
			}
			else
			{
				if (m_pCurrentEngin)
				{
					m_pCurrentEngin->UpdateToken(m_pEnginHandle, m_stUpdateTokenResponse.ssTokenUser.c_str(), m_stUpdateTokenResponse.ssTokenPass.c_str());
				}
			}

		}
		LOG_INFO(MAINLOG_DEF, L"UpdateToken" << U2W(m_stUpdateTokenResponse.ssTokenUser));
	}


}

void CAccLogicCenter::StartLog()
{
	STParamLogStart param;
	param.nUserID = m_stPinRes.nUserID;
	param.nSvrID = m_nSvrID;
	param.nConnType = 0;
	param.nGameID = m_nGameID;
	param.nAreaID = m_nAreaID;
	param.ssLocalIP = m_accData.szServerIpAddress;
	param.ssSvrName = m_accData.szServerIpAddress;
	param.nSubUserID = m_stPinRes.nSubUserID;

	PROTO_HELPER()->StartAccLog(param);
}

void CAccLogicCenter::EndLog()
{
	STParamLogEnd param;
	param.nUserID = m_stPinRes.nUserID;
	param.nSvrID = m_nSvrID;
	param.nConnType = 0;
	param.nSessionID = 0;
	param.ssLocalIP = m_accData.szServerIpAddress;
	param.ssNodeIP = m_accData.szServerIpAddress;
	param.nSubUserID = m_stPinRes.nSubUserID;

	PROTO_HELPER()->EndAccLog(param);
}

void CAccLogicCenter::NetWorkFix()
{
	HINSTANCE retVal = ::ShellExecuteA(NULL, "open", "cmd", "/c netsh winsock reset catalog > CUninstall.log", NULL, SW_HIDE);
}

void CAccLogicCenter::ClearBuffer()
{


}

void CAccLogicCenter::VirtualCardFix()
{
	if (m_pCurrentEngin)
	{
		m_pCurrentEngin->ModeResetTap();
	}
}

void CAccLogicCenter::PraseProvince()
{
	CIni iniReader;
	wstring wsKeyPath = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\layout1.xml";
	if (S_OK == iniReader.Load(wsKeyPath.c_str()))
	{
		const CIni::SectionTable & rfSecTbl = iniReader.GetSectionTable();
		for (auto itr : rfSecTbl)
		{
			if (itr.sectionName == L"lang_cn")
			{
				for (auto itrMap : itr.vctSave)
				{
					STProvinceItem item = { (itrMap.first), itrMap.second };
					if (m_stGeoIP.wsProvince.empty() && 
						stralgo::StringHelper::MakeLower(m_stGeoIP.ssProviceCode) == stralgo::StringHelper::MakeLower(W2U(itrMap.first)))
					{
						m_stGeoIP.wsProvince = itrMap.second;
						m_stProvince.vctProvince.insert(m_stProvince.vctProvince.begin(), item);
					}
					else
					{
						m_stProvince.vctProvince.push_back(item);
					}

					
				}
			}
		}

	}

	if (m_stProvince.vctProvince.size() >3)
	{
		{
			m_stProvince.vctProvince.erase(m_stProvince.vctProvince.begin()+1, m_stProvince.vctProvince.begin()+3);
		}
	}
}

bool CAccLogicCenter::IsLogined()
{
	return m_bLogin;
}

bool CAccLogicCenter::GetLineData(int nSvrId, STLineItem & rfData)
{
	for (STLineGroupStrore & itrS : m_vctLineGroups)
	{
		for (STGroupItem & itrG : itrS.stData.vctGroups)
		{
			for (STLineItem & itrI : itrG.vctGroupLines)
			{
				if (itrI.nLineID == nSvrId)
				{
					rfData = itrI;
					return true;
				}
			}
		}
	}

	return false;
}

wstring GetFontDir()
{
	TCHAR sWinDir[MAX_PATH];
	GetWindowsDirectory(sWinDir, MAX_PATH);
	wstring sFontDir(sWinDir);
	sFontDir += _T("\\Fonts\\");

	return sFontDir;
}

void CAccLogicCenter::InstallFont()
{
	wstring wsSysFontDir = GetFontDir();
	wstring wsExeDir = ui::PathUtil::GetCurrentModuleDir();

	wstring wsSysPath = wsSysFontDir + L"BERNIER-Regular.otf";
	wstring wsExePath = wsExeDir + L"BERNIER-Regular.otf";

	CopyFile(wsExePath.c_str(), wsSysPath.c_str(), FALSE);
	//int nFontNum = AddFontResource(wsSysPath.c_str());
	int nFontNum = AddFontResourceEx(wsExePath.c_str(), FR_PRIVATE, 0);
	if (nFontNum <= 0)
	{
		return;
	}

	return;
	//永久字体注册
	HKEY phkResult;
	DWORD RegResult;
	wchar_t wsKey[10] = L"REG_SZ";
	LONG Rlt = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
		0, wsKey, 0, KEY_WRITE, 0, &phkResult, &RegResult);
	if (Rlt != ERROR_SUCCESS)
	{
		return;
	}

	int nLength = wsSysPath.size();
	wstring sFileName = wsSysPath;
	BYTE* m_byte = (BYTE*)sFileName.c_str();
	Rlt = RegSetValueEx(phkResult, L"BERNIER-Regular.otf (TrueType)", 0, REG_SZ, m_byte, nLength * 2);
	if (Rlt != ERROR_SUCCESS)
	{
		return;
	}

	Rlt = RegCloseKey(phkResult);
	if (Rlt != ERROR_SUCCESS)
	{
		return;
	}
}

bool CAccLogicCenter::IsProcessOnRun()
{
	HWND hPriWnd = ::FindWindowW(L"MainFrame", L"雷神加速器（网吧版）");
	return (::IsWindow(hPriWnd) && ::IsWindowVisible(hPriWnd)) ;
}

void CAccLogicCenter::LoadGameListFast()
{
	wstring wsExeDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\gamepool.xml";
	leigod_netpub::LoadGameListLocalPool(wsExeDir, m_stGamePool, m_stGameListConfigration.poolgame);
}

int is_log_file()
{
	return LOGIC_CENTER()->GetCinfigIni().nLogFile;
}

int is_dbg_engin()
{
	return LOGIC_CENTER()->GetCinfigIni().nTestDebug;
}

int is_log_trace()
{
	return LOGIC_CENTER()->GetCinfigIni().nLogTrace;
}

wstring get_isp_name(int isp)
{
	if (ISP_TELCOM == isp)
	{
		return L"电信";
	}
	else if (ISP_MOBILE == isp)
	{
		return L"移动";
	}
	else if (ISP_UNICOM== isp)
	{
		return L"联通";
	}
	return L"全服";
}
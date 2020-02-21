#include "stdafx.h"
#include "CEnginModeSSHelper.h"
#include "engine/modessheader.h"
#include "CAccLogicCenter.h"
#include "StringHelper.h"

CEnginModeSSHelper::CEnginModeSSHelper()
{
}


CEnginModeSSHelper::~CEnginModeSSHelper()
{
}


int CEnginModeSSHelper::Init(pPluginsEventMessage logfunc, int isDebug)
{
	if (!m_hDllHandle)
	{
		wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"mode_ss.dll";
		m_hDllHandle = LoadLibraryW(wsDir.c_str());
	}

	if (m_hDllHandle)
	{
		pInit pfun_init = (pInit)GetProcAddress(m_hDllHandle, "ModeInit");
		if (pfun_init)
		{
			return pfun_init(logfunc, isDebug);
		}
	}

	return 0;
}

int CEnginModeSSHelper::InitEx(pPluginsEventMessage logfunc, int isDebug, const char* coredll)
{
	return 0;
}

int CEnginModeSSHelper::UnInit()
{
	if (m_hDllHandle)
	{
		pUnInit pfun_uninit = (pUnInit)GetProcAddress(m_hDllHandle, "ModeUnInit");
		if (pfun_uninit)
		{
			return pfun_uninit();
		}
	}
	return 0;
}

int CEnginModeSSHelper::ModePrefix()
{
	if (m_hDllHandle)
	{
		pModePrefix pfun_perfix = (pModePrefix)GetProcAddress(m_hDllHandle, "ModePrefix");
		if (pfun_perfix)
		{
			return pfun_perfix();
		}
	}
	return 0;
}

int CEnginModeSSHelper::Config(void* c, const char* configname, const char* configvalue)
{
	LOG_INFO(ENGIN_PARAM, L"modess Config: " <<  U2W(configname) << L" -- " << U2W(configvalue));
	if (m_hDllHandle)
	{
		pConfig pfun_config = (pConfig)GetProcAddress(m_hDllHandle, "ModeConfig");
		if (pfun_config)
		{
			return pfun_config(c, configname, configvalue);
		}
	}

	return 0;
}

void * CEnginModeSSHelper::Connect(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns)
{
	LOG_INFO(ENGIN_PARAM, L"modess Connect: " << U2W(serveraddr) << L" -- " << port << L" -- " << U2W(username) << L" -- " << U2W(password) << L" -- " << U2W(rules) << L" -- " << U2W(bandwidth) << L" -- " << islogconnect << L" -- " << issendappstatus << L" -- " << dns);
	if (m_hDllHandle)
	{
		pConnect pfun_conn = (pConnect)GetProcAddress(m_hDllHandle, "ModeConnect");
		if (pfun_conn)
		{
			return pfun_conn(serveraddr, port, username, password, rules, logfunc, bandwidth, islogconnect, issendappstatus, dns);
		}
	}
	return 0;
}

void* CEnginModeSSHelper::ConnectEx(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns, \
	const char* add_server_area, const char* add_server, int add_server_port)
{
	LOG_INFO(ENGIN_PARAM, L"modess ConnectEx: " << U2W(serveraddr) << L" -- " << port << L" -- " << U2W(username) << L" -- " << U2W(password) << L" -- " << U2W(rules) << L" -- " << U2W(bandwidth) << L" -- " << islogconnect << L" -- " << issendappstatus << L" -- " << dns \
		<< L" -- " << U2W(add_server_area) << L" -- " << U2W(add_server) << L" -- " << add_server_port);

	if (m_hDllHandle)
	{
		pConnectEx pfun_connex = (pConnectEx)GetProcAddress(m_hDllHandle, "ModeConnectEx");
		if (pfun_connex)
		{
			return pfun_connex(serveraddr, port, username, password, rules, logfunc, bandwidth, islogconnect, issendappstatus, dns, add_server_area, add_server, add_server_port);
		}
	}

	return 0;
}

void* CEnginModeSSHelper::ConnectEx2(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns, \
	const char* add_server_areaex, const char* add_serverex, const char* add_server_portex)
{
	LOG_INFO(ENGIN_PARAM, L"modess ConnectEx2: " << U2W(serveraddr) << L" -- " << port << L" -- " << U2W(username) << L" -- " << U2W(password) << L" -- " << U2W(rules) << L" -- " << U2W(bandwidth) << L" -- " << islogconnect << L" -- " << issendappstatus << L" -- " << dns \
		<< L" -- " << U2W(add_server_areaex) << L" -- " << U2W(add_serverex) << L" -- " << U2W(add_server_portex));
	if (m_hDllHandle)
	{
		pConnectEx2 pfun_connex = (pConnectEx2)GetProcAddress(m_hDllHandle, "ModeConnectEx2");
		if (pfun_connex)
		{
			return pfun_connex(serveraddr, port, username, password, rules, logfunc, bandwidth, islogconnect, issendappstatus, dns, add_server_areaex, add_serverex, add_server_portex);
		}
	}

	return 0;
}

int CEnginModeSSHelper::GetStatus(vpn_info* info, void* c)
{
	if (m_hDllHandle)
	{
		pGetStatus pfun_status = (pGetStatus)GetProcAddress(m_hDllHandle, "ModeGetStatus");
		if (pfun_status)
		{
			return pfun_status(info, c);
		}
	}
	return 0;
}

int CEnginModeSSHelper::Close(void* c)
{
	if (m_hDllHandle)
	{
		pClose pfun_close = (pClose)GetProcAddress(m_hDllHandle, "ModeClose");
		if (pfun_close)
		{
			return pfun_close(c);
		}
	}
	return 0;
}

int CEnginModeSSHelper::UpdateToken(void* c, const char* tokenuser, const char* tokenpass)
{
	if (m_hDllHandle)
	{
		pUpdateToken pfun_token = (pUpdateToken)GetProcAddress(m_hDllHandle, "ModeUpdateToken");
		if (pfun_token)
		{
			return pfun_token(c, tokenuser, tokenpass);
		}
	}

	return 0;
}

int CEnginModeSSHelper::ModeResetTap()
{
	return 0;
}

int CEnginModeSSHelper::ModeResetAll()
{
	return 0;
}

int CEnginModeSSHelper::ModeSetupTcpCN2Svr(vpn_info* info, const char * svr, int port)
{
	if (m_hDllHandle)
	{
		pModeSetupTcpCN2Svr pfun_config = (pModeSetupTcpCN2Svr)GetProcAddress(m_hDllHandle, "ModeSetupTcpCN2Server");
		if (pfun_config)
		{
			return pfun_config(info, svr, port);
		}
	}

	return 0;
}

int CEnginModeSSHelper::ModeSetupDownloadSvr(vpn_info* info, const char * svr, int port, const char * bandwidth)
{
	if (m_hDllHandle)
	{
		pModeSetupDownloadSvr pfun_config = (pModeSetupDownloadSvr)GetProcAddress(m_hDllHandle, "ModeSetupDownloadServer");
		if (pfun_config)
		{
			return pfun_config(info, svr, port, bandwidth);
		}
	}

	return 0;
}

int CEnginModeSSHelper::InitLogic()
{
	
	Config(0, "debug", "0");
	Config(0, "udpovertcp", "0");
	Config(0, "socksmode", std::to_string(LOGIC_CENTER()->GetCinfigIni().nAccMode).c_str());

	return 0;
}
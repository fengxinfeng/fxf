#pragma once
#include "EnginHelper.h"

class CEnginModeEntryHelper : public IEnginInterface
{
public:
	CEnginModeEntryHelper();
	~CEnginModeEntryHelper();
public:
	virtual int Init(pPluginsEventMessage logfunc, int isDebug);
	virtual int InitEx(pPluginsEventMessage logfunc, int isDebug, const char* coredll);
	virtual int UnInit();
	virtual int ModePrefix();
	virtual int Config(void* c, const char* configname, const char* configvalue);
	virtual void* Connect(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns);
	virtual void* ConnectEx(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns, \
		const char* add_server_area, const char* add_server, int add_server_port);
	virtual void* ConnectEx2(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns, \
		const char* add_server_areaex, const char* add_serverex, const char* add_server_portex);
	virtual int GetStatus(vpn_info* info, void* c);
	virtual int Close(void* c);
	virtual int UpdateToken(void* c, const char* tokenuser, const char* tokenpass);
	virtual int ModeResetTap();
	virtual int ModeResetAll();
	virtual int ModeSetupTcpCN2Svr(vpn_info* info, const char * svr, int port);
	virtual int ModeSetupDownloadSvr(vpn_info* info, const char * svr, int port, const char * bandwidth);
	virtual int InitLogic();
	virtual void SetProceeRule(string ssProcessRule) { m_ssProceeRule = ssProcessRule; }
	virtual bool IsVpnMode() { return true; }
private:
	HINSTANCE m_hDllHandle = NULL;
	string m_ssProceeRule;
};


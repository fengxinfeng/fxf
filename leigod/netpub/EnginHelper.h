#pragma once
#include "engine/modessheader.h"

class IEnginInterface
{
public:
	virtual int Init(pPluginsEventMessage logfunc, int isDebug) = 0;
	virtual int InitEx(pPluginsEventMessage logfunc, int isDebug, const char* coredll) = 0;
	virtual int UnInit() = 0;
	virtual int ModePrefix() = 0;
	virtual int Config(void* c, const char* configname, const char* configvalue) = 0;
	virtual void* Connect(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns) = 0;
	virtual void* ConnectEx(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns,\
		const char* add_server_area, const char* add_server, int add_server_port) = 0;
	virtual void* ConnectEx2(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns,\
		const char* add_server_areaex, const char* add_serverex, const char* add_server_portex) = 0;
	virtual int GetStatus(vpn_info* info, void* c) = 0;
	virtual int Close(void* c) = 0;
	virtual int UpdateToken(void* c, const char* tokenuser, const char* tokenpass) = 0;
	virtual int ModeResetTap() = 0;
	virtual int ModeResetAll() = 0;
	virtual int ModeSetupTcpCN2Svr(vpn_info* info, const char * svr, int port) = 0;
	virtual int ModeSetupDownloadSvr(vpn_info* info, const char * svr, int port, const char * bandwidth) = 0;
	virtual int InitLogic() = 0;
	virtual void SetProceeRule(string ssProcessRule) = 0;
	virtual bool IsVpnMode() = 0;
};

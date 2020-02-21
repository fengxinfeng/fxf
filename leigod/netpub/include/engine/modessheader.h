#pragma once

#ifndef _MODE_SS_HEADER_
#define _MODE_SS_HEADER_
#include <stdint.h>

//pure c name bind
#ifdef __cplusplus
extern "C" {
#endif

	struct vpn_info
	{
		char szGateWay[128];			//路由的网关IP
		char szServerIpAddress[128];	//服务器IP
		char szIpAddress[128];			//客户端IP
		char szDeviceName[128];			//设备名称
		char szDeviceType[128];			//设备类型
		unsigned long dwConnectDuration;//已连接时间
		unsigned long dwBytesXmited;	//发送的字节数
		unsigned long dwBytesRcved;		//接收的字节数
		unsigned long dwSendX;			//发送速度 字节/秒
		unsigned long dwRecvX;			//接收速度 字节/秒
		unsigned long adapter_index;
	};
	struct hosted_info
	{
		unsigned long dwOpened;			//当前状态 0 未开启 1 已开启
		char szIpAddress[128];			//主机设置的IP
		char szMaskAddress[128];		//子网掩码
		char szGateWay[128];			//主机网关IP
		char szDNSAddress[128];			//推荐DNS
		char szDeviceName[128];			//当前网络连接名称
	};


	typedef int(__stdcall *pGetHostedInfo)(hosted_info* info, void* c);

	typedef void (__stdcall  *pPluginsEventMessage)(int flag, const char* message);
	typedef int (__stdcall	*pInit)(pPluginsEventMessage logfunc, int isDebug);
	typedef int (__stdcall	*pInitEx)(pPluginsEventMessage logfunc, int isDebug, const char* coredll);
	typedef	int (__stdcall *pUnInit)();
	typedef	int (__stdcall *pModePrefix)();
	typedef	int (__stdcall *pConfig)(void* c, const char* configname, const char* configvalue);
	typedef void* (__stdcall	*pConnect)(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns);
	typedef void* (__stdcall	*pConnectEx)(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns,
		const char* add_server_area, const char* add_server, int add_server_port);
	typedef void* (__stdcall	*pConnectEx2)(const char* serveraddr, int port, const char* username, const char* password, const char* rules, pPluginsEventMessage logfunc, const char* bandwidth, int islogconnect, int issendappstatus, int dns,
		const char* add_server_areaex, const char* add_serverex, const char* add_server_portex);
	typedef	int (__stdcall *pGetStatus)(vpn_info* info, void* c);
	typedef	int (__stdcall *pClose)(void* c);
	typedef	int (__stdcall *pUpdateToken)(void* c, const char* tokenuser, const char* tokenpass);
	typedef	int (__stdcall *pModeResetTap)();
	typedef	int (__stdcall *pModeResetAll)();
	typedef	int(__stdcall *pModeSetupTcpCN2Svr)(vpn_info* info, const char * svr, int port);
	typedef	int(__stdcall *pModeSetupDownloadSvr)(vpn_info* info, const char * svr, int port, const char * bandwidth);
	typedef int(__stdcall *pModeHaveVirtualAdapter)();

	const int plugins_status_ready = 0;
	const int plugins_status_closed = 1;

#ifdef __cplusplus
};
#endif // _cplusplus


#endif


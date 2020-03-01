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
		char szGateWay[128];			//·�ɵ�����IP
		char szServerIpAddress[128];	//������IP
		char szIpAddress[128];			//�ͻ���IP
		char szDeviceName[128];			//�豸����
		char szDeviceType[128];			//�豸����
		unsigned long dwConnectDuration;//������ʱ��
		unsigned long dwBytesXmited;	//���͵��ֽ���
		unsigned long dwBytesRcved;		//���յ��ֽ���
		unsigned long dwSendX;			//�����ٶ� �ֽ�/��
		unsigned long dwRecvX;			//�����ٶ� �ֽ�/��
		unsigned long adapter_index;
	};
	struct hosted_info
	{
		unsigned long dwOpened;			//��ǰ״̬ 0 δ���� 1 �ѿ���
		char szIpAddress[128];			//�������õ�IP
		char szMaskAddress[128];		//��������
		char szGateWay[128];			//��������IP
		char szDNSAddress[128];			//�Ƽ�DNS
		char szDeviceName[128];			//��ǰ������������
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


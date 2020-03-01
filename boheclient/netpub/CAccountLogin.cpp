#include "stdafx.h"
#include "CAccountLogin.h"
#include "CProtocalHelper.h"

CAccountLogin::CAccountLogin()
{
	
}

CAccountLogin::~CAccountLogin()
{

}

unsigned __stdcall funGetGameThreadCall(LPVOID lpParam)
{
	CAccountLogin * pThis = (CAccountLogin*)lpParam;
	pThis->ThreadCallInit();
	_endthreadex(0);
	return 0;
}
bool CAccountLogin::initLogin()
{
	PROTO_HELPER()->GetNationCodeList();
	(HANDLE)_beginthreadex(NULL, 0, funGetGameThreadCall, (LPVOID)this, 0, 0);
	return true;
}

void CAccountLogin::ThreadCallInit()
{
	bool bRet = false;
	{
		//bRet = PROTO_HELPER()->GetSvrSet(stParasSet);
		//bRet = PROTO_HELPER()->GetPlatList();    平台接口合并
		bRet = PROTO_HELPER()->GetGamePlatList();
		bRet = PROTO_HELPER()->GetGameList();
	}

}
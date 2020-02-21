// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

// C runtime header
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// base header
#include "base/base.h"

// duilib
#include "duilib/UIlib.h"
// ui components
#include "ui_components/ui_components.h"
#include "ui_components/ui_cef_control.h"
#include <string>
#include "log/LogHelper.h"
using namespace std;

enum ThreadId
{
	kThreadBegin = -1,//线程ID定义开始
	kThreadUI,			//UI线程（主线程）
	kThreadDatabase,	//写DB线程
	kThreadGlobalMisc,	//全局Misc线程（比如：打开url）
	kThreadUpLoad,		//上传线程
	kThreadLiveStreaming,//直播线程
	kThreadDown,		//下载
	kThreadProtocal,	//用户自定义应用线程
	kInnerWork,			//杂项线程
	kThreadEnd,//线程ID定义结束
};


struct GFGameLIST
{
	GFGameLIST()
	{
		nPlate=-1;
		nGameId = -1;
	}

	std::wstring GameName;
	int nPlate;
	int nGameId;   //1:chn, 2:forign
	ui::ListContainerElement* Element;
};

typedef std::pair<std::wstring, ui::VBox*> Insert_Pair;
typedef std::pair<std::wstring, ui::ListContainerElement*> Insert_Pair_server;
typedef std::pair<int, ui::ListContainerElement*> Insert_Pair_Game_Box;

#define WUM_TARYICON_MSG (WM_USER + 10)

extern HINSTANCE gInstance;

void SetLableText(ui::WindowImplBase * pWnd, wstring wsCtrlID, wstring wsText);
void ShowCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID);
void HideCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID);

void ShowCtrl(ui::WindowImplBase * pWnd, ui::Control * pParent, wstring wsCtrlID);
void HideCtrl(ui::WindowImplBase * pWnd, ui::Control * pParent, wstring wsCtrlID);

void EnableCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID, BOOL bEnable);
void ClearButImage(ui::WindowImplBase * pWnd, wstring wsCtrlID);

template<typename TCtrl>
void SetCtrlText(ui::WindowImplBase * pWnd, wstring wsCtrlID, wstring wsText)
{
	TCtrl * pLbCtrl = dynamic_cast<TCtrl *>(pWnd->FindControl(wsCtrlID));
	if (pLbCtrl)
	{
		pLbCtrl->SetText(wsText);
	}
}

#define NO_SELECT L"node_noselect_bk_color"
#define SELECT    L"node_select_bk_color"

#define WEB_LOG		L"web.log"
#define ENGIN_PARAM			L""//		L"engin_param.log"
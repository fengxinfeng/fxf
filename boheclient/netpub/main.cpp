    // basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "layouts_form.h"
#include "CMainFrameUI.h"
#include "wMainFrame.h"

HINSTANCE gInstance = NULL;
CMainFrameUI* gMainWnd = NULL;

static bool IsProcessOnRun();
BOOL RaisePrivilege();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//gInstance = hInstance;
	//RaisePrivilege();
	//CAccLogicCenter::InstallFont();
	nim_comp::CefManager::GetInstance()->AddCefDllToPath();
	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	 //初始化 CEF
	CefSettings settings;
	if (!nim_comp::CefManager::GetInstance()->Initialize(nbase::win32::GetCurrentModuleDirectory() + L"cef_temp\\", settings, kEnableOffsetRender))
	{
		return 0;
	}

	//ui::PathUtil::GetCurrentModuleDir();
	//AddFontMemResourceEx();
	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);
	/*if (gMainWnd)
	{
		gMainWnd->ReleaseOnExit();
	}*/
	// 清理 CEF
	nim_comp::CefManager::GetInstance()->UnInitialize();
	::OleUninitialize();
	return 0;
}

void ShowMainFrame()
{

	//if (IsProcessOnRun())
	//{
	//	::MessageBoxExW(NULL, L"雷神加速器已经运行。", L"提示", MB_OK, 0);
	//	return PostQuitMessage(0);;
	//}
	//CMainFrameUI * pMainWnd = new CMainFrameUI();
	wMainFrame * pMainWnd = new wMainFrame();
	//gMainWnd = pMainWnd;
	pMainWnd->Create(NULL, CMainFrameUI::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	pMainWnd->ShowWindow();
	pMainWnd->CenterWindow();
}


void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);
	m_DownThread_.reset(new DownThread(kThreadDown, "Down Thread"));
	m_DownThread_->Start();
	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	
#ifdef _DEBUG
	// 	// Debug 模式下使用本地文件夹作为资源
	// 	// 默认皮肤使用 resources\\themes\\default
	// 	// 默认语言使用 resources\\lang\\zh_CN
	// 	// 如需修改请指定 Startup 最后两个参数
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
#else
	// 	// Release 模式下使用资源中的压缩包作为资源
	// 	// 资源被导入到资源列表分类为 THEME，资源名称为 IDR_THEME
	// 	// 如果资源使用的是本地的 zip 文件而非资源中的 zip 压缩包
	// 	// 可以使用 OpenResZip 另一个重载函数打开本地的资源压缩包
	ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_THEME), L"THEME", "");//ui::GlobalManager::OpenResZip(L"resources.zip", "");
	ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
#endif
	ShowMainFrame();
}

void MainThread::Cleanup()
{
	m_DownThread_->Stop();
	m_DownThread_.reset(nullptr);
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}


DownThread::DownThread(enum ThreadId thread_id, const char* name)
	: FrameworkThread(name)
	, thread_id_(thread_id)
{

}

DownThread::~DownThread(void)
{

}

void DownThread::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void DownThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}

bool IsProcessOnRun()
{

	HWND hPriWnd = FindWindowW(L"MainFrame", L"雷神加速器（网吧版）");
	if (hPriWnd && !IsWindowVisible(hPriWnd))
	{
		::ShowWindow(hPriWnd, SW_SHOW);
		Sleep(100);
	}
	
	return (::IsWindow(hPriWnd) && IsWindowVisible(hPriWnd));

#if 0	
		HANDLE  m_hProcessParam = ::CreateEvent(NULL, FALSE, FALSE, UINQUEPRCESSSTR);
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
		return false;

	#endif
}

BOOL RaisePrivilege()
{
	HANDLE              hToken;
	BOOL bRet = OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken);
	if (bRet != TRUE)
	{
		return FALSE;
	}

	//
	// Get the LUID for the shutdown privilege. 
	TOKEN_PRIVILEGES    tkp;
	LookupPrivilegeValue(NULL,
		SE_DEBUG_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;                    // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//
	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken,
		FALSE,
		&tkp,
		0,
		(PTOKEN_PRIVILEGES)NULL,
		0);

	if (GetLastError() != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}
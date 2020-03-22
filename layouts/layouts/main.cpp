// basic.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "layouts_form.h"

//#pragma comment(lib, "dbghelp.lib")
 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	OutputDebugString(L"maing------------------->");
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 
	nim_comp::CefManager::GetInstance()->AddCefDllToPath();
	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;
	OutputDebugString(L"maing-------------------0");
	// 初始化 CEF
	CefSettings settings;
	if (!nim_comp::CefManager::GetInstance()->Initialize(nbase::win32::GetCurrentModuleDirectory() + L"cef_temp\\", settings, kEnableOffsetRender))
	{
		return 0;
	}
	//AllocConsole(); 
	 
	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	 
	// 清理 CEF
	nim_comp::CefManager::GetInstance()->UnInitialize();

	::OleUninitialize();
	 
	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	// 一个仿微信的布局示例
	LayoutsForm::ShowCustomWindow(L"basic_layout", L"layouts", L"wechat.xml");

	// 一个仿登录窗口的布局示例
	// LayoutsForm::ShowCustomWindow(L"login", L"layouts", L"login.xml");
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}

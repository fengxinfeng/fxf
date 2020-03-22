#include "stdafx.h"
#include "CLoginVBOX.h"
#include "CLogin_Wnd.h"

const std::wstring CLoginWnd::kClassName = L"standalone login window";

CLoginWnd::CLoginWnd()
{
}

CLoginWnd::~CLoginWnd()
{
}

CLoginWnd* CLoginWnd::ShowLoginWindow(ui::UiRect pos, HWND parent, nui::CLoginVBOX *loginbox)
{
	CLoginWnd* window = new CLoginWnd( );
	window->Create(parent, CLoginWnd::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0, nim_comp::CefManager::GetInstance()->IsEnableOffsetRender());
	window->CenterWindow(); 
	window->ShowWindow();
	window->login_box_ = loginbox;
	return window;
}

std::wstring CLoginWnd::GetSkinFolder()
{
	return L"layouts";
}

std::wstring CLoginWnd::GetSkinFile()
{
	return L"login/login3.xml";
}

std::wstring CLoginWnd::GetWindowClassName() const
{
	return kClassName;
}

ui::Control* CLoginWnd::CreateControl(const std::wstring& pstrClass)
{
	// 扫描 XML 发现有名称为 CefControl 的节点，则创建一个 ui::CefControl 控件
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}

	return NULL;
}

void CLoginWnd::InitWindow()
{
	// 监听鼠标单击事件
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&CLoginWnd::OnClicked, this, std::placeholders::_1));

	// 从 XML 中查找指定控件
	cef_control_		= dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control"));
	cef_control_dev_	= dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control_dev"));
	btn_dev_tool_		= dynamic_cast<ui::Button*>(FindControl(L"btn_dev_tool"));
	ui::Button *closebtn = dynamic_cast<ui::Button*>(FindControl(L"close_btn"));
	closebtn->AttachClick([this](ui::EventArgs *args) {
		Close();
		return true;
	});

	 
	// 监听页面加载完毕通知
	cef_control_->AttachLoadEnd(nbase::Bind(&CLoginWnd::OnLoadEnd, this, std::placeholders::_1));

	cef_control_->AttachBeforeBrowser(nbase::Bind(&CLoginWnd::OnBeforeBrowser, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	cef_control_->AttachAfterCreated(nbase::Bind(&CLoginWnd::OnAfterCreated, this, std::placeholders::_1));


	// 打开开发者工具
	//cef_control_->AttachDevTools(cef_control_dev_);

	// 加载皮肤目录下的 html 文件
	//cef_control_->LoadURL(nbase::win32::GetCurrentModuleDirectory() + L"resources\\themes\\default\\cef\\cef.html");
	//cef_control_->LoadURL(L"https://passport.csdn.net/login?code=public");

	//if (!nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
	//	cef_control_dev_->SetVisible(false);
}

LRESULT CLoginWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	nim_comp::CefManager::GetInstance()->PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void CLoginWnd::CloseWnd() { 
	 Close();
}

void CLoginWnd::LoadURL(std::wstring url) {
	cef_control_->LoadURL(url);
}

bool CLoginWnd::OnClicked(ui::EventArgs* msg)
{
	std::wstring name = msg->pSender->GetName();

	if (name == L"btn_dev_tool")
	{
		if (cef_control_->IsAttachedDevTools())
		{
			cef_control_->DettachDevTools();
		}
		else
		{
			cef_control_->AttachDevTools(cef_control_dev_);
		}

		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
		{
			cef_control_dev_->SetVisible(cef_control_->IsAttachedDevTools());
		}
	}

	return true;
}

void CLoginWnd::OnLoadEnd(int httpStatusCode)
{
	OutputDebugString(L"CLoginWnd::OnLoadEnd ------->");

}


/*
*
*  @return   false, 正常加载网页；    true， 不加载网页
*/

bool CLoginWnd::OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

	OutputDebugString(std::wstring(request->GetURL().ToWString()).append(L"    ----------         CLoginWnd::OnBeforeBrowser ").c_str());
	OutputDebugString(std::wstring(L"CLoginWnd::OnBeforeBrowser3      ").append(request->GetURL().ToWString()).c_str());
	std::wstring url = request->GetURL().ToWString();
	std::wstring NN = L"dev-box.nn.com";

	if (url.find(NN) != url.npos   )    // find it.  weixin login
	{
		OutputDebugString(std::wstring(L"CLoginWnd::OnBeforeBrowser3  back to login page").c_str());
		login_box_->LoadUrl(url);
		Close();
		return true;
	}

	return false;
}

void  CLoginWnd::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	OutputDebugString(L"OnAfterCreated ------->");
}
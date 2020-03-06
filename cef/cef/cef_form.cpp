#include "stdafx.h"
#include "cef_form.h"
 

const std::wstring CefForm::kClassName = L"CEF_Control_Example";

CefForm::CefForm()
{
}

CefForm::~CefForm()
{
}

std::wstring CefForm::GetSkinFolder()
{
	return L"cef";
}

std::wstring CefForm::GetSkinFile()
{
	return L"cef.xml";
}

std::wstring CefForm::GetWindowClassName() const
{
	return kClassName;
}

ui::Control* CefForm::CreateControl(const std::wstring& pstrClass)
{
	// ɨ�� XML ����������Ϊ CefControl �Ľڵ㣬�򴴽�һ�� ui::CefControl �ؼ�
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}

	return NULL;
}

void CefForm::InitWindow()
{
	// ������굥���¼�
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&CefForm::OnClicked, this, std::placeholders::_1));

	// �� XML �в���ָ���ؼ�
	//cef_control_		= dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control"));
	//cef_control_dev_	= dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control_dev"));
	ui::VBox *container = dynamic_cast<nim_comp::VBox*>(FindControl(L"login_container_box"));
	m_CefLogin = new nui::CLoginVBOX(container, this);
	m_CefLogin->construct(0,50,600,500);

	btn_dev_tool_		= dynamic_cast<ui::Button*>(FindControl(L"btn_dev_tool"));
	edit_url_			= dynamic_cast<ui::RichEdit*>(FindControl(L"edit_url"));

	// �����������ʽ
	edit_url_->SetSelAllOnFocus(true);
	edit_url_->AttachReturn(nbase::Bind(&CefForm::OnNavigate, this, std::placeholders::_1));

 

	// ����Ƥ��Ŀ¼�µ� html �ļ�
	//cef_control_->LoadURL(nbase::win32::GetCurrentModuleDirectory() + L"resources\\themes\\default\\cef\\cef.html");
	m_CefLogin->LoadUrl(L"https://passport.csdn.net/login?code=public");

 
}

LRESULT CefForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	nim_comp::CefManager::GetInstance()->PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool CefForm::OnClicked(ui::EventArgs* msg)
{
	std::wstring name = msg->pSender->GetName();

	if (name == L"btn_dev_tool")
	{
 
	}
	else if (name == L"btn_back")
	{
		//cef_control_->GoBack();
	}
	else if (name == L"btn_forward")
	{
		//cef_control_->GoForward();
	}
	else if (name == L"btn_navigate")
	{
		OnNavigate(nullptr);
	}
	else if (name == L"btn_refresh")
	{
		//cef_control_->Refresh();

		nim_comp::Toast::ShowToast(L"CallJSFunction", 3000, GetHWND());
 
	}

	return true;
}

bool CefForm::OnNavigate(ui::EventArgs* msg)
{
	if (!edit_url_->GetText().empty())
	{
		m_CefLogin->LoadUrl(edit_url_->GetText());
		m_CefLogin->SetFocus();
	}

	return true;
}

void CefForm::OnLoadEnd(int httpStatusCode)
{
	OutputDebugString(L"OnLoadEnd ------->");
	//FindControl(L"btn_back")->SetEnabled(cef_control_->CanGoBack());
	//FindControl(L"btn_forward")->SetEnabled(cef_control_->CanGoForward());

 

}


/*
*
*  @return   false, ����������ҳ��    true�� ��������ҳ
*/

bool CefForm::OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

	OutputDebugString(std::wstring(request->GetURL().ToWString()).c_str());
	return false;
}

void  CefForm::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	OutputDebugString(L"OnAfterCreated ------->");
}
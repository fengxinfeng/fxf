/*
date: 2020.02.25
author: fengxinfeng
*/
#include "stdafx.h"
#include "CLoginVBOX.h" 
#include "cef_form.h"

namespace nui {


	CLoginVBOX::CLoginVBOX(ui::VBox *p , CefForm *mainwnd) :CSubVBox(p) {
		OutputDebugString(L"CLoginVBOX::CLoginVBOX ----->");
		
		m_MainHwnd = mainwnd->GetHWND();
		m_MainWnd = mainwnd;
		m_parent = p;
		//auto callback = nbase::Bind(&ui::WindowImplBase::CreateControl, m_MainWnd, std::placeholders::_1);
		//ui::GlobalManager::FillBoxWithCache(this, L"cef/cef_login.xml", callback);  //L"layouts/playimage.xml"  L"cef/cef_login.xml"
		//m_parent->Add(this);
		OutputDebugString(L"CLoginVBOX::CLoginVBOX <-----");
	}

	CLoginVBOX::~CLoginVBOX( )  {
	}

	void CLoginVBOX::construct(int left, int top, int width, int height) {
		OutputDebugString(L"CLoginVBOX::construct ---->");

		auto callback = nbase::Bind(&CLoginVBOX::CreateControl, this, std::placeholders::_1);
		ui::GlobalManager::FillBoxWithCache(this, L"cef/cef_login.xml", callback);  //L"layouts/playimage.xml"  L"cef/cef_login.xml"
		m_parent->Add(this);
		//WCHAR margin[64];
		//wsprintf(margin, L"%d,%d,0,0",left,top);
		//SetAttribute(L"margin",margin);
		WCHAR swidth[32];
		wsprintf(swidth, L"%d", width);
		SetAttribute(L"width", swidth);
		WCHAR sheight[32];
		wsprintf(sheight, L"%d", height);
		SetAttribute(L"height", sheight);

		cef_control_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control"));
		cef_control_->SetVisible(true);
		cef_control_dev_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control_dev"));

		OutputDebugString(L"CLoginVBOX::construct - 1");
		// 监听页面加载完毕通知
		cef_control_->AttachLoadEnd(nbase::Bind(&CLoginVBOX::OnLoadEnd, this, std::placeholders::_1));

		cef_control_->AttachBeforeBrowser(nbase::Bind(&CLoginVBOX::OnBeforeBrowser, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

		cef_control_->AttachAfterCreated(nbase::Bind(&CLoginVBOX::OnAfterCreated, this, std::placeholders::_1));

		if (!nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			cef_control_dev_->SetVisible(false);
		// 打开开发者工具
		//cef_control_->AttachDevTools(cef_control_dev_);

		OutputDebugString(L"CLoginVBOX::construct - 2");
		if (!isBindEvent) { 
			ui::Button * pClose = dynamic_cast<ui::Button*>(FindSubControl(L"close_btn"));
			pClose->AttachClick([this](ui::EventArgs* args) {
				m_parent->SetVisible(false);
				return true;
			});


			ui::Button* btn_dev_tool_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_dev_tool"));

			btn_dev_tool_->AttachClick([this](ui::EventArgs* args) {
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
				return true;
			});

			isBindEvent = true;
		}
 
		// 注册一个方法提供前端调用
		cef_control_->RegisterCppFunc(L"ShowMessageBox", ToWeakCallback([this](const std::string& params, nim_comp::ReportResultFunction callback) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_MainHwnd);
			callback(false, R"({ "message": "Success." })");
		}));
		OutputDebugString(L"CLoginVBOX::construct <-----");
	}


	void CLoginVBOX::LoadUrl(std::wstring url) {
		OutputDebugString(L"CLoginVBOX::LodUrl");
		cef_control_->LoadURL(url);
		//cef_control_dev_ = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control_dev"));
        // 打开开发者工具
        //cef_control_->AttachDevTools(cef_control_dev_);

        // 加载皮肤目录下的 html 文件
        //cef_control_->LoadURL(L"https://www.baidu.com/");

	}

	void CLoginVBOX::SetVisible(bool value) {
		WCHAR buf[64];
		wsprintf(buf, L"CLoginVBOX::SetVisible-----  %d", value);
		OutputDebugString(buf);
		if(m_parent)
			m_parent->SetVisible(value);
		CSubVBox::SetVisible(value);
		if(cef_control_)
		   cef_control_->SetVisible(value);
		OutputDebugString(L"CLoginVBOX::SetVisible<------");
	}
 

	void CLoginVBOX::OnLoadEnd(int httpStatusCode)
	{
		OutputDebugString(L"OnLoadEnd ------->");
		//FindControl(L"btn_back")->SetEnabled(cef_control_->CanGoBack());
		//FindControl(L"btn_forward")->SetEnabled(cef_control_->CanGoForward());



	}


	/*
	*
	*  @return   false, 正常加载网页；    true， 不加载网页
	*/

	bool CLoginVBOX::OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
	{
		//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

		OutputDebugString(std::wstring(request->GetURL().ToWString()).c_str());
		return false;
	}

	void  CLoginVBOX::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		OutputDebugString(L"OnAfterCreated ------->");
	}

	void CLoginVBOX::CallJS() {
		cef_control_->CallJSFunction(L"outnative", L"{\"first\":\"hello\"}", ToWeakCallback([this](const std::string& params) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_MainHwnd);
		}));
	}

	ui::Control* CLoginVBOX::CreateControl(const std::wstring& pstrClass)
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
 
} 
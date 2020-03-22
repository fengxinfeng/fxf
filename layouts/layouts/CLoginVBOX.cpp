/*
date: 2020.02.25
author: fengxinfeng
*/
#include "stdafx.h"
#include "CLoginVBOX.h"  
#include "include/cef_parser.h"
#include <fstream>
#include "CLogin_Wnd.h"

namespace nui {

	const static int g_weixinbox_w = 450;
	const static int g_weixinbox_h = 450;

	CLoginVBOX::CLoginVBOX(ui::VBox *p  ) :CSubVBox(p) {
		OutputDebugString(L"CLoginVBOX::CLoginVBOX ----->");		 
		m_parent = p; 
		auto callback = nbase::Bind(&CLoginVBOX::CreateControl, this, std::placeholders::_1);
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/login/cef_login.xml", callback); 
		m_parent->Add(this);
		m_MainHwnd = m_parent->GetWindow()->GetHWND();
		OutputDebugString(L"CLoginVBOX::CLoginVBOX <-----");
	}

	CLoginVBOX::~CLoginVBOX( )  {
	}

	void CLoginVBOX::construct(int left, int top, int width, int height) {
		OutputDebugString(L"CLoginVBOX::construct ---->");
	
		//WCHAR margin[64];
		//wsprintf(margin, L"%d,%d,0,0",left,top);
		//SetAttribute(L"margin",margin);
		WCHAR swidth[32];
		wsprintf(swidth, L"%d", width);
		//SetAttribute(L"width", swidth);
		WCHAR sheight[32];
		wsprintf(sheight, L"%d", height);
		//SetAttribute(L"height", sheight);

		cef_control_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control"));
		cef_control_dev_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control_dev"));
		m_fixed_height = GetFixedHeight();
		m_fixed_width = GetFixedWidth();
 
		//创建第三方登录box 
		m_thirdbox = dynamic_cast<ui::VBox*>(FindSubControl(L"third_box"));
		cef_control_3 = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control_3"));
		ui::Button * pCloseThirdBox = dynamic_cast<ui::Button*>(FindSubControl(L"close_btn_3"));
		pCloseThirdBox->AttachClick([this](ui::EventArgs* args) {
			m_thirdbox->SetVisible(false);
			return true;
		});
	
		// 监听页面加载完毕通知
		 
		cef_control_->AttachLinkClick(nbase::Bind(&CLoginVBOX::OnLinkClick, this, std::placeholders::_1));
		 
		cef_control_->AttachBeforeNavigate(nbase::Bind(&CLoginVBOX::OnBeforeNavigate, this, std::placeholders::_1, std::placeholders::_2));
		 		
		cef_control_->AttachUrlChange(nbase::Bind(&CLoginVBOX::OnUrlChange, this, std::placeholders::_1));

		cef_control_->AttachLoadEnd(nbase::Bind(&CLoginVBOX::OnLoadEnd, this, std::placeholders::_1));

		cef_control_->AttachBeforeBrowser(nbase::Bind(&CLoginVBOX::OnBeforeBrowser, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

		cef_control_->AttachAfterCreated(nbase::Bind(&CLoginVBOX::OnAfterCreated, this, std::placeholders::_1));

		cef_control_3->AttachBeforeBrowser(nbase::Bind(&CLoginVBOX::OnBeforeBrowser3, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

		//if (!nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			//cef_control_dev_->SetVisible(false);
		// 打开开发者工具
		//cef_control_->AttachDevTools(cef_control_dev_);

		OutputDebugString(L"CLoginVBOX::construct - 2");
		if (!isBindEvent) { 
			ui::Button * pClose = dynamic_cast<ui::Button*>(FindSubControl(L"close_btn"));
			pClose->AttachClick([this](ui::EventArgs* args) {
		        SetVisible(false);
				//CallJS();
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
		// 注册一个方法提供前端调用
		cef_control_->RegisterCppFunc(L"ShowMessageBox", ToWeakCallback([this](const std::string& params, nim_comp::ReportResultFunction callback) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_MainHwnd);
			callback(false, R"({ "message": "Success." })");
		}));

		cef_control_->RegisterCppFunc(L"CallNativeFun", ToWeakCallback([this](const std::string& params, nim_comp::ReportResultFunction callback) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params).append(L"     CallNativeFun"), 3000, m_MainHwnd);

			std::ofstream out(nbase::win32::GetCurrentModuleDirectory() + L"\\user.info", std::ios::out | std::ios::binary);
			out.write(params.c_str(), params.length());
			out.close();
			CefRefPtr<CefValue> jsonObject = CefParseJSON(params, JSON_PARSER_ALLOW_TRAILING_COMMAS);
			CefRefPtr<CefDictionaryValue> dict = jsonObject->GetDictionary();
			CefRefPtr<CefDictionaryValue> dictmsg = dict->GetDictionary("message");
			std::string  operation = dictmsg->GetString("operation");
			if (operation.compare("afterlogin") == 0) {
				CefRefPtr<CefDictionaryValue> data = dictmsg->GetDictionary("data");
				if (data != NULL && data->IsValid()) {
					std::string token = data->GetString("account_token");
					std::string mobile = "";
					std::string userid = "";
					CefRefPtr<CefDictionaryValue> login_info = data->GetDictionary("login_info");
					if (login_info != NULL && login_info->IsValid()) {
						int  id = login_info->GetInt("user_id");
						userid = nbase::UintToString(id);
						mobile = login_info->GetString("mobile");
					}
					else {
						OutputDebugString(L"CallNativeFun::afterlogin::data::login_info error ");
					}
					CefRefPtr<CefDictionaryValue> dictout = CefDictionaryValue::Create();
					dictout->SetString("userid", userid);
					dictout->SetString("mobile", mobile);
					dictout->SetString("token", token);
					CefRefPtr<CefValue> pValue = CefValue::Create();
					pValue->SetDictionary(dictout);
					CefString jsonString = CefWriteJSON(pValue, JSON_WRITER_DEFAULT);
					std::ofstream out(nbase::win32::GetCurrentModuleDirectory() + L"\\user.info", std::ios::out | std::ios::binary);
					out.write(jsonString.ToString().c_str(), jsonString.length());
					out.close();
				}
				else {
					OutputDebugString(L"CallNativeFun::afterlogin::data error ");
				}

			}

			callback(false, R"({ "message": "Success." })");
		}));
 
	}


	/*
	*
	*  @return   false, 正常加载网页；    true， 不加载网页
	*/

	bool CLoginVBOX::OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
	{
		//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

		OutputDebugString(std::wstring(L"CLoginVBOX::OnBeforeBrowser      ").append(request->GetURL().ToWString()).c_str());
		std::wstring url = request->GetURL().ToWString();
		std::wstring WINXIN = L"https://open.weixin.qq.com";

		if (url.find(WINXIN) != url.npos)    // find it.  weixin login
		{
			int left = (m_fixed_width - g_weixinbox_h )/ 2;
			int top = (m_fixed_height - g_weixinbox_h) / 2;
			top = top > 0 ? top : 0;
			WCHAR buf[32];
			//  正常窗口渲染不到浏览器窗口前，所以如果后面是浏览器，button栏看不见
			//wsprintf(buf, L"%d", g_weixinbox_w);
			//m_thirdbox->SetAttribute(L"width", buf);
			//wsprintf(buf, L"%d", g_weixinbox_h);
			//m_thirdbox->SetAttribute(L"height", buf); 
			//ui::UiRect r(left, top, 0, 0);
			//m_thirdbox->SetMargin(r); 
			//m_thirdbox->SetVisible(true);
			//cef_control_3->LoadURL(url);

			ui::UiRect pos(left, top, g_weixinbox_w, g_weixinbox_h);
			m_LoginWnd = CLoginWnd::ShowLoginWindow(pos, m_MainHwnd,this);
			m_LoginWnd->LoadURL(url);
			return true;
		}
		return false;
	}

	bool CLoginVBOX::OnBeforeBrowser3(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
	{
		//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

		OutputDebugString(std::wstring(L"CLoginVBOX::OnBeforeBrowser3      ").append(request->GetURL().ToWString()).c_str());
		std::wstring url = request->GetURL().ToWString();
		std::wstring NN = L"dev-box.nn.com";

		if (url.find(NN) != url.npos && m_thirdbox->IsVisible())    // find it.  weixin login
		{
			OutputDebugString(std::wstring(L"CLoginVBOX::OnBeforeBrowser3  back to login page").c_str());
			m_thirdbox->SetVisible(false);
			cef_control_->LoadURL(url);
			return true;
		}
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

	CefRequestHandler::ReturnValue CLoginVBOX::OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect)
	{
		// 返回RV_CANCEL截断导航
		request->GetURL();
		//OutputDebugStringA("OnBeforeNavigate::");
		//OutputDebugStringA(request->GetURL().ToString().c_str());
		return RV_CONTINUE;
	} 

	void CLoginVBOX::OnUrlChange(const std::wstring& url)
	{

	}

	bool CLoginVBOX::OnLinkClick(const std::wstring& url)
	{
		return true;
	}
 
} 
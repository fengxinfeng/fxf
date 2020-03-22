/*
date: 2020.02.19
author: fengxinfeng
*/
#pragma once

#include "CSubVBox.h"
class CLoginWnd;

namespace  nui {
	  
	class CLoginVBOX:public CSubVBox
	{
	public:
		CLoginVBOX(ui::VBox *p );
		~CLoginVBOX();
		void construct(int left, int top, int width, int height);
		void LoadUrl(std::wstring url);
 
		void SetVisible(bool value);   // override
		bool  OnLinkClick(const std::wstring& url);
		CefRequestHandler::ReturnValue  OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect);
		void  OnUrlChange(const std::wstring& url);
		void OnLoadEnd(int httpStatusCode);
		void  OnAfterCreated(CefRefPtr<CefBrowser> browser);
		bool OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect);
		bool OnBeforeBrowser3(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect);
		void CallJS();

		ui::Control*  CreateControl(const std::wstring& pstrClass);
	private:
		nim_comp::CefControlBase* cef_control_;
		nim_comp::CefControlBase* cef_control_3;
		ui::VBox * m_thirdbox; 
		nim_comp::CefControlBase* cef_control_dev_;
		HWND m_MainHwnd; 
		int m_fixed_width;
		int m_fixed_height;
		CLoginWnd * m_LoginWnd;
	};
}
 


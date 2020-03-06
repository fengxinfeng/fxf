/*
date: 2020.02.19
author: fengxinfeng
*/
#pragma once

#include "CSubVBox.h"
class CefForm;

namespace  nui {
	 
	class CLoginVBOX:public CSubVBox
	{
	public:
		CLoginVBOX(ui::VBox *p, CefForm *pwnd);
		~CLoginVBOX();
		void construct(int left, int top, int width, int height);
		void LoadUrl(std::wstring url);
 
		void SetVisible(bool value);   // override

		void OnLoadEnd(int httpStatusCode);
		void  OnAfterCreated(CefRefPtr<CefBrowser> browser);
		bool OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect);
		void CallJS();

		ui::Control*  CreateControl(const std::wstring& pstrClass);
	private:
		nim_comp::CefControlBase* cef_control_;
		nim_comp::CefControlBase* cef_control_dev_;
		HWND m_MainHwnd;
		CefForm * m_MainWnd;
	};
}
 


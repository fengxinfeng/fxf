/*
date: 2020.02.25
author: fengxinfeng
*/
#include "stdafx.h"
#include "CCefVBOX.h"
#include "CMainFrameUI.h"

namespace nui {


	CCefVBOX::CCefVBOX(CMainFrameUI *p) :CSubVBox(p) {
		OutputDebugString(L"CCefVBOX::CCefVBOX ----->");
		m_container = dynamic_cast<ui::VBox*>(p->FindControl(L"cef_container_box"));
		OutputDebugString(L"CCefVBOX::CCefVBOX <-----");
	}

	void CCefVBOX::construct(int left, int top, int width, int height) { 
		OutputDebugString(L"CCefVBOX::construct ---->");

		//WCHAR margin[64];
		//wsprintf(margin, L"%d,%d,0,0",left,top);
		//SetAttribute(L"margin",margin);
		WCHAR swidth[32];
		wsprintf(swidth, L"%d", width);
		SetAttribute(L"width", swidth);
		WCHAR sheight[32];
		wsprintf(sheight, L"%d", height);
		SetAttribute(L"height", sheight);

		if (!isBindEvent) {
			cef_control_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control"));
			ui::Button * pClose = dynamic_cast<ui::Button*>(FindSubControl(L"close_btn"));
			pClose->AttachClick([this](ui::EventArgs* args) {
				m_container->SetVisible(false);
				return true;
			});
			isBindEvent = true;
		}
 
		OutputDebugString(L"CCefVBOX::construct <-----");
	}


	void CCefVBOX::LodUrl(std::wstring url) {
		OutputDebugString(L"CCefVBOX::LodUrl");
		cef_control_->LoadURL(url);
		//cef_control_dev_ = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control_dev"));
        // 打开开发者工具
        //cef_control_->AttachDevTools(cef_control_dev_);

        // 加载皮肤目录下的 html 文件
        //cef_control_->LoadURL(L"https://www.baidu.com/");

	}

	void CCefVBOX::SetVisible(bool value) {
		WCHAR buf[64];
		wsprintf(buf, L"CCefVBOX::SetVisible-----  %d", value);
		OutputDebugString(buf);
		if(m_container)
		    m_container->SetVisible(value);
		CSubVBox::SetVisible(value);
		if(cef_control_)
		   cef_control_->SetVisible(value);
		OutputDebugString(L"CCefVBOX::SetVisible<------");
	}
 
} 
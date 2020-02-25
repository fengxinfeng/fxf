#pragma once

/*
date: 2020.02.19
author: fengxinfeng
*/
#include "CSubVBox.h"
class CMainFrameUI;

namespace  nui {
	class CCefVBOX:public CSubVBox
	{
	public:
		CCefVBOX(CMainFrameUI *p);
		~CCefVBOX() {};
		void construct(int left, int top, int width, int height);
		void LodUrl(std::wstring url);
 
		void SetVisible(bool value);   // override

	private:
		nim_comp::CefControlBase* cef_control_;
		nim_comp::CefControlBase* cef_control_dev_;
	};
}
 


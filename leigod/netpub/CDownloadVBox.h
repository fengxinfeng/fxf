/*
date: 2020.02.26
author: fengxinfeng
*/
#pragma once
#include "CSubVBox.h"

namespace nui {

	class CDownloadVBox :public CSubVBox
	{
	public:
		CDownloadVBox(CMainFrameUI *p);
		~CDownloadVBox() {};
		void SetVisible(bool value); 
	public:
		void CreateElementList(int count, int type);
		void SetProgressValueChanged(wstring gameid, float value);
 
	};
}
 
 


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
		CDownloadVBox(ui::VBox *p);
		~CDownloadVBox() {};
		void SetVisible(bool value); 
	public:
		void CreateElementList(int count, int type);
		void SetProgressValueChanged(std::wstring gameid, float value);
 
	};
}
 
 


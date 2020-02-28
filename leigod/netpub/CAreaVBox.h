#pragma once
 
/*
date: 2020.02.19
author: fengxinfeng
*/
#include "CSubVBox.h"
namespace  nui {

	class CAreaVBox : public CSubVBox
	{
	public:
		CAreaVBox(CMainFrameUI *p);
		~CAreaVBox() {};
	public:
		void CreateAreaElementList();
		void SelectAreaItem(wstring wsKey);
		bool   OnAreaSelected(ui::EventArgs* args);
	private:
		 
	};
}
 


#pragma once
 
/*
date: 2020.02.19
author: fengxinfeng
*/
#include "CSubVBox.h"
class CMainFrameUI;
namespace  nui {

	class CAreaVBox : public CSubVBox
	{
	public:
		CAreaVBox(CMainFrameUI *p);
		~CAreaVBox() {};
	public:
		void CreateAreaElementList();
		void SelectAreaItem(wstring wsKey);
	private:
		 
	};
}
 


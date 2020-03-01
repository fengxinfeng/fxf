/*
date: 2020.02.20
author: fengxinfeng
*/
#pragma once
#include "CSubVBox.h"
#include "engine_define.h"

namespace nui {
	class CSearchVBox :public CSubVBox
	{
	public:
		CSearchVBox(CMainFrameUI *p);
		~CSearchVBox();
		void   CreateSearchElementList(vector<STGame> vctNickGame);
		void  BindEventHandler();

	private:
		 
		bool  OnGameListClick(ui::EventArgs* args);

	};
}
 


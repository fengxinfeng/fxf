/*
date: 2020.02.23
author: fengxinfeng
*/
#pragma once
#include "CSubVBox.h"


namespace nui {
	class COperVBox : public CSubVBox
	{
	public:
		COperVBox(CMainFrameUI *p);
		~COperVBox();
		void   CreateOperatorElementList(int nGameID, int nAreaID);
		bool   OnOpertSelected(ui::EventArgs* args);

	private:
		void EnableCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID, BOOL bEnable);
		void  ClearListCtrl(ui::Control * pCtrl);
		wstring  Get_Isp_logo(int nIspType);
		void  OnOpertEvnBimd();
		bool  OnBtnOpertTabClick(ui::EventArgs* args);
	};
}
 


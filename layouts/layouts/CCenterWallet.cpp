/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterWallet.h"   

namespace nui { 

	CCenterWallet::CCenterWallet(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/wallet_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterWallet::~CCenterWallet() {}
	  

	void CCenterWallet::Construct() {
 
	}
	      

}

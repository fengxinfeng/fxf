/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterAccountSafe.h"   

namespace nui { 

	CCenterAccountSafe::CCenterAccountSafe(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/accountsafe_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterAccountSafe::~CCenterAccountSafe() {}
	  

	void CCenterAccountSafe::Construct() {
 
	}
	      

}

/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterAccountBind.h"   

namespace nui { 

	CCenterAccountBind::CCenterAccountBind(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/accountbind_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterAccountBind::~CCenterAccountBind() {}
	  

	void CCenterAccountBind::Construct() {
 
	}
	      

}

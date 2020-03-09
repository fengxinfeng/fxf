/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterHelp.h"   

namespace nui { 

	CCenterHelp::CCenterHelp(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/helpcenter_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterHelp::~CCenterHelp() {}
	  

	void CCenterHelp::Construct() {
 
	}
	      

}

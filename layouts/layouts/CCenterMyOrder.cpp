/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterMyOrder.h"   

namespace nui { 

	CCenterMyOrder::CCenterMyOrder(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/myorder_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterMyOrder::~CCenterMyOrder() {}
	  

	void CCenterMyOrder::Construct() {
 
	}
	      

}

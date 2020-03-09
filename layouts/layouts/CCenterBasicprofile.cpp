/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterBasicprofile.h"   

namespace nui { 

	CCenterBasicprofile::CCenterBasicprofile(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/basiprofile_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
	}
	 
	CCenterBasicprofile::~CCenterBasicprofile() {}
	  

	void CCenterBasicprofile::Construct() {
 
	}
	      

}

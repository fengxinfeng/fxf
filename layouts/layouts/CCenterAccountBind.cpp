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
 
		ui::ListBox * bindedgamelist = dynamic_cast<ui::ListBox*>(FindSubControl(L"bindedgame_list"));
		WCHAR  buf[64];
		for (int i = 0; i < 20; i++) {
			ui::ListContainerElement* item = buildSubListContainerElement(L"layouts\\mycenter\\game_item.xml");
			if (item) {
				OutputDebugString(L"CCenterAccountBind::Construct build item ok");
				ui::Label * account = dynamic_cast<ui::Label*>(item->FindSubControl(L"accountid")); 
				wsprintf(buf,L"game_%d",i);
				account->SetText(buf);
				bindedgamelist->Add(item);
			}
			else {
				OutputDebugString(L"CCenterAccountBind::Construct build item failed");
			}
		}
	}
	      

}

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
		OutputDebugString(L"CCenterAccountSafe::Construct -->");
		ui::Button* changephonebtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"changephone"));
		ui::Button* changepwdbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"changepwd"));
		ui::Button* bindqqbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"bindqq"));
		m_ShowPage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"showpage"));
		m_PwdPage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"changepwdpage"));
		m_PhonePage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"changephonepage"));
		m_BindqqPage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"bindqqpage"));

		changephonebtn->AttachClick([this](ui::EventArgs* args) {
			_cwprintf(L"CCenterAccountSafe:: changephonebtn");
			m_ShowPage->SetVisible(false);
			m_PhonePage->SetVisible(true);
			return true;
		});

		changepwdbtn->AttachClick([this](ui::EventArgs* args) {
			_cwprintf(L"CCenterAccountSafe:: changepwdbtn");
			m_ShowPage->SetVisible(false);
			m_PwdPage->SetVisible(true);
			return true;
		});

		bindqqbtn->AttachClick([this](ui::EventArgs* args) {
			_cwprintf(L"CCenterAccountSafe:: bindqqbtn");
			m_ShowPage->SetVisible(false);
			m_BindqqPage->SetVisible(true);
			return true;
		});

		ui::Button* nextbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"next"));
		ui::Button* pwd_yesbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"pwd_yes"));
		nextbtn->AttachClick([this](ui::EventArgs* args) {
			m_PhonePage->SetVisible(false);
			m_ShowPage->SetVisible(true);
			return true;
		});
		pwd_yesbtn->AttachClick([this](ui::EventArgs* args) {
			m_PwdPage->SetVisible(false);
			m_ShowPage->SetVisible(true);
			return true;
		});
		OutputDebugString(L"CCenterAccountSafe::Construct <----");
	}
	      

}

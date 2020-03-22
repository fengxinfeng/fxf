/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CMyCenter.h"    
#include "CCenterAccountBind.h"
#include "CCenterAccountSafe.h"
#include "CCenterBasicprofile.h"
#include "CCenterHelp.h"
#include "CCenterMyOrder.h"
#include "CCenterWallet.h"
#include "CCalendar.h"


namespace nui { 

	CMyCenter::CMyCenter(ui::VBox *p) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/mycenter.xml", NULL);
		p->Add(this); 
		m_parent = p; 
	}
	 
	CMyCenter::~CMyCenter() {}
	 
	void CMyCenter::SetVisibe(bool v) {
		m_parent->SetVisible(v);
		__super::SetVisible(v);
	}

	void CMyCenter::Construct() {
 
		//m_hwnd = this->GetWindow()->GetHWND();		
		//WCHAR buf[32];
		//wsprintf(buf, L"handler =  %lx", m_hwnd);
		//OutputDebugString(buf);
		OutputDebugString(L"CMyCenter::Construct  --->");
		ui::Button *close = dynamic_cast<ui::Button*>(FindSubControl(L"close_btn"));
		close->AttachClick([=](ui::EventArgs* args) {
			SetVisible(false);
			//calendar->ShowCalendar();
			return true;
		});
 
		m_NickName = dynamic_cast<ui::Label*>(FindSubControl(L"nickname"));
		m_nnid = dynamic_cast<ui::Label*>(FindSubControl(L"nnid")); 
		m_MyPhoto = dynamic_cast<ui::Control*>(FindSubControl(L"myphoto"));
		 
		ui::ButtonBox *basicprofile = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"basicprofile"));
		m_TabButton.push_back(basicprofile);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"basicprofile_l")));

		ui::ButtonBox *accountsafe = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"accountsafe"));
		m_TabButton.push_back(accountsafe);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"accountsafe_l")));

		ui::ButtonBox *accountbind = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"accountbind"));
		m_TabButton.push_back(accountbind);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"accountbind_l")));

		ui::ButtonBox *myorder = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"myorder"));
		m_TabButton.push_back(myorder);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"myorder_l")));

		ui::ButtonBox *wallet = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"wallet"));
		m_TabButton.push_back(wallet);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"wallet_l")));

		ui::ButtonBox *helpcenter = dynamic_cast<ui::ButtonBox*>(FindSubControl(L"helpcenter"));
		m_TabButton.push_back(helpcenter);
		m_TabLabel.push_back(dynamic_cast<ui::Control*>(FindSubControl(L"helpcenter_l")));
		 
		basicprofile->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(0);
			return true;
		});
		accountsafe->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(1);
			return true;
		});
		accountbind->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(2);
			return true;
		});
		myorder->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(3);
			return true;
		});
		wallet->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(4);
			return true;
		});
		helpcenter->AttachClick([this](ui::EventArgs* args) {
			SwitchTab(5);
			return true;
		});
		 
		ui::VBox *showcalendar = dynamic_cast<ui::VBox*>(FindSubControl(L"canlendar_container"));
		nui::CCalendar  *calendar = new nui::CCalendar(showcalendar);
		calendar->Construct();

		ui::VBox *formcontainer = dynamic_cast<ui::VBox*>(FindSubControl(L"centerform"));
		CCenterBasicprofile *basicform = new CCenterBasicprofile(formcontainer, calendar);
		basicform->Construct();			
		m_Forms.push_back(basicform);
 

		CCenterAccountSafe *accountsafeform = new CCenterAccountSafe(formcontainer);
		accountsafeform->Construct();
		m_Forms.push_back(accountsafeform);

		CCenterAccountBind * accountbindform = new CCenterAccountBind(formcontainer);
		accountbindform->Construct();
		m_Forms.push_back(accountbindform);

		m_Forms.push_back(new CCenterMyOrder(formcontainer));
		m_Forms.push_back(new CCenterWallet(formcontainer));
		m_Forms.push_back(new CCenterHelp(formcontainer)); 

		SwitchTab(0);
		OutputDebugString(L"CMyCenter::Construct  <------");

	}
	     
	void CMyCenter::SwitchTab(int serno) {
		OutputDebugString(L"CMyCenter::SwitchTab --->");
		for (size_t i = 0; i < m_TabButton.size(); i++) {
			m_TabButton.at(i)->SetAttribute(L"normaltextcolor", L"black");
		}
		m_TabButton.at(serno)->SetAttribute(L"normaltextcolor", L"green"); 
 
		for (size_t i = 0; i < m_TabLabel.size(); i++) {
			m_TabLabel.at(i)->SetBkColor(L"");
		}
		m_TabLabel.at(serno)->SetBkColor(L"green"); 

		for (auto c : m_Forms) {
			c->SetVisible(false);
		}
		m_Forms.at(serno)->SetVisible(true);
		OutputDebugString(L"CMyCenter::SwitchTab <----");
	}

}

/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CCenterBasicprofile.h"   

namespace nui { 

	CCenterBasicprofile::CCenterBasicprofile(ui::VBox *p, CCalendar * calendar) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/basiprofile_form.xml", NULL);
		p->Add(this); 
		m_parent = p;
		m_Calendar = calendar;
		m_Calendar->RegistCallback(this);
	}
	 
	CCenterBasicprofile::~CCenterBasicprofile() {}
 

	void CCenterBasicprofile::Construct() {

		OutputDebugString(L"CCenterBasicprofile::Construct -->");
		ui::Button* editbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"edit"));
		ui::Button* cancelbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"cancel"));
		ui::Button* savebtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"save"));
		m_ShowPage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"showpage"));
		m_EditPage = dynamic_cast<nim_comp::Box*>(FindSubControl(L"editpage"));
		editbtn->AttachClick([this](ui::EventArgs* args) { 
			m_ShowPage->SetVisible(false);
			m_EditPage->SetVisible(true);
			return true;
		});
		cancelbtn->AttachClick([this](ui::EventArgs* args) {
			m_ShowPage->SetVisible(true);
			m_EditPage->SetVisible(false);
			return true;
		});
		savebtn->AttachClick([this](ui::EventArgs* args) {
			m_ShowPage->SetVisible(true);
			m_EditPage->SetVisible(false);
			return true;
		});

		m_BirthdayEdit = dynamic_cast<nim_comp::RichEdit*>(FindSubControl(L"edit_birthday"));
		m_BirthdayEdit->AttachButtonUp([this](ui::EventArgs* args) {
			_cwprintf(L"  birthday button up, show calendar");
			m_Calendar->ShowCalendar();
			return true;
		});
		OutputDebugString(L"CCenterBasicprofile::Construct <----");
	}
	      
	void CCenterBasicprofile::OnDateSelected(Birthday& birthday) {
		_cwprintf(L"\n CCenterBasicprofile::OnDateSelected  %d %d %d   ", birthday.year, birthday.month, birthday.day);
		WCHAR buf[64];
		wsprintf(buf, L"%d/%02d/%02d", birthday.year, birthday.month, birthday.day);
		m_BirthdayEdit->SetText(buf);
	}

}

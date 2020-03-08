/*
 *date: 2020.03.07
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CSetting.h"   

#define  TIMER_PLAY_NEXT 1001


namespace nui {
	 
	CSetting::CSetting(ui::VBox *p) :CSubVBox(p)
	{ 
		m_parent = p;
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/setting.xml", NULL);
		p->Add(this); 
		m_hasinited = false;
		Construct();
	}
	 
	CSetting::~CSetting() {}

 
 

	void CSetting::Construct() {
  
		//WCHAR buf[32]; 
		//OutputDebugString(buf);

		//ui::Control *image = dynamic_cast<ui::Control*>(FindSubControl(L"one")); 
 

		ui::Button * yesbtn = dynamic_cast<ui::Button*>(FindSubControl(L"yes"));
		ui::Button * nobtn = dynamic_cast<ui::Button*>(FindSubControl(L"no"));
		m_normalbtn = dynamic_cast<ui::Button*>(FindSubControl(L"normalset"));
		m_downloadbtn = dynamic_cast<ui::Button*>(FindSubControl(L"downloadset"));
		ui::VBox * m_normalarea = dynamic_cast<ui::VBox*>(FindSubControl(L"normalarea"));
		ui::VBox * m_downloadarea = dynamic_cast<ui::VBox*>(FindSubControl(L"downloadarea"));

		if (yesbtn) { 
			yesbtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CSetting::CSetting   yes button");
				return true;
			});
		} 

		nobtn->AttachClick([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   nobtn  ");
			return true;
		});

		m_normalbtn->AttachClick([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   normalbtn  ");
			m_normalbtn->SetBkColor(L"green");
			m_downloadbtn->SetAttribute(L"bkcolor", L"");
			m_normalarea->SetVisible(true);
			m_downloadarea->SetVisible(false);
 
			return true;
		});

		m_downloadbtn->AttachClick([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   downloadbtn  ");
			m_downloadbtn->SetBkColor(L"green");
			m_normalbtn->SetAttribute(L"bkcolor", L"");
			m_downloadarea->SetVisible(true);
			m_normalarea->SetVisible(false);
			return true;
		});
 
		ui::CheckBox * autostart = dynamic_cast<ui::CheckBox*>(FindSubControl(L"autostart"));
		ui::CheckBox * autologin = dynamic_cast<ui::CheckBox*>(FindSubControl(L"autologin"));
		autostart->AttachClick([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   autostart clicked  "); 
			return true;
		});

		autostart->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   autostart unselected  ");
			return true;
		});

		autostart->AttachSelect([this](ui::EventArgs* args) {
			//int value = (int)args->wParam;
			//WCHAR buf[32]; 
			//wsprintf(buf, L"%d", value);
   //         OutputDebugString(buf);
			OutputDebugString(L"CSetting::CSetting   autostart selected  "); 
			return true;
		});

		autologin->AttachSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   autologin selected  ");
			return true;
		});

		autologin->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   autologin unselected  ");
			return true;
		});

		ui::Combo* combo = dynamic_cast<ui::Combo*>(FindSubControl(L"capturehotkey"));
		for (auto i = 0; i < 3; i++)
		{
			ui::ListContainerElement* element = new ui::ListContainerElement;
			element->SetClass(L"listitem");
			element->SetFixedHeight(30);
			element->SetBkColor(L"white");
			element->SetTextPadding({ 6,0,6,0 });
			switch (i) {
			case 0:
				element->SetText(L"ALT + 2");
				break;
			case 1:
				element->SetText( L"ALT + F12");
				break;
			case 2:
				element->SetText( L"ALT + X");
				break;
			}
			 
			combo->Add(element);
		}
		combo->SelectItem(0);
		combo->AttachSelect([=](ui::EventArgs* args) {
			int value = (int)args->wParam;
            WCHAR buf[32]; 
            wsprintf(buf, L"capturehotkey:: %d", value);
            OutputDebugString(buf); 
			return true;
		});

		ui::Option *close_min = dynamic_cast<ui::Option*>(FindSubControl(L"close_min"));
		ui::Option *close_quit = dynamic_cast<ui::Option*>(FindSubControl(L"close_quit"));
		close_min->AttachSelect([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting  close_min selected ");
			return true;
		});
		close_min->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   close_min unselected  ");
			return true;
		});
		close_quit->AttachSelect([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting  close_quit selected ");
			return true;
		});
		close_quit->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   close_quit unselected  ");
			return true;
		});

		ui::Option *start_min = dynamic_cast<ui::Option*>(FindSubControl(L"start_min"));
		ui::Option *start_keep = dynamic_cast<ui::Option*>(FindSubControl(L"start_keep"));
		start_min->AttachSelect([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting  start_min selected ");
			return true;
		});
		start_min->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   start_min unselected  ");
			return true;
		});
		start_keep->AttachSelect([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting  start_keep selected ");
			return true;
		});
		start_keep->AttachUnSelect([this](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting   start_keep unselected  ");
			return true;
		});

		ui::Button *clearcache = dynamic_cast<ui::Button*>(FindSubControl(L"clearcache"));
		clearcache->AttachClick([=](ui::EventArgs* args) {
			OutputDebugString(L"CSetting::CSetting  clearcache clicked ");
			return true;
		});

	}
 
}

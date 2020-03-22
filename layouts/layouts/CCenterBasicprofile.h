#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>
#include "CCalendar.h"

namespace  nui {

	class CCenterBasicprofile : public CSubVBox, public ICalendarCallback
	{
	public:
		CCenterBasicprofile(ui::VBox *p, CCalendar * calendar);
		~CCenterBasicprofile();
		void Construct(); 
		virtual  void OnDateSelected(Birthday&);
	protected: 
		 
	private: 
		bool  m_hasinited; 

	private:
		ui::Box * m_ShowPage;
		ui::Box * m_EditPage;
		nui::CCalendar *m_Calendar;
		ui::RichEdit* m_BirthdayEdit;
	};
}
 


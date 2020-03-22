#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	struct Birthday {
		int year;
		int month;
		int day;
	};

	class  ICalendarCallback {
	public:
		virtual  void OnDateSelected(Birthday&) = 0;
	};

	class CCalendar : public CSubVBox
	{
	public:
		CCalendar(ui::VBox *p );
		~CCalendar();
		void SetVisible(bool v);
		void Construct();
		void ShowCalendar();
		void RegistCallback(ICalendarCallback *);
	private:
		int DeYear();
		int InYear();
		int  DeMonth();
		int InMonth();
		int  dayNumber(int day, int month, int year);
		std::string  getMonthName(int monthNumber); 
		int numberOfDays(int monthNumber, int year);
		int GetNowTime();
		void build(int year, int month);
		bool OnDaySelected(ui::EventArgs* args);
		bool  OnYearSelected(ui::EventArgs* args);
		void buildyeararea(int year);
		int  GetStartIndex(int year, int month);

	private: 
		bool  m_hasinited; 
		std::vector<ui::Label*> m_days;
		std::vector<ui::Label*> m_years;
		int m_currentyear;
		int m_currentmonth;
		int m_selectday;
		ui::Label *m_LabelYear;
		ui::Label *m_LabelMonth;
		ICalendarCallback *m_CallBack;

		ui::VBox *  m_selectyeararea; 
		ui::VBox *  m_normalwordarea;  


	private: 
	};
}
 


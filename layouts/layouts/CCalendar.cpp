/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "stdafx.h"
#include <time.h>
#include "CCalendar.h"   

namespace nui { 
	using namespace std;
	CCalendar::CCalendar(ui::VBox *p ) :CSubVBox(p)
	{  
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/mycenter/calendar.xml", NULL);
		p->Add(this); 
		 
		m_parent = p;
	}

	void CCalendar::RegistCallback(ICalendarCallback * callback) {
		m_CallBack = callback;
	}

	CCalendar::~CCalendar() {}

	void CCalendar::SetVisible(bool v) {
		m_parent->SetVisible(v);
		__super::SetVisible(v);
	}
	  
	bool CCalendar::OnDaySelected(ui::EventArgs* args) {
		wstring name = args->pSender->GetName();
		ui::Label *w = dynamic_cast<nim_comp::Label*>(FindSubControl(name));
		wstring s = w->GetText();  
		long g = _wtol(s.c_str());
		if (g != 0) {
			m_selectday = (int)g;
			SetVisible(false);
			_cwprintf(L"select  %d\n", g);
			Birthday d;
			d.year = m_currentyear;
			d.month = m_currentmonth ;    
			d.day = m_selectday;
			m_CallBack->OnDateSelected(d);
		}
		return true;
	}

	bool CCalendar::OnYearSelected(ui::EventArgs* args) {
		wstring name = args->pSender->GetName();
		ui::Label *w = dynamic_cast<nim_comp::Label*>(FindSubControl(name));
		wstring s = w->GetText();
		long g = _wtol(s.c_str());
		if (g != 0) {
			m_currentyear = (int)g; 
			build(m_currentyear, m_currentmonth);
			_cwprintf(L"select  year %d\n", g);
		}
		m_selectyeararea->SetVisible(false);
		return true;
	}

	void CCalendar::Construct() {

		WCHAR buf[32];
		for (int i = 1; i < 43; i++) {		 
			wsprintf(buf, L"d%d", i);
			ui::Label * la = dynamic_cast<nim_comp::Label*>(FindSubControl(buf));
			la->AttachButtonUp(nbase::Bind(&CCalendar::OnDaySelected, this, std::placeholders::_1));
			m_days.push_back(la );
		} 
 
		for (int i =0; i < 16; i++) {
			wsprintf(buf, L"y%d", i);
			ui::Label * la = dynamic_cast<nim_comp::Label*>(FindSubControl(buf));
			la->AttachButtonUp(nbase::Bind(&CCalendar::OnYearSelected, this, std::placeholders::_1));
			m_years.push_back(la);
		}

		ui::Button* deyearbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"deyear")); 
		ui::Button* addyearbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"addyear"));
		ui::Button* demonthbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"demonth"));
		ui::Button* addmonthbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"addmonth"));
		m_selectyeararea =  dynamic_cast<nim_comp::VBox*>(FindSubControl(L"selectyeararea"));
		m_normalwordarea = dynamic_cast<nim_comp::VBox*>(FindSubControl(L"normalwordarea"));

		ui::Button* upyearbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"upyear"));
		ui::Button* afteryearbtn = dynamic_cast<nim_comp::Button*>(FindSubControl(L"afteryear"));

		m_LabelYear = dynamic_cast<nim_comp::Label*>(FindSubControl(L"year"));
		m_LabelMonth  = dynamic_cast<nim_comp::Label*>(FindSubControl(L"month"));

		m_LabelYear->AttachButtonUp([this](ui::EventArgs* args) {
			buildyeararea(m_currentyear);
			m_selectyeararea->SetVisible(true);
			return true;
		});

		upyearbtn->AttachClick([this](ui::EventArgs* args) {
			m_currentyear -= 16;
			buildyeararea(m_currentyear  ); 
			return true;
		});

		afteryearbtn->AttachClick([this](ui::EventArgs* args) {
			m_currentyear += 16;
			buildyeararea(m_currentyear ); 
			return true;
		});

		deyearbtn->AttachClick([this](ui::EventArgs* args) {
			m_currentyear--;
			build(m_currentyear, m_currentmonth);
       	    return true;
       }); 
		addyearbtn->AttachClick([this](ui::EventArgs* args) {
			m_currentyear++;
			build(m_currentyear, m_currentmonth);
			return true;
		});

		demonthbtn->AttachClick([this](ui::EventArgs* args) {

			m_currentmonth = m_currentmonth <= 1 ? 12 : --m_currentmonth;

			int index = dayNumber(1, m_currentmonth, m_currentyear);
			_cwprintf(L"demonthbtn:: year %d,  month %d  index  %d\n", m_currentyear, m_currentmonth, index);
			build(m_currentyear, m_currentmonth);
			return true;
		});

		addmonthbtn->AttachClick([this](ui::EventArgs* args) {
			m_currentmonth = m_currentmonth >= 12 ? 1 : ++m_currentmonth;

			int index = dayNumber(1, m_currentmonth, m_currentyear);
			_cwprintf(L"addmonthbtn:: year %d,  month %d  index  %d\n", m_currentyear, m_currentmonth, index);
			build(m_currentyear, m_currentmonth);
			return true;
		});
		_cwprintf(L" days lable is %d \n", m_days.size());
		 
	}

	void CCalendar::ShowCalendar() {
		GetNowTime();
		build(m_currentyear, m_currentmonth);

		SetVisible(true);
	}

	void CCalendar::build(int year, int month) {

		_cwprintf(L"CCalendar::build  year = %d  month= %d \n", year, month);
		int startpos = dayNumber(1, month, year);
		int numberofdays = numberOfDays(month -1, year);

		_cwprintf(L" \n  startpos %d      %d  year=%d   month=%d   m_currentmonth=%d\n", startpos, numberofdays,year, month, m_currentmonth);
		for (auto a : m_days) {
			a->SetText(L"");
		}

		WCHAR buf[64];
		wsprintf(buf, L"%dÔÂ", m_currentmonth  );
		m_LabelMonth->SetText(buf);
 
		wsprintf(buf, L"%dÄê", m_currentyear);
		m_LabelYear->SetText(buf);

		WCHAR day[32];
		for (int i = 1; i < numberofdays + 1; i++) {
			wsprintf(day, L"%d", i);
			m_days.at(startpos -1 + i)->SetText(day);
		}
	}
	      
	void CCalendar::buildyeararea(int year) {
		WCHAR yearbuf[32];
		for (size_t i = 0; i < m_years.size(); i++) {
			wsprintf(yearbuf, L"%d", year + i);
			m_years.at(i)->SetText(yearbuf);
		}
	}
	/*A Function that returns the index of the day 
  of the date- day/month/year
  For e.g-

  Index     Day
  0         Sunday
  1         Monday
  2         Tuesday
  3         Wednesday
  4         Thursday
  5         Friday
  6         Saturday*/
	int CCalendar::dayNumber(int day, int month, int year)
	{

		static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
						   4, 6, 2, 4 };
		year -= month < 3;
		return (year + year / 4 - year / 100 +
			year / 400 + t[month - 1] + day) % 7;
	}
	/*
	  A Function that returns the name of the month
	  with a given month number

	  Month Number     Name
	  0                January
	  1                February
	  2                March
	  3                April
	  4                May
	  5                June
	  6                July
	  7                August
	  8                September
	  9                October
	  10               November
	  11               December 
	  */
	string CCalendar::getMonthName(int monthNumber)
	{
		string months[] = { "January", "February", "March",
						   "April", "May", "June",
						   "July", "August", "September",
						   "October", "November", "December"
		};

		return (months[monthNumber]);
	}
	/* A Function to return the number of days in
	   a month

	  Month Number     Name        Number of Days
	  0                January     31
	  1                February    28 (non-leap) / 29 (leap)
	  2                March       31
	  3                April       30
	  4                May         31
	  5                June        30
	  6                July        31
	  7                August      31
	  8                September   30
	  9                October     31
	  10               November    30
	  11               December    31

	*/

	int CCalendar::numberOfDays(int monthNumber, int year)
	{
		// January 
		if (monthNumber == 0)
			return (31);

		// February 
		if (monthNumber == 1)
		{
			// If the year is leap then February has 
			// 29 days 
			if (year % 400 == 0 ||
				(year % 4 == 0 && year % 100 != 0))
				return (29);
			else
				return (28);
		}
		// March 
		if (monthNumber == 2)
			return (31);

		// April 
		if (monthNumber == 3)
			return (30);

		// May 
		if (monthNumber == 4)
			return (31);

		// June 
		if (monthNumber == 5)
			return (30);
		// July 
		if (monthNumber == 6)
			return (31);

		// August 
		if (monthNumber == 7)
			return (31);

		// September 
		if (monthNumber == 8)
			return (30);

		// October 
		if (monthNumber == 9)
			return (31);
		// November 
		if (monthNumber == 10)
			return (30);

		// December 
		if (monthNumber == 11)
			return (31);

		return 0;
	}

	int CCalendar::GetNowTime() {
		//time_t t = time(NULL);
		__time64_t long_time;

		//tm* timePtr = localtime_s(&t);
		struct tm newtime;
		_time64(&long_time);
		_localtime64_s(&newtime, &long_time);

		m_currentyear = newtime.tm_year  + 1900 - 20;
		m_currentmonth  = newtime.tm_mon + 1; 

		_cwprintf(L"CCalendar::GetNowTime %d  %d\n", m_currentyear, m_currentmonth);
		return 0;
		//cout << "seconds= " << timePtr->tm_sec << endl;
		//cout << "minutes = " << timePtr->tm_min << endl;
		//cout << "hours = " << timePtr->tm_hour << endl;
		//cout << "day of month = " << timePtr->tm_mday << endl;
		//cout << "month of year = " << timePtr->tm_mon << endl;
		//cout << "year = " << timePtr->tm_year << endl;
		//cout << "weekday = " << timePtr->tm_wday << endl;
		//cout << "day of year = " << timePtr->tm_yday << endl;
		//cout << "daylight savings = " << timePtr->tm_isdst << endl
	}

	int  CCalendar::GetStartIndex(int year, int month) {
		int days;

		// Index of the day from 0 to 6 
		int current = dayNumber(1, 1, year);
 
		printf("start pos of feberuery is %d \n", current);

		// i --> Iterate through all the months 
		// j --> Iterate through all the days of the 
		//       month - i 
		for (int i = 0; i < 12; i++)
		{
			days = numberOfDays(i, year);

			// Print the current month name 
			printf("\n  ------------%s-------------\n",
				getMonthName(i).c_str());

			// Print the columns 
			printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
			// Print appropriate spaces 
			int k;
			for (k = 0; k < current; k++)
				printf("     ");

			for (int j = 1; j <= days; j++)
			{
				printf("%5d", j);

				if (++k > 6)
				{
					k = 0;
					printf("\n");
				}
			}
			if (k)
				printf("\n");

			current = k;
			printf("start pos of month %d", current);
		}
		return 0;
	}
}

#pragma once
 
/*
 *date: 2020.03.07
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CSetting : public CSubVBox
	{
	public:
		CSetting(ui::VBox *p);
		~CSetting();
	protected: 
	public: 
	private:
		void Construct();
		bool  m_hasinited; 

		ui::Button * m_normalbtn;
		ui::Button * m_downloadbtn;

		ui::VBox * m_normalarea;
		ui::VBox * m_downloadarea;
		HWND m_hwnd;
		 
	};
}
 


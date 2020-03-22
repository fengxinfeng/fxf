#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterAccountSafe : public CSubVBox
	{
	public:
		CCenterAccountSafe(ui::VBox *p);
		~CCenterAccountSafe();
		void Construct();
	protected: 

	private:
		 
		bool  m_hasinited; 
	private:
		ui::Box* m_ShowPage;  
		ui::Box* m_PwdPage; 
		ui::Box*m_PhonePage; 
		ui::Box* m_BindqqPage;  
	};
}
 


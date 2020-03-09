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
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


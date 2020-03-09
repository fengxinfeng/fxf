#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterHelp : public CSubVBox
	{
	public:
		CCenterHelp(ui::VBox *p);
		~CCenterHelp();
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


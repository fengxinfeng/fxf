#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterBasicprofile : public CSubVBox
	{
	public:
		CCenterBasicprofile(ui::VBox *p);
		~CCenterBasicprofile();
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


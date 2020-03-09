#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterAccountBind : public CSubVBox
	{
	public:
		CCenterAccountBind(ui::VBox *p);
		~CCenterAccountBind();
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


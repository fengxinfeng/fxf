#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterMyOrder : public CSubVBox
	{
	public:
		CCenterMyOrder(ui::VBox *p);
		~CCenterMyOrder();
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


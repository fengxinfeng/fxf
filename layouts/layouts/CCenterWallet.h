#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CCenterWallet : public CSubVBox
	{
	public:
		CCenterWallet(ui::VBox *p);
		~CCenterWallet();
	protected: 

	private:
		void Construct(); 
		bool  m_hasinited; 
	};
}
 


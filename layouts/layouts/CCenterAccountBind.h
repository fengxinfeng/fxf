#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	struct Game_Info {
		std::wstring logo;
		std::wstring account;
	};

	class CCenterAccountBind : public CSubVBox
	{
	public:
		CCenterAccountBind(ui::VBox *p);
		~CCenterAccountBind();
		//void SetVisible(bool value);
		void Construct();
	protected: 

	private: 
		bool  m_hasinited; 
	};
}
 


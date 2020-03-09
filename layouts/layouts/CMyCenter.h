#pragma once
 
/*
 *date: 2020.03.09
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CMyCenter : public CSubVBox
	{
	public:
		CMyCenter(ui::VBox *p);
		~CMyCenter();
		void SetVisibe(bool v);
		void Construct();
	protected: 

	private:
		 
		void SwitchTab(int serno);
		bool  m_hasinited;
		std::vector<ui::ButtonBox *> m_TabButton;
		std::vector<ui::Control *> m_TabLabel;
		std::vector<ui::VBox *> m_Forms;
		HWND m_hwnd;
		ui::Label * m_NickName;
		ui::Label * m_nnid;
		ui::Control *m_MyPhoto;
	};
}
 


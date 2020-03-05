#pragma once
 
/*
 *date: 2020.03.05
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {

	class CTurnImage : public CSubVBox
	{
	public:
		CTurnImage(ui::VBox *p);
		~CTurnImage();
	protected:
		//void HandleMessage(ui::EventArgs &msg);
	public:
		void Start();
		void Stop();
		void ToLeft();
		void ToRight();
	private:
		void Construct();
		bool  m_hasinited;
		std::vector<ui::Control *> m_vector;
		HWND m_hwnd;
		 
	};
}
 


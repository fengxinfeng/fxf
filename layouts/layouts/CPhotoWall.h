#pragma once
 
/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector> 

namespace  nui {

	class CPhotoWall : public CSubVBox
	{
	public:
		CPhotoWall(ui::VBox *p);
		~CPhotoWall();
		/*
		*  二次构造
		* @param  data, 初始化数据
		*/
		void Construct(std::vector<std::wstring> data);
		void SetVisible(bool value);
	protected:
		//void HandleMessage(ui::EventArgs &msg);
	private:
		void ToLeft();
		void ToRight();
		 
	private:
		 
		bool  m_hasinited;
		std::vector<ui::Label *> m_vector;
		int m_StepLength;
		int m_Serno;
		int m_LeftMargin;
		int m_RightMargin;
		HWND m_hwnd;
		 
	};
}
 


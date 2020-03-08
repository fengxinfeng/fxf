#pragma once
 
/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector> 

namespace  nui {

	class CPhotoViewer : public CSubVBox
	{
	public:
		CPhotoViewer(ui::VBox *p);
		~CPhotoViewer();
		/*
		*  二次构造
		* @param  data, 初始化数据
		*/
		void Construct(std::vector<std::wstring> data);
		void SetVisible(bool value);
		void Start();
		void Stop();
		void DisplayNext();
	protected:
		//void HandleMessage(ui::EventArgs &msg);
	private:
		 
		void ToLeft();
		void ToRight();
		void AutoToLeft(); 
		void SetBorder(int num);
		 
	private:
		 
		bool  m_hasinited;
		std::vector<ui::Label *> m_vector;
		std::vector<std::wstring> m_data;
		int m_StepLength;
		int m_Serno;
		int m_LeftMargin;
		int m_RightMargin;
		ui::Label *m_labelserno;
		ui::Label *m_bigimage;
		int  m_MiddleNO;
		HWND m_hwnd;
		 
	};
}
 


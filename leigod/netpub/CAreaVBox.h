#pragma once

#include "duilib/UIlib.h" 

class CMainFrameUI;
namespace  nui {

	class CAreaVBox : public ui::VBox
	{
	public:
		CAreaVBox(CMainFrameUI *p) { m_parent = p;  };
		~CAreaVBox() {};
	public:
		void CreateAreaElementList();
		void SelectAreaItem(wstring wsKey);
	private:
		ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
		CMainFrameUI *m_parent;
	};
}
 


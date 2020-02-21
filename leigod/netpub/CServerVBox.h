#pragma once
 
#include "duilib/UIlib.h" 
#include "engine_define.h"

class CMainFrameUI;
namespace  nui {

	class CServerVBox : public ui::VBox
	{
	public:
		CServerVBox(CMainFrameUI *p);
		~CServerVBox() {};
	public:
		void CreateServerElementList(STGame &stGame);
		bool  OnSvrSelected(ui::EventArgs* args);
		//void SelectAreaItem(std::wstring wsKey);
	private:
		ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
		CMainFrameUI *m_parent;
	};

}



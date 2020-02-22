#pragma once
#include "duilib/UIlib.h" 

class CMainFrameUI;
namespace nui {

	class CSubVBox : public ui::VBox
	{
	public:
		CSubVBox(CMainFrameUI *p);
	protected:
		ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
		CMainFrameUI *m_parent;
	};
}
 

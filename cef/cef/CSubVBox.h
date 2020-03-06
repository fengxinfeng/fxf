#pragma once
#include "duilib/UIlib.h" 


namespace nui {

	class CSubVBox : public ui::VBox
	{
	public:
		CSubVBox(ui::VBox *parent);
	protected:
		ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
		ui::VBox *  buildSubView(const wchar_t * xmlName);
		virtual void BindEventHandler() {};
		bool isBindEvent;
		ui::VBox *m_parent;
		//ui::VBox  *m_container;
	};
}
 


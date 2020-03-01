#pragma once
#include "duilib/UIlib.h" 
 
class CreateXmlObj;
namespace nui {

	class CSubVBox : public ui::VBox
	{
	public:
		CSubVBox(CreateXmlObj *p);
	protected:
		ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
		virtual void BindEventHandler() {};
		bool isBindEvent;
		CreateXmlObj *m_parent;
		ui::VBox  *m_container;
	};
}
 


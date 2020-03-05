#include "stdafx.h"
#include "CSubVBox.h"

namespace nui {

	CSubVBox::CSubVBox(ui::VBox *p) 
	{ 
		m_parent = p;  
		isBindEvent = false;
	}


	ui::ListContainerElement * CSubVBox::buildSubListContainerElement(const wchar_t * xmlName)
	{
    
		ui::ListContainerElement * item = new ui::ListContainerElement;
		ui::GlobalManager::FillBoxWithCache(item, xmlName);
		return item;
	}

	ui::VBox *  CSubVBox::buildSubView(const wchar_t * xmlName) {
		ui::VBox * item = new ui::VBox; 
		ui::GlobalManager::FillBoxWithCache(item, xmlName, NULL);
		return item;
	}

}
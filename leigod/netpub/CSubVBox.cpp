#include "stdafx.h"
#include "CSubVBox.h"

namespace nui {

	CSubVBox::CSubVBox(CMainFrameUI *p) 
	{ 
		m_parent = p; 
	}


	ui::ListContainerElement * CSubVBox::buildSubListContainerElement(const wchar_t * xmlName)
	{
		//wstring s = L"CAreaVBox::buildSubListContainerElement     ";
		//s.append(xmlName);
		//OutputDebugString(s.c_str());

		ui::ListContainerElement * item = new ui::ListContainerElement;
		ui::GlobalManager::FillBoxWithCache(item, xmlName);
		return item;
	}

}
#pragma once
 
#include "CSubVBox.h" 
#include "engine_define.h"


namespace  nui {

	class CServerVBox : public CSubVBox
	{
	public:
		CServerVBox(CreateXmlObj *p);
		~CServerVBox() {};
	public:
		void CreateServerElementList(STGame &stGame);
		bool  OnSvrSelected(ui::EventArgs* args);
		//void SelectAreaItem(std::wstring wsKey);
	private:

	};

}



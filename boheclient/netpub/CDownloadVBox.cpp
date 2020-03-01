/*
date: 2020.02.25
author: fengxinfeng
*/
#include "stdafx.h"
#include "CDownloadVBox.h"
#include "CreateXmlObj.h"

namespace nui {


	CDownloadVBox::CDownloadVBox(CreateXmlObj *p) :CSubVBox(p) {
		//OutputDebugString(L"CDownloadVBox::CDownloadVBox ----->");
		//m_container = dynamic_cast<ui::VBox*>(p->FindControl(L"download_tabpage_vbox"));
		//OutputDebugString(L"CDownloadVBox::CDownloadVBox <-----");
	}

	/*void CDownloadVBox::SetVisible(bool value) {
		if (m_container) {
			m_container->SetVisible(value);
		}
		CSubVBox::SetVisible(value);
	}
	*/

	/*
	 *@param count   数量
	 *@param type     0, 正在下载的；1，下载完成的
	*/
	void CDownloadVBox::CreateElementList(int count, int type) {
		OutputDebugString(L"CDownloadVBox::CreateElementList  ----->.");

		if (!isBindEvent) {
			m_downloadtitle = dynamic_cast<ui::Button*>(FindSubControl(L"downloading_title"));
			m_downloadedtitle = dynamic_cast<ui::Button*>(FindSubControl(L"downloaded_title"));

			m_downloadtitle->AttachClick([this](ui::EventArgs* args) {
				m_downloadtitle->SetAttribute(L"normaltextcolor", L"my_start_text_color");
				m_downloadedtitle->SetAttribute(L"normaltextcolor", L"my_half_white");
				return true;
			});
 
			m_downloadedtitle->AttachClick([this](ui::EventArgs* args) {
				m_downloadedtitle->SetAttribute(L"normaltextcolor", L"my_start_text_color");
				m_downloadtitle->SetAttribute(L"normaltextcolor", L"my_half_white");
				return true;
			});
			isBindEvent = true;
		}


		ui::ListBox * plist = dynamic_cast<ui::ListBox*>(FindSubControl(L"download_list"));
		if (!plist) {
			OutputDebugString(L"CDownloadVBox::CreateElementList , can not find download_list, return");
			return;
		}
		plist->RemoveAll();
		ui::ListContainerElement * element = NULL;
		for (int i = 0; i < count; i++) {
			if (type == 0) {
				OutputDebugString(L"build bh/bh_list_mode_xml/download_model.xml ---->");
				 element = buildSubListContainerElement(L"bh/bh_list_mode_xml/download_model.xml");
				 OutputDebugString(L"build bh/bh_list_mode_xml/download_model.xml  <-------");
				 WCHAR gameid[64];
				 wsprintf(gameid, L"game%d", i);
				 element->SetName(gameid);
				 plist->Add(element);
				 ui::Button * pausebtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"pause"));
				 if (pausebtn) {
					 OutputDebugString(L"CDownloadVBox::CreateElementList  find pause.");

					 pausebtn->AttachClick([this, element](ui::EventArgs* args) {
						 wstring  itemname = element->GetName();
						 OutputDebugString(itemname.append(L"     gameid").c_str());
						 OutputDebugString(L"CDownloadVBox::CreateElementList  pause click.");
						 return true;
					 });
				 }
				 else {
					 OutputDebugString(L"CDownloadVBox::CreateElementList cannot find pause button");
				 }

				 ui::Button * openfolderbtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"openfolder"));
				 if (openfolderbtn) {
					 OutputDebugString(L"CDownloadVBox::CreateElementList  find openfolder.");
					 openfolderbtn->AttachClick([this, element](ui::EventArgs* args) {
						 OutputDebugString(L"CDownloadVBox::CreateElementList openfolder click.");
						 return true;
					 });
				 }
				 else {
					 OutputDebugString(L"CDownloadVBox::CreateElementList cannot find openfolder button");
				 }

				 ui::Button * removebtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"remove"));
				 if (removebtn) {
					 OutputDebugString(L"CDownloadVBox::CreateElementList  find remove.");
					 removebtn->AttachClick([this, element](ui::EventArgs* args) {
						 OutputDebugString(L"CDownloadVBox::CreateElementList  remove click.");
						 return true;
					 });
				 }
				 else {
					 OutputDebugString(L"CDownloadVBox::CreateElementList cannot find remove button");
				 }
			}
			else {
				element = buildSubListContainerElement(L"bh/bh_list_mode_xml/downloaded_model.xml");

				WCHAR gameid[64];
				wsprintf(gameid, L"game%d", i);
				element->SetName(gameid);
				plist->Add(element);
				ui::Button * openfilebtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"openfile"));
				if (openfilebtn) {
					OutputDebugString(L"CDownloadVBox::CreateElementList  find openfile.");

					openfilebtn->AttachClick([this, element](ui::EventArgs* args) {
						wstring  itemname = element->GetName();
						OutputDebugString(itemname.append(L"     gameid").c_str());
						OutputDebugString(L"CDownloadVBox::CreateElementList  openfile click.");
						return true;
					});
				}
				else {
					OutputDebugString(L"CDownloadVBox::CreateElementList cannot find openfile button");
				}

				ui::Button * openfolderbtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"openfolder"));
				if (openfolderbtn) {
					OutputDebugString(L"CDownloadVBox::CreateElementList  find openfolder.");
					openfolderbtn->AttachClick([this, element](ui::EventArgs* args) {
						OutputDebugString(L"CDownloadVBox::CreateElementList openfolder click.");
						return true;
					});
				}
				else {
					OutputDebugString(L"CDownloadVBox::CreateElementList cannot find openfolder button");
				}

				ui::Button * removebtn = dynamic_cast<ui::Button *>(element->FindSubControl(L"remove"));
				if (removebtn) {
					OutputDebugString(L"CDownloadVBox::CreateElementList  find remove.");
					removebtn->AttachClick([this, element](ui::EventArgs* args) {
						OutputDebugString(L"CDownloadVBox::CreateElementList  remove click.");
						return true;
					});
				}
				else {
					OutputDebugString(L"CDownloadVBox::CreateElementList cannot find remove button");
				}
			}
 
		}

		ui::VBox      * pnone = dynamic_cast<ui::VBox*>(FindSubControl(L"item_none"));
		if (count == 0) {
			plist->SetVisible(false);
			pnone->SetVisible(true);
		}
		else {
			plist->SetVisible(true);
			pnone->SetVisible(false);
		}
		OutputDebugString(L"CDownloadVBox::CreateElementList  <------.");
	}

	void CDownloadVBox::SetProgressValueChanged(wstring gameid, float value) {
		OutputDebugString(wstring(L"CDownloadVBox::SetProgressValueChange--->    ").append(gameid).c_str());
		ui::ListBox * plist = dynamic_cast<ui::ListBox*>(FindSubControl(L"download_list"));
		uint32_t ItemCount = plist->GetCount();
		for (int i =0;  i < ItemCount;  i++ )
		{
			ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(plist->GetItemAt( i));
			if (gameid.compare(pElem->GetName()) == 0) {
				OutputDebugString(L" CDownloadVBox::SetProgressValueChanged  find item   ");
				break;
			}
		} 


		OutputDebugString(wstring(L"CDownloadVBox::SetProgressValueChange <------- ").c_str());
	}

}
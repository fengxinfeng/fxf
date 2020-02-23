#include "stdafx.h"
#include "CServerVBox.h"

#include "CAccLogicCenter.h"
#include "StringHelper.h"

#include "CMainFrameUI.h"

namespace  nui {

	CServerVBox::CServerVBox(CMainFrameUI *p):CSubVBox(p) {   };


	void CServerVBox::CreateServerElementList(STGame &stGame) 
	{
		OutputDebugString(L"CreateServerElementList---------->");

		ui::ListBox * pMainServerListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"server_listbox"));
		if (pMainServerListBox)
		{
			pMainServerListBox->RemoveAll();
			pMainServerListBox->AttachSelect(nbase::Bind(&CServerVBox::OnSvrSelected, this, std::placeholders::_1));
			//pMainServerListBox->AttachSelect([this, pMainServerListBox](ui::EventArgs* args) {
			//});


			for (STGameArea & itr : stGame.vctAreas)
			{
				ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/server_model_box.xml");
				if (pElem)
				{
					pElem->SetName(stralgo::StringHelper::FormatW(L"svr_%d", itr.nID));
					pMainServerListBox->Add(pElem);

					ui::Label * pLbSvr = dynamic_cast<ui::Label *>(m_parent->FindSubControlByName(pElem, L"game_name"));
					if (pLbSvr)
					{
						pLbSvr->SetText(itr.wsTitle);
					}

					ui::Label * pLbSubTitle = dynamic_cast<ui::Label *>(m_parent->FindSubControlByName(pElem, L"game_server"));
					if (pLbSubTitle)
					{
						pLbSubTitle->SetText(itr.wsSubTitle);
					}

					ui::Label * pAreaLogo = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"game_logo"));
					if (pAreaLogo)
					{
						pAreaLogo->SetBkImage(itr.wsLogoPath);
					}

					pElem->AttachMouseEnter([this](ui::EventArgs* args) {
						if (args->pSender)
						{
							args->pSender->SetBorderColor(L"green");
						}
						return true;
					});
					pElem->AttachMouseLeave([this](ui::EventArgs* args) {
						if (args->pSender)
						{
							auto pTip = m_parent->FindSubControlByName(args->pSender, L"loc_tip");

							ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(args->pSender);
							if (pTip && pTip->IsVisible() == false && (pElem) && pElem->IsSelected() == false)
							{
								args->pSender->SetBorderColor(L"no_select_node_border_color");
							}

						}
						return true;
					});
				}
			}


		}
	}



	bool CServerVBox::OnSvrSelected(ui::EventArgs* args)
	{
		OutputDebugString(L"OnSvrSelected  --------->");
		int current = args->wParam;

		int old = args->lParam;
		int nTipSelect = 0;
		if (current != 0)
		{
			old = max(0, args->lParam);
		}

		if (m_parent->m_nIsArccrate)
		{
			if (nTipSelect = m_parent->CreateTipMsgBox())
			{
				m_parent->SetStopAccrate();
			}
		}

		ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"server_listbox"));
		if (pMainAreaListBox)
		{
			uint32_t ItemCount = pMainAreaListBox->GetCount();
			if (0 <= current && current < ItemCount)
			{
				ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(current));
				if (pElem)
				{
					pElem->SetBorderColor(L"select_node_border_color");

					ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(true);
					}

					ui::Label * pSvrName = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"game_name"));

					ui::ButtonBox * pBtnSvrBtnBox = dynamic_cast<ui::ButtonBox *>(m_parent->FindControl(L"server_btnbox"));
					if (pBtnSvrBtnBox && pSvrName)
					{
						if (m_parent->m_nIsArccrate)
						{
							if (m_parent->m_nIsArccrate)
							{
								if (nTipSelect)
								{
									pBtnSvrBtnBox->SetText(pSvrName->GetText());
								}
							}
							else
							{
								pBtnSvrBtnBox->SetText(pSvrName->GetText());
							}
						}
						else
						{
							pBtnSvrBtnBox->SetText(pSvrName->GetText());
						}
					}

					wstring wsName = pElem->GetName();
					if (stralgo::StringHelper::BeginWith(wsName.c_str(), L"svr_"))
					{
						m_parent->OnAreaSelect(_wtoi(wsName.c_str() + wcslen(L"svr_")));
					}
				}
			}

			if (0 <= old && old < ItemCount)
			{
				ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(old));
				if (pElem)
				{
					pElem->SetBorderColor(L"no_select_node_border_color");

					ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(false);
					}
				}
			}

			auto parentXml = m_parent->m_mapXmlParentBox.find(L"push_window_box");
			parentXml->second->SetVisible(false);
		}

		return true;
	}

 

}
/*
date: 2020.02.19
author: fengxinfeng
*/
#include "stdafx.h"
#include "CAreaVBox.h"
#include "engine_define.h"
#include "CAccLogicCenter.h"
#include "StringHelper.h"
 
#include "CMainFrameUI.h"

namespace nui {

	CAreaVBox::CAreaVBox(CMainFrameUI *p) :CSubVBox(p)
	{
		OutputDebugString(L"CAreaVBox::CAreaVBox-->");
		m_container = dynamic_cast<ui::VBox*>(p->FindControl(L"push_window_box"));
		OutputDebugString(L"CAreaVBox::CAreaVBox<--");
	}

	void CAreaVBox::CreateAreaElementList() {
		OutputDebugString(L"CreateAreaElementList---------->");
		int i = 0;
		std::wstring game_id;
		std::wstring id_numb;
		ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"area_listbox"));
		ui::Button * pClose = dynamic_cast<ui::Button*>(FindSubControl(L"area_window_close_btn"));
		pClose->AttachClick([this](ui::EventArgs* args) {
			m_container->SetVisible(false);
			return true;
		});

		if (pMainAreaListBox)
		{
			pMainAreaListBox->AttachSelect([this, pMainAreaListBox](ui::EventArgs* args) {
				int current = args->wParam;
				int old = args->lParam;
				int nTipSelect = 0;
				WCHAR buf[64];
				wsprintf(buf,L"CreateAreaElementList::    current=%d   old=%d ", current, old);
				OutputDebugString(buf);
				if (current != 0)
				{
					old = max(0, args->lParam);
				}

				if (m_parent->m_nIsArccrate) // 先停止加速
				{
					if (nTipSelect = m_parent->CreateTipMsgBox())
					{
						m_parent->SetStopAccrate();
					}
				}

				bool bDoReplace(false); 
				if (pMainAreaListBox)
				{
					uint32_t ItemCount = pMainAreaListBox->GetCount();
					if (0 <= current && current < ItemCount)
					{
						ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(current));
						if (pElem)
						{

							//pElem->SetBorderColor(L"light_green");
							pElem->SetBorderColor(L"select_node_border_color"); //边框
							//ui::Control * pLocIcon = (FindSubControlByName(pElem, L"loc_icon"));
							//if (pLocIcon)
							//{
							//	pLocIcon->SetVisible(true);
							//}
							ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
							if (pLocTip)
							{
								//if (nTipSelect/*&&m_nIsArccrate*/)
								pLocTip->SetVisible(true);
							}

							ui::Label * pProvName = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"area_prov"));

							ui::ButtonBox * pBtnAreaBtnBox = dynamic_cast<ui::ButtonBox *>(m_parent->FindControl(L"area_btnbox"));
							if (pBtnAreaBtnBox && pProvName)
							{
								bDoReplace = true;
								m_parent->m_wsProvinceName = pProvName->GetText(); //复制给主界面
								if (m_parent->m_nIsArccrate)
								{
									if (nTipSelect)
									{
										OutputDebugString(wstring(L"area selected 0   ").append(pProvName->GetText()).c_str()  );
										pBtnAreaBtnBox->SetText(pProvName->GetText());
									}
								}
								else
								{
									OutputDebugString(wstring(L"area selected 1   ").append(pProvName->GetText()).c_str());
									pBtnAreaBtnBox->SetText(pProvName->GetText());
								}
							}
						}
					}

					if (0 <= old && old < ItemCount && old != current)
					{
						ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(old));
						if (pElem)
						{
							pElem->SetBorderColor(L"no_select_node_border_color");
							//ui::Control * pLocIcon = (FindSubControlByName(pElem, L"loc_icon"));
							//if (pLocIcon)
							//{
							//	pLocIcon->SetVisible(false);
							//}
							ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
							if (pLocTip)
							{
								pLocTip->SetVisible(false);
							}
						}
					}

					if (bDoReplace)
					{
						m_parent->OnGetGroupLineResponse(0);  //通知创建节点选择数据
					}

					// 选择后隐藏
					auto parentXml = m_parent->m_mapXmlParentBox.find(L"push_window_box");
					parentXml->second->SetVisible(false);
				}

				return true;
			});  //  attachselect
		 

			STProvice & rfProvinceInfo = LOGIC_CENTER()->GetProvince();
			for (STProvinceItem & itr : rfProvinceInfo.vctProvince)
			{
				game_id = L"area_btnbox_game_id_";
				id_numb = std::to_wstring(i);
				game_id += id_numb;
				ui::ListContainerElement* pElem = buildSubListContainerElement(L"leigod\\model\\area_model_box.xml");
				if (pElem)
				{
					wstring wsName(L"area_");
					wsName += (itr.wsKey);
					pElem->SetName(wsName);
					pMainAreaListBox->Add(pElem);
					ui::Label * pAreaLabel = dynamic_cast<ui::Label *>(m_parent->FindSubControlByName(pElem, L"area_prov"));
					if (pAreaLabel)
					{
						pAreaLabel->SetText(itr.wsName);
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

 
			SelectAreaItem(U2W(LOGIC_CENTER()->GetGeoIP().ssProviceCode));
		}
		OutputDebugString(L"CreateAreaElementList<-------");

	}

	void CAreaVBox::SelectAreaItem(wstring wsKey)
	{
 
		OutputDebugString(L"CAreaVBox::SelectAreaItem----->");
		ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"area_listbox"));
		if (pMainAreaListBox) {
			OutputDebugString(L"CAreaVBox::SelectAreaItem ----- find area_listbox ");
		}
		else {
			OutputDebugString(L"CAreaVBox::SelectAreaItem ----- cannot find area_listbox ");
			return;
		}
		for (int i = 0; i < pMainAreaListBox->GetCount(); i++)
		{
			ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(i));
			if (pElem)
			{

				wstring wsName = pElem->GetName();
				wstring s(L"area_");
				s.append(wsKey);
				//if (pElem->GetName() == stralgo::StringHelper::FormatW(L"area_%s", wsKey.c_str()))
				if(s.compare(pElem->GetName())==0)
				{
 
					pMainAreaListBox->SelectItem(i);
					pElem->SetBorderColor(L"select_node_border_color");
					ui::Control * pLocIcon = (m_parent->FindSubControlByName(pElem, L"loc_icon"));
					if (pLocIcon)
					{
						pLocIcon->SetVisible(true);
					}
					ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(true);
					}
				}
				else
				{
 
					pElem->SetBorderColor(L"no_select_node_border_color");
					ui::Control * pLocIcon = (m_parent->FindSubControlByName(pElem, L"loc_icon"));
					if (pLocIcon)
					{
						pLocIcon->SetVisible(false);
					}
					ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(false);
					}
				}
			}
		}
		OutputDebugString(L"CAreaVBox::SelectAreaItem");

	}


 

}

#include "stdafx.h"
#include "CAreaVBox.h"
#include "engine_define.h"
#include "CAccLogicCenter.h"
#include "StringHelper.h"
#include "CMainFrameUI.h"

namespace nui {
	COperVBox::COperVBox(CMainFrameUI *p) : CSubVBox(p) {

	}

	 COperVBox::~COperVBox() {

	}

	 void COperVBox::EnableCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID, BOOL bEnable)
	 {
		 auto itr = pWnd->FindControl(wsCtrlID);
		 if (itr)
		 {
			 itr->SetEnabled(bEnable);
		 }
	 }

	 void COperVBox::ClearListCtrl(ui::Control * pCtrl)
	 {
		 ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(pCtrl);
		 if (pMainOperatorListBox)
		 {
			 pMainOperatorListBox->RemoveAll();
			 pMainOperatorListBox->AttachSelect(nbase::Bind(&COperVBox::OnOpertSelected, this, std::placeholders::_1));
		 }
	 }

	 wstring COperVBox::Get_Isp_logo(int nIspType)
	 {
		 if (nIspType == 1)
		 {
			 return L"label/telcom.png";
		 }
		 else if (nIspType == 2)
		 {
			 return L"label/svr_logo.png";
		 }
		 else if (nIspType == 3)
		 {
			 return L"label/mobile.png";
		 }

		 return L"label/allsvr.png";
	 }

	 void COperVBox::OnOpertEvnBimd()
	 {
		 static wstring arrTable[] = { L"tj_btn_oper", L"dx_btn_oper", L"yd_btn_oper", L"lt_btn_oper" };//, L"other_btn_oper"
		 ui::Control * pParent = m_parent->FindControl(L"push_window_box");
		 if (pParent)
		 {
			 for (wstring & itr : arrTable)
			 {
				 ui::Button * pBut = dynamic_cast<ui::Button *>(m_parent->FindSubControlByName(pParent, itr));
				 if (pBut)
				 {
					 pBut->AttachClick(nbase::Bind(&COperVBox::OnBtnOpertTabClick, this, std::placeholders::_1));
				 }
			 }

		 }
	 }


	 bool COperVBox::OnBtnOpertTabClick(ui::EventArgs* args)
	 {
		 static wstring arrTable[] = { L"tj_btn_oper", L"dx_btn_oper", L"yd_btn_oper", L"lt_btn_oper" };//, L"other_btn_oper"
		 static wstring viewTable[] = { L"operator_listbox_recommend", L"operator_listbox_dx", L"operator_listbox_yd", L"operator_listbox_lt", L"operator_listbox_other" };
		 int nHit(-1);
		 for (int i = 0; i < _countof(viewTable); i++)
		 {
			 ui::Control * pView = m_parent->FindControl(viewTable[i]);
			 if (pView)
			 {
				 pView->SetVisible(false);
			 }
		 }
		 for (int i = 0; i < _countof(arrTable); i++)
		 {
			 if (args->pSender->GetName() == arrTable[i])
			 {
				 ui::Control * pView = m_parent->FindControl(viewTable[i]);
				 if (pView)
				 {
					 pView->SetVisible(true);
				 }

				 nHit = i;
				 ((ui::Button*)args->pSender)->SetStateTextColor(ui::kControlStateNormal, L"node_select_bk_color");
			 }
			 else
			 {
				 ui::Button * pBtn = dynamic_cast<ui::Button *>(m_parent->FindControl(arrTable[i]));
				 if (pBtn)
				 {
					 pBtn->SetStateTextColor(ui::kControlStateNormal, L"node_noselect_bk_color");
				 }
			 }
		 }

		 return true;
	 }

	void COperVBox::CreateOperatorElementList(int nGameID, int nAreaID)
	{
		WCHAR dbuf[64];
		wsprintf(dbuf, L"      gameid= %d   areaid=%d ", nGameID, nAreaID);
		OutputDebugString(wstring(L"COperVBox::CreateOperatorElementList").append(dbuf).c_str());
		OnOpertEvnBimd();
		static wstring arrListTable[] = { L"operator_listbox_other", L"operator_listbox_dx", L"operator_listbox_lt", L"operator_listbox_yd", L"operator_listbox_recommend" };
		//	STLineGroupStrore & groupData = LOGIC_CENTER()->FindLineGroup(nGameID, nAreaID);
		STGroupItem *  pGroupItem = LOGIC_CENTER()->FindGroupItem(nGameID, nAreaID, m_parent->m_wsProvinceName);
		
		wstring TAG(L"COPER--------    ");
		OutputDebugString(TAG.append(pGroupItem->wsGroupName).append(L"   ").append(pGroupItem->wsProvName).c_str());
		
		if (!pGroupItem  || pGroupItem->vctGroupLines.size() == 0) {
			OutputDebugString(L"COperVBox::CreateOperatorElementList   can not find data. ");
			return;
		}
	 
			
			EnableCtrl(m_parent, L"start_accrate_btn", TRUE);//激活加速按钮
			//清空各组数据
			ClearListCtrl(FindSubControl(L"operator_listbox_recommend"));
			ClearListCtrl(FindSubControl(L"operator_listbox_dx"));
			ClearListCtrl(FindSubControl(L"operator_listbox_yd"));
			ClearListCtrl(FindSubControl(L"operator_listbox_lt"));
			ClearListCtrl(FindSubControl(L"operator_listbox_other"));
			ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"operator_listbox_recommend"));
			if (pMainOperatorListBox)    //初始化推荐板块数据
			{
				STLineItem * pFastLineItem = NULL;

				//找到最快的节点
				for (STLineItem & itrf : pGroupItem->vctGroupLines)
				{
					WCHAR buf[128];
					TAG.clear();
					TAG.append(L"COPER    ");
					wsprintf(buf, L"delay %d  lineid  %d   isptype %d prototype %d", itrf.nDelay, itrf.nLineID, itrf.nIspType, itrf.nProtoType);
					OutputDebugString(TAG.append(buf).append(L"  wsLineName: ").append(itrf.wsLineName).append(L"  wsName: ").append(itrf.wsName).c_str());
					if (pFastLineItem == NULL)
					{
						pFastLineItem = &itrf;
					}
					else
					{
						if (pFastLineItem->nDelay > itrf.nDelay)
						{
							pFastLineItem = &itrf;
							OutputDebugString(L"COPER  find fast item");
						}
					}
				}


				{
					int nDelayGuard(1000);
 
						ui::ListContainerElement * pElemBest(NULL);

						STGeoIP & stGeoIP = LOGIC_CENTER()->GetGeoIP();  // 当前网络服务商类型
						STLineItem arrFackItem[6];      // 初始化6个数据
						for (int i = 0; i < 6; i++)
						{
							arrFackItem[i].wsLineName = L'A' + i;
						}
						bool bMatch = false;
						for (STLineItem & itr : pGroupItem->vctGroupLines)
						{
							if (stGeoIP.nIspType == itr.nIspType)
							{
								for (int i = 0; i < 6; i++)
								{
									if (arrFackItem[i].wsLineName == itr.wsLineName)
									{
										OutputDebugString(wstring(L"COPER1----").append(itr.wsLineName).c_str());
										arrFackItem[i] = itr;     //从数据中找出属于当前服务商的
										bMatch = true;

									}
								}
							}
						}
						for (int i = 0; i < 6 && pFastLineItem; i++)
						{
							if (arrFackItem[i].nLineID == 0)
							{
								WCHAR buf[32];
								wsprintf(buf, L"COPER find the fastitem %d ", i);
								OutputDebugString(buf);
								arrFackItem[i] = *pFastLineItem;
								arrFackItem[i].nDelay += (rand() % 3);
								arrFackItem[i].nIspType = stGeoIP.nIspType;
							}
						}
#if 0
						for (STLineItem & itr : groupData.stData.vctGroups[0].vctGroupLines)
						{
							if (itr.nIspType == 0 || stGeoIP.nIspType == itr.nIspType)
							{
								for (int i = 0; i < 6; i++)
								{
									if (arrFackItem[i].wsLineName == itr.wsLineName)
									{
										arrFackItem[i] = itr;
									}
								}

								ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/operator_model_box.xml");
								if (pElem)
								{
									ui::Control * pGameLogo = FindSubControlByName(pElem, L"game_logo");
									if (pGameLogo)
									{
										pGameLogo->SetBkImage(Get_Isp_logo(itr.nIspType));
									}
									ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
									if (pLocTip)
									{
										pLocTip->SetVisible(false);
									}
									ui::Control * pBestTip = (FindSubControlByName(pElem, L"best_tip"));
									if (pBestTip)
									{
										pBestTip->SetVisible(false);
									}
									pElem->SetName(stralgo::StringHelper::FormatW(L"line_%d_%d", itr.nLineID, itr.nIspType));
									vector<wstring> vctSplit;
									stralgo::StringHelper::SplitString(itr.wsValue, L'|', vctSplit);
									if (vctSplit.size() > 10)
									{
										ui::Label * pLineName = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"line_name"));
										if (pLineName)
										{
											pLineName->SetText(GetLineItemName(itr));
										}
									}

									ui::Label * pLbDelay = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"local_delay"));
									if (pLbDelay)
									{
										pLbDelay->SetText(stralgo::StringHelper::FormatW(L"本地延迟: %dms", itr.nDelay));
										if (itr.nDelay < nDelayGuard)
										{
											nDelayGuard = itr.nDelay;
											m_nSvrID = itr.nLineID;
											pElemBest = pElem;
										}
									}
									pMainOperatorListBox->Add(pElem);
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
											auto pTip = FindSubControlByName(args->pSender, L"loc_tip");

											if (pTip && pTip->IsVisible() == false)
											{
												args->pSender->SetBorderColor(L"no_select_node_border_color");
											}

										}
										return true;
									});
								}
							}


						}
#endif


						//排序
						std::sort(arrFackItem, arrFackItem + 6, [&](STLineItem & left, STLineItem & right) {
							return left.nDelay < right.nDelay;
						});

						for (int i = 0; i < 6; i++)
						{
							OutputDebugString(wstring(L"COPER   ").append(arrFackItem[i].wsLineName).c_str());
							arrFackItem[i].wsLineName = L'A' + i;
						}
						int nBest(-1);
						int i = 0;
						for (STLineItem & itr : arrFackItem)   //  推荐栏中加入数据
						{
							if (1)
							{

								ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/operator_model_box.xml");
								if (pElem)
								{
									ui::Control * pGameLogo = m_parent->FindSubControlByName(pElem, L"game_logo");
									if (pGameLogo)
									{
										pGameLogo->SetBkImage( Get_Isp_logo(itr.nIspType));
									}
									ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
									if (pLocTip)
									{
										pLocTip->SetVisible(false);
									}
									ui::Control * pBestTip = (m_parent->FindSubControlByName(pElem, L"best_tip"));
									if (pBestTip)
									{
										pBestTip->SetVisible(false);
									}
									pElem->SetName(stralgo::StringHelper::FormatW(L"line_%d_%d", itr.nLineID, itr.nIspType));
									vector<wstring> vctSplit;
									stralgo::StringHelper::SplitString(itr.wsValue, L'|', vctSplit);
									if (vctSplit.size() > 10)
									{
										ui::Label * pLineName = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"line_name"));
										if (pLineName)
										{
											pLineName->SetText(m_parent->GetLineItemName(itr));
										}
									}

									ui::Label * pLbDelay = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"local_delay"));
									pLbDelay->SetVisible(true);
									if (pLbDelay)
									{
										pLbDelay->SetText(stralgo::StringHelper::FormatW(L"本地延迟: %dms", itr.nDelay));
										if (itr.nDelay < nDelayGuard)
										{
											nDelayGuard = itr.nDelay;
											m_parent->m_nSvrID = itr.nLineID;
											pElemBest = pElem;
											nBest = i;
										}
									}
									pMainOperatorListBox->Add(pElem);
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

							i++;
						}

						if (pElemBest)  // 显示最优
						{
							pElemBest->SetBorderColor(L"node_select_bk_color");

							ui::Control * pLocTip = (m_parent->FindSubControlByName(pElemBest, L"loc_tip"));
							if (pLocTip)
							{
								pLocTip->SetVisible(true);
							}
							ui::Control * pBestTip = (m_parent->FindSubControlByName(pElemBest, L"best_tip"));
							if (pBestTip)
							{
								pBestTip->SetVisible(true);
							}
							ui::EventArgs args;
							args.wParam = nBest;
							args.lParam = -1;
							args.pSender = pMainOperatorListBox;
							OnOpertSelected(&args);
						}

						vector<STLineItem> arrIspCategory[4];

						{
							for (STLineItem & itr : pGroupItem->vctGroupLines)
							{
								if (itr.nIspType < _countof(arrListTable))
								{
									if (itr.nIspType < 4)
									{
										arrIspCategory[itr.nIspType].push_back(itr);
										WCHAR  dbuf[32];
										wsprintf(dbuf, L"parse data   isptype %d  ", itr.nIspType);
										OutputDebugString(dbuf);
									}

								}
							}
						}

						for (int k = 0; k < 4; k++)
						{
							STLineItem arrFackItem[6];
							for (int i = 0; i < 6; i++)
							{
								arrFackItem[i].wsLineName = L'A' + i;
							}

							bool bMatch(false);
							for (int i = 0; i < 6; i++)
							{
								for (STLineItem & itrli : arrIspCategory[k])
								{
									if (arrFackItem[i].wsLineName == itrli.wsLineName)
									{
										arrFackItem[i] = itrli;
										bMatch = true;
									}

								}
							}


							for (int i = 0; i < 6; i++)
							{
								if (arrFackItem[i].nLineID == 0)
								{
									arrFackItem[i] = *pFastLineItem;
									arrFackItem[i].nDelay += (rand() % 3);
									arrFackItem[i].nIspType = k;
								}
							}
							std::sort(arrFackItem, arrFackItem + 6, [&](STLineItem & left, STLineItem & right) {
								return left.nDelay < right.nDelay;
							});
							for (int i = 0; i < 6; i++)
							{
								arrFackItem[i].wsLineName = L'A' + i;
							}

							arrIspCategory[k].clear();
							for (int i = 0; i < 6; i++)
							{
								arrIspCategory[k].push_back(arrFackItem[i]);
							}

						}


						for (int i = 0; i < 4; i++)
						{
							for (STLineItem & itr : arrIspCategory[i])
							{
								ui::ListBox * pMainOperatorListBoxIsp = dynamic_cast<ui::ListBox*>(m_parent->FindControl(arrListTable[itr.nIspType]));
								if (pMainOperatorListBoxIsp)
								{
									ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/operator_model_box.xml");
									if (pElem)
									{
										ui::Control * pGameLogo = m_parent->FindSubControlByName(pElem, L"game_logo");
										if (pGameLogo)
										{
											pGameLogo->SetBkImage(Get_Isp_logo(itr.nIspType));
										}
										pElem->SetName(stralgo::StringHelper::FormatW(L"line_%d_%d", itr.nLineID, itr.nIspType));
										vector<wstring> vctSplit;
										stralgo::StringHelper::SplitString(itr.wsValue, L'|', vctSplit);
										if (vctSplit.size() > 10)
										{
											ui::Label * pLineName = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"line_name"));
											if (pLineName)
											{
												pLineName->SetText(m_parent->GetLineItemName(itr));
											}
										}
										ui::Label * pLbDelay = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"local_delay"));
										if (pLbDelay)
										{
											pLbDelay->SetText(stralgo::StringHelper::FormatW(L"本地延迟: %dms", itr.nDelay));
										}
										pMainOperatorListBoxIsp->Add(pElem);
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


				}
			}
	 
	}




	bool COperVBox::OnOpertSelected(ui::EventArgs* args)
	{
		int current = args->wParam;
		int old = args->lParam;
		int nTipSelect = 0;
		bool bNotHot = (-2 == old);
		if (current == old)
		{
			return false;
		}
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
		ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(args->pSender);
		if (pMainAreaListBox)
		{
			uint32_t ItemCount = pMainAreaListBox->GetCount();
			if (0 <= current && current < ItemCount)
			{
				ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(current));
				if (pElem)
				{
					if (!bNotHot)
					{
						pElem->SetBorderColor(L"green");

						ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
						if (pLocTip)
						{
							pLocTip->SetVisible(true);
						}
					}
					else
					{
						pElem->SetBorderColor(L"select_node_border_color");

						ui::Control * pLocTip = (m_parent->FindSubControlByName(pElem, L"loc_tip"));
						if (pLocTip)
						{
							pLocTip->SetVisible(false);
						}
					}


					wstring wsLineName;
					ui::Label * pLineName = dynamic_cast<ui::Label*>(m_parent->FindSubControlByName(pElem, L"line_name"));
					if (pLineName)
					{
						wsLineName = pLineName->GetText();
					}

					wstring wsName = pElem->GetName();
					if (stralgo::StringHelper::BeginWith(wsName.c_str(), L"line_"))
					{
						vector<wstring> vctSplt;
						stralgo::StringHelper::SplitString(wsName, L'_', vctSplt);
						if (vctSplt.size() > 2)
						{
							m_parent->m_nSvrID = _wtoi(vctSplt[1].c_str());
							int nIsp = _wtoi(vctSplt[2].c_str());
							ui::ButtonBox * pBtnOperatorBtnBox = dynamic_cast<ui::ButtonBox *>(m_parent->FindControl(L"operator_btnbox"));
							{
								if (m_parent->m_nIsArccrate)
								{
									if (nTipSelect)
									{
										if (0 && wsLineName.size())
										{
											pBtnOperatorBtnBox->SetText(get_isp_name(nIsp) + L" " + wsLineName);
										}
										else
										{
											pBtnOperatorBtnBox->SetText(get_isp_name(nIsp));
										}

									}
								}
								else
								{
									if (0 && wsLineName.size())
									{
										pBtnOperatorBtnBox->SetText(get_isp_name(nIsp) + L" " + wsLineName);
									}
									else
									{
										pBtnOperatorBtnBox->SetText(get_isp_name(nIsp));
									}

								}

							}
							if (m_parent->m_nSvrID)
							{

							}
						}
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
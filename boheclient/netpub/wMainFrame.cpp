#include "stdafx.h"
#include "wMainFrame.h"
#include "CTool.h"



const std::wstring wMainFrame::kClassName = L"bhMainFrame";

wMainFrame::wMainFrame()
{
}


wMainFrame::~wMainFrame()
{
}

std::wstring wMainFrame::GetSkinFolder()
{
	return L"bh";
}

std::wstring wMainFrame::GetSkinFile()
{
	return L"bh_mainframe_xml/bh_mainframe.xml";
}

std::wstring wMainFrame::GetWindowClassName() const
{
	return kClassName;
}

void wMainFrame::InitWindow()
{
	/*leigod_netpub::CProtocalHelper t;
	t.GetNationCodeList();*/
	/*BH_LOGIC_CENTER()->initLoginData();
	STCountryList &CountryList = BH_LOGIC_CENTER()->GetCountryCode();*/
	InitCreate();
	CreateTabLb();
	ui::Combo* combo = dynamic_cast<ui::Combo*>(FindControl(L"nation_com"));

	for (auto i = 0; i < 10; i++)  
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"Combo element %d", i));
		combo->Add(element);
	}
	//AddNickBox();
	//AddDownLoadBox();
	//AddSvrBox();
	//AddAreaBox();
	//AddNodeBox();
	//AddNoticeText();
	//BindBtn();
}

LRESULT wMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool wMainFrame::NavTabShow(std::wstring btnName)
{
	for (auto &p : m_btnVct)
	{
		if (p.btnName == btnName)
		{
			p.pBtn->SetStateImage(ui::ControlStateType::kControlStateNormal,L"file='btn_icon/nav_btn.png' source='0,0,164,50'");
			p.pBtn->SetStateImage(ui::ControlStateType::kControlStateHot, L"file='btn_icon/nav_btn.png' source='0,0,164,50'");
		}
		else
		{
			p.pBtn->SetStateImage(ui::ControlStateType::kControlStateNormal, L"file='btn_icon/nav_btn.png' source='328,0,492,50'");
			p.pBtn->SetStateImage(ui::ControlStateType::kControlStateHot, L"file='btn_icon/nav_btn.png' source='164,0,328,50'");
		}
	}
	return true;
}

bool wMainFrame::OnNodeClick(ui::EventArgs* args)
{
	std::wstring NodeBtnArry[] = { L"svr_btn",L"area_btn" ,L"node_btn" ,L"mode_btn" };
	std::wstring wstrName = args->pSender->GetName();

	auto pParent=m_mapFloatParentVb.find(L"select_svr_area_node_vb");
	if (pParent->second)
		pParent->second->SetVisible(true);

	if (wstrName == NodeBtnArry[0])
	{
	
		auto pSvr = m_mapFloatXmlList.find(L"svr_float");
		if (pSvr->second)
			pSvr->second->SetVisible(true);
		//AddSvrBox();
	}
	if (wstrName == NodeBtnArry[1])
	{
	}
	if (wstrName == NodeBtnArry[2])
	{
	}
	if (wstrName == NodeBtnArry[3])
	{
	}
	return 0;
}

bool wMainFrame::OnNavTabClick(ui::EventArgs* args)
{

	std::wstring navBtnarry[] = { L"main_myspeed_btn",L"game_download_btn",L"content_arccrate_btn",L"main_recharge_btn",L"bh_notice_btn"};
	std::wstring wstrName = args->pSender->GetName();

	NavTabShow(wstrName);
	if (wstrName == navBtnarry[0])
	{
		
		for (auto &pXml : m_mapNavTabXmlList)
		{
			if (L"nav_myspeed_tbpage" == pXml.first)
			{
				pXml.second->SetVisible(true);
			}
			else
			{
				pXml.second->SetVisible(false);
			}
		}
	}
	if (wstrName == navBtnarry[1])
	{
		for (auto &pXml : m_mapNavTabXmlList)
		{
			if (L"nav_download_tbpage" == pXml.first)
			{
				OutputDebugString(L"display download nav_download_tbpage");
				pXml.second->SetVisible(true);
				//m_downloadVBox->SetVisible(true);
			}
			else
			{
				pXml.second->SetVisible(false);
			}
		}
		/*
		auto tabNUll = m_mapTabXmlList.find(L"download_null_mode");
		if (tabNUll->second)
		{
			tabNUll->second->SetVisible(true);
		}
		*/
	}
	if (wstrName == navBtnarry[2])
	{
		for (auto &pXml : m_mapNavTabXmlList)
		{
			if (L"nav_content_arccrate_tbpage" == pXml.first)
			{
				pXml.second->SetVisible(true);
			}
			else
			{
				pXml.second->SetVisible(false);
			}
		}
	}
	if (wstrName == navBtnarry[3])
	{
	}
	if (wstrName == navBtnarry[4])
	{
		for (auto &pXml : m_mapNavTabXmlList)
		{
			if (L"nav_notice_tbpage" == pXml.first)
			{
				pXml.second->SetVisible(true);
			}
			else
			{
				pXml.second->SetVisible(false);
			}
		}
	}
	
	return true;
}

bool wMainFrame::OnLoginClick(ui::EventArgs* args)
{
	std::wstring LoginBtnArry[] = { L"main_login_btn",L"reg_btn",L"go_login_btn" };
	std::wstring wstrName = args->pSender->GetName();
	if (wstrName == LoginBtnArry[0])
	{
		auto pFloatParent = m_mapFloatParentVb.find(L"login_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(true);
		auto pFloat = m_mapFloatXmlList.find(L"login_float");
		if (pFloat->second)
			pFloat->second->SetVisible(true);
	}
	if (wstrName == LoginBtnArry[1])
	{
		auto pFloatLogin = m_mapFloatXmlList.find(L"child_login_float");
		if (pFloatLogin->second)
			pFloatLogin->second->SetVisible(false);
		auto pFloatReg = m_mapFloatXmlList.find(L"child_regedit_float");
		if (pFloatReg->second)
			pFloatReg->second->SetVisible(true);
	}
	if (wstrName == LoginBtnArry[2])
	{
		auto pFloatLogin = m_mapFloatXmlList.find(L"child_login_float");
		if (pFloatLogin->second)
			pFloatLogin->second->SetVisible(true);
		auto pFloatReg = m_mapFloatXmlList.find(L"child_regedit_float");
		if (pFloatReg->second)
			pFloatReg->second->SetVisible(false);
	}
	
	return true;
}

bool wMainFrame::OnDialogCloseClick(ui::EventArgs* args)
{
	std::wstring DialogCloseBtnArry[] = { L"login_close_btn",L"svr_close_btn",L"monitor_close_btn",L"set_path_close_btn",L"thrid_close_btn" };
	std::wstring wstrName = args->pSender->GetName();
	if (wstrName == DialogCloseBtnArry[0])
	{
		auto pFloatParent = m_mapFloatParentVb.find(L"login_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(false);
		auto pFloat = m_mapFloatXmlList.find(L"login_float");
		if (pFloat->second)
			pFloat->second->SetVisible(false);
	}
	if (wstrName == DialogCloseBtnArry[1])
	{
		
		auto pFloatParent = m_mapFloatParentVb.find(L"select_svr_area_node_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(false);
		auto pFloat = m_mapFloatXmlList.find(L"svr_float");
		if (pFloat->second)
			pFloat->second->SetVisible(false);
	}
	if (wstrName == DialogCloseBtnArry[2])
	{

		auto pFloatParent = m_mapFloatParentVb.find(L"monitor_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(false);
		auto pFloat = m_mapFloatXmlList.find(L"monitor_float");
		if (pFloat->second)
			pFloat->second->SetVisible(false);
	}
	if (wstrName == DialogCloseBtnArry[3])
	{

		auto pFloatParent = m_mapFloatParentVb.find(L"set_game_path_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(false);
		auto pFloat = m_mapFloatXmlList.find(L"set_game_path_float");
		if (pFloat->second)
			pFloat->second->SetVisible(false);
	}
	if (wstrName == DialogCloseBtnArry[4])
	{

		auto pFloatParent = m_mapFloatParentVb.find(L"thridparty_vb");
		if (pFloatParent->second)
		{
			pFloatParent->second->SetVisible(false);
		}
		auto pXml = m_mapFloatXmlList.find(L"thridparty_float");
		if (pXml->second)
		{
			pXml->second->SetVisible(false);
		}
	}
	return true;
}

void wMainFrame::CreateTabLb()
{
	TabBtnLb Lb; 
	std::wstring mySpeedTabBtnArryLb[] = { L"myspeed_btnb_lb",L"hot_btnb_lb" ,L"often_btnb_lb" ,L"all_btnb_lb" ,L"for_btnb_lb",L"chnn_btnb_lb",L"free_btnb_lb" };
	std::wstring mySpeedTabBtnArry[] = { L"myspeed_btnb",L"hot_btnb" ,L"often_btnb" ,L"all_btnb" ,L"for_btnb",L"chnn_btnb",L"free_btnb" };
	std::wstring myMonitorTabBtnArry[] = { L"monitor_net_btnb",L"monitor_log_btnb" };
	std::wstring myMonitorLbBtnArry[] = { L"monitor_net_icon_lb",L"monitor_log_icon_lb"};
	int i = 0;
	for (auto &lbName: mySpeedTabBtnArryLb)
	{
		
		ui::Label * pLb = dynamic_cast<ui::Label*>(FindControl(lbName));
		if (pLb)
		{
			Lb.btnParentName = mySpeedTabBtnArry[i];
			Lb.btnName = lbName;
			Lb.pBtn = pLb;
			m_TabLbVct.push_back(Lb);
		}
		i++;
	}
	i = 0;
	for (auto &lbMonitorName: myMonitorLbBtnArry)
	{
		ui::Label * pLb = dynamic_cast<ui::Label*>(FindControl(lbMonitorName));
		if (pLb)
		{
			Lb.btnParentName = myMonitorTabBtnArry[i];
			Lb.btnName = lbMonitorName;
			Lb.pBtn = pLb;
			m_MonitorTabLbVct.push_back(Lb);
		}
		i++;
	}
}
bool wMainFrame::OnTabClick(ui::EventArgs* args)
{
	std::wstring mySpeedTabBtnArry[] = { L"myspeed_btnb",L"hot_btnb" ,L"often_btnb" ,L"all_btnb" ,L"for_btnb",L"chnn_btnb",L"free_btnb" };
	std::wstring mySpeedTabBtnArryLb[] = { L"myspeed_btnb_lb",L"hot_btnb_lb" ,L"often_btnb_lb" ,L"all_btnb_lb" ,L"for_btnb_lb",L"chnn_btnb_lb",L"free_btnb_lb" };
	std::wstring wstrName = args->pSender->GetName();

	auto pTab = m_mapTabParentVb.find(L"tabpage_vb");
	if (pTab->second)
		pTab->second->SetVisible(true);

	for (auto &pbtn : m_mySpeedTabVct)
	{
		if (pbtn.btnName == wstrName)
		{
			dynamic_cast<ui::ButtonBox*>(pbtn.pBtn)->SetStateTextColor(ui::ControlStateType::kControlStateNormal, L"my_start_text_color");
		}
		else
		{
			dynamic_cast<ui::ButtonBox*>(pbtn.pBtn)->SetStateTextColor(ui::ControlStateType::kControlStateNormal, L"my_half_white");
		}
	}

	for (auto &pBtnLb : m_TabLbVct)
	{
		if (pBtnLb.btnParentName== wstrName)
		{
			pBtnLb.pBtn->SetVisible(true);
		}
		else
		{
			pBtnLb.pBtn->SetVisible(false);
		}
	}

	if (wstrName == mySpeedTabBtnArry[0])
	{
		for (auto &pMySpeed : m_mapTabXmlList)
		{
			if (pMySpeed.first == L"myspeed_tbpage")
			{
				if (pMySpeed.second)
					pMySpeed.second->SetVisible(true);
			}
			else
			{
				pMySpeed.second->SetVisible(false);
			}
		}
	}
	if (wstrName == mySpeedTabBtnArry[1])
	{
		for (auto &pMyPlat : m_mapTabXmlList)
		{
			if (pMyPlat.first == L"plat_tabpage")
			{
				if (pMyPlat.second)
					pMyPlat.second->SetVisible(true);
			}
			else
			{
				pMyPlat.second->SetVisible(false);
			}
		}
		for (auto &GameObj : m_GameVct)
		{
			GameObj.GameElement->SetVisible(true);
		}
		
	}
	if (wstrName == mySpeedTabBtnArry[2])
	{
	}
	if (wstrName == mySpeedTabBtnArry[3])
	{
	}
	if (wstrName == mySpeedTabBtnArry[4])
	{
	}
	if (wstrName == mySpeedTabBtnArry[5])
	{
	}
	if (wstrName == mySpeedTabBtnArry[6])
	{
	}
	return 0; 
}

bool wMainFrame::OnNickSelect(ui::EventArgs* args)
{

	return 0;
}

bool wMainFrame::OnMonitorClick(ui::EventArgs* args)
{
	std::wstring MonitorArry[] = { L"monitor_btn",L"monitor_net_btnb",L"monitor_log_btnb" };
	std::wstring wstrName = args->pSender->GetName();

	
	if (wstrName == MonitorArry[0])
	{
		auto pFloatParent = m_mapFloatParentVb.find(L"monitor_vb");
		if (pFloatParent->second)
			pFloatParent->second->SetVisible(true);
		auto pXml = m_mapFloatXmlList.find(L"monitor_float");
		if (pXml->second)
		{
			pXml->second->SetVisible(true);
		}
		return true;
	}

	for (auto &pBtnLb : m_MonitorTabLbVct)
	{
		if (pBtnLb.btnParentName == wstrName)
		{
			pBtnLb.pBtn->SetVisible(true);
		}
		else
		{
			pBtnLb.pBtn->SetVisible(false);
		}
	}

	if (wstrName == MonitorArry[1])
	{
		auto pNetXml = m_mapTabXmlList.find(L"monitor_net_tabpage");
		if (pNetXml->second)
		{
			pNetXml->second->SetVisible(true);
		}
		auto pLogXml = m_mapTabXmlList.find(L"monitor_log_tabpage");
		if (pLogXml->second)
		{
			pLogXml->second->SetVisible(false);
		}
	}
	if (wstrName == MonitorArry[2])
	{
		auto pNetXml = m_mapTabXmlList.find(L"monitor_net_tabpage");
		if (pNetXml->second)
		{
			pNetXml->second->SetVisible(false);
		}
		auto pLogXml = m_mapTabXmlList.find(L"monitor_log_tabpage");
		if (pLogXml->second)
		{
			pLogXml->second->SetVisible(true);
		}
	}

	return true;
}

bool wMainFrame::OnGamePathClick(ui::EventArgs* args)
{
	std::wstring GameOperationArry[] = { L"get_game_path_btn",L"game_path_dialog_btn" };
	std::wstring wstrName = args->pSender->GetName();
	std::wstring OpenGamePath = _T("");
	if (wstrName == GameOperationArry[0])
	{
		auto pFloatParent = m_mapFloatParentVb.find(L"set_game_path_vb");
		if (pFloatParent->second)
		{
			pFloatParent->second->SetVisible(true);
		}
		auto pXml = m_mapFloatXmlList.find(L"set_game_path_float");
		if (pXml->second)
		{
			pXml->second->SetVisible(true);
		}
	}
	if (wstrName == GameOperationArry[1])
	{
		OpenGamePath = bh_personpub::CTool::SelectPath();
		ui::RichEdit * pEdit = dynamic_cast<ui::RichEdit*>(FindControl(L"path_name_edit"));
		if (pEdit)
		{
			pEdit->SetText(OpenGamePath);
		}
	}
	return true;
}

bool wMainFrame::OnGameSelect(ui::EventArgs* args)
{
	std::wstring wstrName = args->pSender->GetName();
	ToMySpeed();
	return 0;
}


bool wMainFrame::OnMainPageBtnClick(ui::EventArgs* args)
{
	ToGame();
	return true;
}

bool wMainFrame::OnPhoneNumberTextChangeEvent(ui::EventArgs* args)
{
	
	return true;
}
bool wMainFrame::OnPhoneNumberKillFocusEvent(ui::EventArgs* args)
{
	return true;
}
bool wMainFrame::OnPhoneNumberGetFocusEvent(ui::EventArgs* args)
{
	return true;
}

bool wMainFrame::OnPwdTextChangeEvent(ui::EventArgs* args)
{

	return true;
}
bool wMainFrame::OnPwdKillFocusEvent(ui::EventArgs* args)
{
	return true;
}
bool wMainFrame::OnPwdGetFocusEvent(ui::EventArgs* args)
{
	return true;
}

/*************************¸¨Öúº¯Êý*************************************/
bool wMainFrame::ToGame()
{
	for (auto &pMyPlat : m_mapTabXmlList)
	{
		if (pMyPlat.first == L"plat_tabpage")
		{
			if (pMyPlat.second)
				pMyPlat.second->SetVisible(true);
		}
		else
		{
			pMyPlat.second->SetVisible(false);
		}
	}
	for (auto &GameObj : m_GameVct)
	{
		GameObj.GameElement->SetVisible(true);
	}
	return true;
}

bool wMainFrame::Switch_Download_Tabpage_VBox(const WCHAR *boxname)
{

	OutputDebugString(wstring(L"wMainFrame::Switch_Download_Tabpage_VBox   ").append(boxname).c_str());
	for (auto &box : m_mapTabXmlList)
	{
		if (box.first.compare(boxname) == 0 )
		{
			if (box.second) {
				OutputDebugString(wstring(L"wMainFrame::Switch_Download_Tabpage_VBox --------1  ").append(boxname).c_str());
				box.second->SetVisible(true);
			}
				 
		}
		else
		{
			box.second->SetVisible(false);
		}
	}
	return true;
} 

bool wMainFrame::ToMySpeed()
{
	OutputDebugString(wstring(L"wMainFrame::ToMySpeed   ").c_str());
	for (auto &pMySpeed : m_mapTabXmlList)
	{
		if (pMySpeed.first == L"myspeed_tbpage")
		{
			if (pMySpeed.second)
				pMySpeed.second->SetVisible(true);
		}
		else
		{
			pMySpeed.second->SetVisible(false);
		}
	}
	return true;
}

bool wMainFrame::OnThridLoginClick(ui::EventArgs* args)
{
	std::wstring ThridLoginBtnArry[] = { L"QQ_login_btn",L"WX_login_btn",L"WB_login_btn" };
	std::wstring wstrName = args->pSender->GetName();
	auto pFloatParent = m_mapFloatParentVb.find(L"thridparty_vb");
	if (pFloatParent->second)
	{
		pFloatParent->second->SetVisible(true);
	}
	auto pXml = m_mapFloatXmlList.find(L"thridparty_float");
	if (pXml->second)
	{
		pXml->second->SetVisible(true);
	} 
	if (wstrName== ThridLoginBtnArry[0])
	{

	}
	if (wstrName == ThridLoginBtnArry[1])
	{
	}
	if (wstrName == ThridLoginBtnArry[2])
	{
	}

	return true;
}
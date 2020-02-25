#include "stdafx.h"
#include "CMainFrameUI.h"
#include "uiMessageBox.h"
#include "uiTipMessageBox.h"
#include "StringHelper.h"
#include "resource.h"
#include "ui_components/ui_components.h"
#include "CProtocalHelper.h"
#include "Shlwapi.h" 

const std::wstring CMainFrameUI::kClassName = L"MainFrame";
list<uint32_t> gListBarControl;
extern CMainFrameUI * gMainWnd;
ui::HBox * gBarBox=nullptr;
ui::Label *CMainFrameUI::m_pLbHConver = nullptr;
ui::Label *CMainFrameUI::m_pLbMConver = nullptr;
ui::Label *CMainFrameUI::m_pLbSConver = nullptr;

ui::Label * CMainFrameUI::m_pLbH = nullptr;
ui::Label * CMainFrameUI::m_pLbM = nullptr;
ui::Label * CMainFrameUI::m_pLbS = nullptr;
DWORD CMainFrameUI::m_dwStartTime = 0;
DWORD CMainFrameUI::m_dwStartTimeConverse = 0;

wstring Get_Isp_logo(int nIspType);

CMainFrameUI::CMainFrameUI()
{
	LOGIC_CENTER()->SetCallBack(this);
	nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {

		LOGIC_CENTER()->LazyInit();
	}));
	AddMessageFilter(this);
}


CMainFrameUI::~CMainFrameUI()
{
}

std::wstring CMainFrameUI::GetSkinFolder()
{
	return L"leigod";
}

std::wstring CMainFrameUI::GetSkinFile()
{
	return L"one/wb_main.xml";
}

std::wstring CMainFrameUI::GetWindowClassName() const
{
	return kClassName;
}

LRESULT CMainFrameUI::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	//bHandled = true;

	//return 0;
	STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
	if (ConfigSet.nExitFix)
	{
		if (ConfigSet.nExitMode)
		{	
			PostQuitMessage(0L);
			return __super::OnClose(uMsg, wParam, lParam, bHandled);
		}
		else
			::ShowWindow(m_hWnd, SW_HIDE);
		return true;
	}
	CreateChildModel();

	return 0;
}

void CMainFrameUI::ReleaseOnExit()
{
	::ShowWindow(m_hWnd, SW_HIDE);
	m_trayTool.Delete();
}

ui::VBox * CMainFrameUI::buildSubView(const wchar_t * xmlName, const wchar_t *subcontrolname)
{
	wstring s = L"buildSubView     ";
	s.append(xmlName).append(L"    ").append(subcontrolname);
	OutputDebugString(s.c_str());
	auto callback = nbase::Bind(&WindowImplBase::CreateControl, this, std::placeholders::_1);
	ui::VBox * item = NULL;
	std::wstring  areacontrol = L"area_btnbox";
	std::wstring  servercontrol = L"server_btnbox";
	std::wstring  operatorcontrol = L"operator_btnbox";
	std::wstring  searchcontrol = L"search_list";
	std::wstring  cefcontrol = L"cef_loadbox";

	if (areacontrol.compare(subcontrolname) == 0) {
		OutputDebugString(L"create CAreaVBox");
		item = new nui::CAreaVBox(this);
		m_areaVbox = dynamic_cast<nui::CAreaVBox*>(item);
	} else	if (servercontrol.compare(subcontrolname) == 0) {
		OutputDebugString(L"create CServerVBox");
		item = new nui::CServerVBox(this);
		m_serverVbox = dynamic_cast<nui::CServerVBox*>(item);
	}
	else	if (operatorcontrol.compare(subcontrolname) == 0) {
		OutputDebugString(L"create COperVBox");
		item = new nui::COperVBox(this);
		m_operVbox = dynamic_cast<nui::COperVBox*>(item);
	}
	else	if (searchcontrol.compare(subcontrolname) == 0) {
		OutputDebugString(L"create CSearchVBox");
		item = new nui::CSearchVBox(this);
		m_searchVBox = dynamic_cast<nui::CSearchVBox*>(item);
	}
	else	if (cefcontrol.compare(subcontrolname) == 0) {
		OutputDebugString(L"create CCefVBox");
		item = new   nui::CCefVBOX(this);
		m_cefVBox = dynamic_cast<nui::CCefVBOX*>(item);
	}
	else {
		 item = new ui::VBox;
	}
	//ui::VBox * item = new ui::VBox;
	ui::GlobalManager::FillBoxWithCache(item, xmlName, callback);
	return item;
}

ui::ListContainerElement * CMainFrameUI::buildSubListContainerElement(const wchar_t * xmlName)
{
	//wstring s = L"buildSubListContainerElement     ";
	//s.append(xmlName);
	//OutputDebugString(s.c_str());

	ui::ListContainerElement * item = new ui::ListContainerElement;
	ui::GlobalManager::FillBoxWithCache(item, xmlName);
	return item;
}

void CMainFrameUI::InitWindow()
{
	StringVector vctV;
	ui::Label * pLbExeV = dynamic_cast<ui::Label*>(FindControl(L"exe_version"));
	if (pLbExeV)
	{
		if(stralgo::StringHelper::SplitString(_T(FILE_VER), vctV,L".")>0)
			pLbExeV->SetText(L"V"+vctV[0]+L"."+vctV[1]+L"."+vctV[2]);
	}
	// trayiocn
	{
		SetTaskbarTitle(L"雷神加速器（网吧版）");
		SetIcon(IDI_SMALL);
		m_trayTool.SetToolTip(L"雷神加速器（网吧版）");
		m_trayTool.SetIcon(gInstance, IDI_LAYOUTS);
		m_trayTool.SetNotifyWnd(GetHWND(), WUM_TARYICON_MSG);
		m_trayTool.Show();

#ifndef _DEBUG
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir() + L"updater.exe";
		wstring wsCmd = stralgo::StringHelper::FormatW(L"-h%u", (uint32_t)GetHWND());
		wsCmd += L" -xleigod.exe";
		ShellExecute(NULL, L"open", wsExepath.c_str(), wsCmd.c_str(), NULL, SW_SHOW);
		
#endif

	}
	CreateXmlObject(); 
 

	CreateBtnBindClick();
	{
		m_pCefEnCharge = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_encharge"));
		if (m_pCefEnCharge)
		{
			
			m_pCefEnCharge->AttachUrlChange(nbase::Bind(&CMainFrameUI::OnEnchageUrlChange, this, std::placeholders::_1));
			m_pCefEnCharge->AttachBeforeNavigate(nbase::Bind(&CMainFrameUI::OnBeforeNavigate, this, std::placeholders::_1, std::placeholders::_2));
			m_pCefEnCharge->AttachLoadEnd(nbase::Bind(&CMainFrameUI::OnLoadEnd, this, std::placeholders::_1));
		}
		m_pCefBind = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_bind"));
		if (m_pCefBind)
		{
			m_pCefBind->AttachBeforeNavigate(nbase::Bind(&CMainFrameUI::OnBeforeNavigateBind, this, std::placeholders::_1, std::placeholders::_2));
		}
		m_pCefReward = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_reward"));
		if (m_pCefReward)
		{			
			m_pCefReward->AttachBeforeNavigate(nbase::Bind(&CMainFrameUI::OnBeforeNavigateReward, this, std::placeholders::_1, std::placeholders::_2));
		}
		
	}

	STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
	ui::ButtonBox * pBtnAccMode = dynamic_cast<ui::ButtonBox *>(FindControl(L"auto_btnbox"));
	if (pBtnAccMode)
	{
		switch (ConfigSet.nAccMode)
		{
		case 0:
			pBtnAccMode->SetText(L"自动");
			break;
		case 1:
			pBtnAccMode->SetText(L"LSP模式");
			break;
		case 2:
			pBtnAccMode->SetText(L"HOOK模式");
			break;
		case 3:
			pBtnAccMode->SetText(L"Driver模式");
			break;
		default:
			break;
		}

	}

}

bool CMainFrameUI::XmlToXmlObjVct(std::wstring parentBox, std::wstring xmlName, bool isShow)
{
	StringVector vctXml;
	StringVector vctName;
	int vctSize = 0;
	vctSize=stralgo::StringHelper::SplitString(xmlName,vctXml,L"/");
	vctSize=stralgo::StringHelper::SplitString(vctXml[vctSize-1], vctName, L".");

	ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(parentBox));
	if (pBox)
	{
		m_mapXmlParentBox.insert(Insert_Pair(parentBox, pBox));
		ui::VBox * pViewXml = buildSubView(xmlName.c_str(), vctName[0].c_str());
		 
 
		if (pViewXml)
		{
			pViewXml->SetVisible(isShow);
			pBox->Add(pViewXml);
			OutputDebugString(wstring(L"XmlToXmlObjVct         " + parentBox + L"   " + xmlName + L"   " + vctName[0]).c_str());
			m_mapXmlList.insert(Insert_Pair(vctName[0], pViewXml));

			return true;
		}
	}
	return false;
}


bool CMainFrameUI::CreateXmlObject()
{

	if (!XmlToXmlObjVct(L"main_box", L"leigod/two/myspeed_btn.xml",true))
	{
		return false;
		
	}
	if (!XmlToXmlObjVct(L"main_box",L"leigod/two/commom_game_list.xml",false))
	{
		return false;

	}
	//if (m_version_select == 1)
	{
 		if (!XmlToXmlObjVct(L"pay_and_advertiseing_box", L"leigod/two/pay_and_advertiseing_box_sm.xml", true))
		{
			return false;
		}
	}
	//else
	{
		if (!XmlToXmlObjVct(L"pay_and_advertiseing_box", L"leigod/two/pay_and_advertiseing_box_by.xml", false))
		{
			return false;


		}
	}
	//if (!XmlToXmlObjVct(L"nick_game_list", L"leigod/two/nick_list.xml",true))
	if (!XmlToXmlObjVct(L"nick_window_box", L"leigod/two/search_list.xml", true))
	{
		return false;
	}

	if (!XmlToXmlObjVct(L"cef_container_box", L"leigod/two/cef_loadbox.xml", true))
	{
		return false;
	}

	if (!XmlToXmlObjVct(L"push_window_box", L"leigod/two/server_btnbox.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"push_window_box", L"leigod/two/area_btnbox.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"push_window_box", L"leigod/two/operator_btnbox.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"monitor_box", L"leigod/two/monitor_box.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"net_and_log_box", L"leigod/two/monitor_net.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"net_and_log_box", L"leigod/two/monitor_log.xml",false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"set_box", L"leigod/two/set_box.xml", false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"accrate_and_quit_box", L"leigod/two/set_accrate_box.xml", false))
	{
		return false;
	}
	if (!XmlToXmlObjVct(L"accrate_and_quit_box", L"leigod/two/set_quit_box.xml", false))
	{
		return false;
	}

	auto parentXml = m_mapXmlParentBox.find(L"push_window_box");
	if (parentXml->second)
		m_pPushBox = parentXml->second;

	m_pMenuBox= dynamic_cast<ui::VBox*>(FindControl(L"menu_window_box"));
	return true;
}

bool CMainFrameUI::ShowSelected(int oldSelect,int newSelect)
{
	static std::wstring arrTabButton[] = { L"all_btn", L"platform_btn",L"Steam_btn",
											L"Origin_btn",L"Uplay_btn",L"Epic_btn",L"zw_btn",L"gf_btn" };

	ui::ButtonBox *pBtnBox = dynamic_cast<ui::ButtonBox*>(FindControl(L"myspeed_btn"));
	if (oldSelect != newSelect)
	{
		if ((oldSelect >= 0))
		{
			ui::Button *pBtnold = dynamic_cast<ui::Button*>(FindControl(arrTabButton[oldSelect]));
			if (pBtnold)
				pBtnold->SetStateTextColor(ui::kControlStateNormal, L"tab_btn_color");
			else
				return false;
		}
		else
		{
			if (pBtnBox)
			{
				pBtnBox->SetStateTextColor(ui::kControlStateNormal, L"tab_btn_color");
				pBtnBox->SetStateImage(ui::kControlStateNormal, L"file='btn/myspeed_btn.png' dest='2,3,18,19' source='0,0,16,16'");
			}
			else
			{

				return false;
			}
		}
	}

	if (newSelect >= 0)
	{
		ui::Button *pBtnNew = dynamic_cast<ui::Button*>(FindControl(arrTabButton[newSelect]));
		if (pBtnNew)
			pBtnNew->SetStateTextColor(ui::kControlStateNormal, L"white");
		else
			return false;
	}
	else
	{
		pBtnBox->SetStateTextColor(ui::kControlStateNormal, L"yellow");
		pBtnBox->SetStateImage(ui::kControlStateNormal, L"file='btn/myspeed_btn.png' dest='2,3,18,19' source='16,0,32,16'");
	}
	m_nOldGameSelected = newSelect;
	return true;
}

bool CMainFrameUI::CleanGameElement()
{
	ui::ListBox * pAllGameListBox = dynamic_cast<ui::ListBox*>(FindControl(L"all_game_listbox"));
	if (pAllGameListBox)
		pAllGameListBox->RemoveAll();
	else
		return false;

	return true;
}

bool CMainFrameUI::OnBtnTabClick(ui::EventArgs* args)
{
	static std::wstring arrTabButton[] = { L"myspeed_btn", L"all_btn", L"platform_btn",L"Steam_btn",
		L"Origin_btn",L"Uplay_btn",L"Epic_btn",L"zw_btn",L"gf_btn" };
	std::wstring wsName = args->pSender->GetName();
	
	ui::VBox * pMainBk = dynamic_cast<ui::VBox *>(FindControl(L"main_bk"));
	for (const auto&p : m_mapXmlList)
	{
		if(p.second)
			p.second->SetVisible(false);
	}

	if (wsName == arrTabButton[0])
	{
		
		if (pMainBk)
			pMainBk->SetBkImage(L"bk_photo/bk_main.png");
	
		auto pXml = m_mapXmlList.find(L"myspeed_btn");
		if (pXml->second)
			pXml->second->SetVisible(true);

		if (m_SmSelect)
		{
			auto pSm = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
			if (pSm->second)
				pSm->second->SetVisible(true);
		}
		else
		{
			auto pSm = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
			if (pSm->second)
				pSm->second->SetVisible(true);
		}

		ShowSelected(m_nOldGameSelected,-1);
		return true;
		
	}

	auto pXml = m_mapXmlList.find(L"commom_game_list");
	if (pXml->second)
		pXml->second->SetVisible(true);
	else
		return false;

	if (pMainBk)
	{
		pMainBk->SetBkImage(L"bk_photo/bk_game_list.png");
		
		pMainBk->SetBorderRound(ui::CSize(10,10));
	}
	
	if (wsName == arrTabButton[1])
	{
		for (const GFGameLIST &p1 : vctGame)
		{
			if (p1.Element)
				p1.Element->SetVisible(true);
			else
				false;
		}
		
		ShowSelected(m_nOldGameSelected,0);
		m_nOldSelected = 0;
		return true;
	}

	

	STGamePlat &GamePlat = LOGIC_CENTER()->GetGamePlat();
	if (wsName == arrTabButton[2])
	{
		ShowSelected(m_nOldGameSelected, 1);
		m_nOldSelected = 1;
	
		ShowGameBox(L"平台");
		return true;
	}
	if (wsName == arrTabButton[3])
	{
		ShowSelected(m_nOldGameSelected, 2);
		m_nOldSelected = 2;
		
		ShowGameBox(L"Steam");
		
		return true;
	}
	if (wsName == arrTabButton[4])
	{
		ShowSelected(m_nOldGameSelected,3);
		m_nOldSelected = 3;
		
		ShowGameBox(L"Origin");
		
		return true;

	}
	if (wsName == arrTabButton[5])
	{
		ShowSelected(m_nOldGameSelected,4);
		m_nOldSelected = 4;
		
		ShowGameBox(L"Uplay");
		
		return true;
	}
	if (wsName == arrTabButton[6])
	{
		ShowSelected(m_nOldGameSelected, 5);
		m_nOldSelected = 5;
		
		ShowGameBox(L"Epic");
		
		return true;
	}
	if (wsName == arrTabButton[7])
	{
		ShowSelected(m_nOldGameSelected,6);
		m_nOldSelected = 6;

		ShowGameBox(L"战网");

		return true;
	}
	if (wsName == arrTabButton[8])
	{
		ShowSelected(m_nOldGameSelected,7);
		m_nOldSelected = 7;
		
		ShowGameBox(L"国服");
		
		return true;
	}
	
	return true;
}

bool CMainFrameUI::OnGameListClick(ui::EventArgs* args)
{
	int index = 0;
	int spiltIndex = 0;
	int nSelect = 0;
	WCHAR  *stopStr;
	STGame STGameGot;
	StringVector nameVct;
	StringVector idVct;
	std::wstring wstrGameId,gameServerName, gameName;
	std::wstring BigPhotoPath; 
	
	if (m_nIsArccrate > 0)
	{
		/*uiTipMessageBox *tipChildWnd = new uiTipMessageBox(m_hWnd);
		nSelect = tipChildWnd->DoModal();
		delete tipChildWnd;*/
		nSelect=CreateTipMsgBox();
	}
	if (nSelect == 1|| m_nIsArccrate ==0)
	{
		wstring boxName = args->pSender->GetName();
		if (boxName.empty())
			return false;
		if (nSelect==1)
		{
			SetStopAccrate();
		}
		spiltIndex = stralgo::StringHelper::SplitString(boxName, idVct, L"_");
		if (spiltIndex > 0)
			ShowSignalGame(std::wcstol(idVct[4].c_str(), &stopStr, 0));
		//STGameGot = GetGameClickObj(std::wcstol(idVct[2].c_str(), &stopStr, 0), std::wcstol(idVct[4].c_str(), &stopStr, 0));
		else
			return false;
	}
	
	//if (STGameGot.wsGameTitle.empty())
	//	return false;
	//
	//ui::Button * pCruentPhotoBtn = dynamic_cast<ui::Button*>(FindControl(L"cruuent_photo_btn"));
	//if(pCruentPhotoBtn)
	//	pCruentPhotoBtn->SetStateImage(ui::kControlStateNormal, STGameGot.wsBgPath);

	//ui::Label * pCruentGameName = dynamic_cast<ui::Label*>(FindControl(L"current_game_name_label"));
	//if(pCruentGameName)
	//{
	//	pCruentGameName->SetText(STGameGot.wsGameTitle);
	//}
	//else
	//{
	//	return false;
	//}

	//ui::VBox * pMainBk = dynamic_cast<ui::VBox *>(FindControl(L"main_bk"));
	//if (pMainBk)
	//	pMainBk->SetBkImage(L"bk_photo/bk_main.png");

	//for (const auto&p : m_mapXmlList)
	//{
	//	//std::wstring wstrShowSmOrBy = L"pay_and_advertiseing_box_sm";
	//	if (p.first == L"myspeed_btn")
	//	{
	//		p.second->SetVisible(true);
	//		continue;
	//	}
	//	/*if (!m_SmSelect)
	//	{
	//		wstrShowSmOrBy= L"pay_and_advertiseing_box_by";
	//	}*/
	//	if (p.first == L"pay_and_advertiseing_box_sm")
	//	{
	//		p.second->SetVisible(true);
	//		continue;
	//	}
	//	
	//	p.second->SetVisible(false);

	//}

	//if (!ShowSelected(m_nOldSelected, -1))
	//{
	//	return false;
	//}
	//
	//ui::VBox * pTipLabel = dynamic_cast<ui::VBox *>(FindControl(L"nick_window_box"));
	//if(pTipLabel)
	//	pTipLabel->SetVisible(false);

	////---------------------- 业务逻辑部分 ------------------
	//{
	//	int nGameID(0);
	//	vector<wstring> vctSplit;
	//	stralgo::StringHelper::SplitString(boxName, L'_', vctSplit);
	//	for (auto itr = vctSplit.rbegin(); itr != vctSplit.rend(); itr++)
	//	{
	//		nGameID = _wtoi(itr->c_str());
	//		if (nGameID)
	//		{
	//			break;
	//		}
	//	}

	//	if (nGameID)
	//	{
	//		STGame stGame;
	//		if (LOGIC_CENTER()->FindGameOnID(nGameID, stGame))
	//		{
	//			if (stGame.vctAreas.size())
	//			{
	//				OnGameSelect(nGameID);
	//				m_nGameID = nGameID;
	//				m_nAreaID = 0;
	//				
	//				if (LOGIC_CENTER()->IsLogined())
	//				{
	//					m_bWaitLogin = false;
	//					OnAreaSelect(stGame.vctAreas[0].nID);
	//					//nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
	//					//	LOGIC_CENTER()->GetLineGroup(nGameID, m_nAreaID);
	//
	//					//}));
	//				}
	//				else
	//				{
	//					m_bWaitLogin = true;
	//				}

	//				
	//				
	//			}
	//
	//		}
	//	
	//	}
	//}
	return true;
}

bool CMainFrameUI::OnBtnSelectNodeClick(ui::EventArgs* args)
{
	static std::wstring arrTabButton[] = {L"server_btnbox",L"area_btnbox",L"operator_btnbox"};
	std::wstring wsName = args->pSender->GetName();
	wstring s = L"OnBtnSelectNodeClick    " + wsName;
	OutputDebugString(s.c_str());
	if (m_pPushBox) {
		m_pPushBox->SetVisible(true);
		OutputDebugString(L"OnBtnSelectNodeClick ----- 0");
	}
	else
		return false;

	for (const auto&p : m_mapXmlList)
	{
		if (p.first == L"myspeed_btn")
			continue;
		if (p.first == L"pay_and_advertiseing_box_sm" || p.first == L"pay_and_advertiseing_box_by")
			continue;
		OutputDebugString(L"OnBtnSelectNodeClick ----- 1");
		p.second->SetVisible(false);
	}
	
	auto pServer = m_mapXmlList.find(wsName);
	if (pServer->second) {
		OutputDebugString(L"OnBtnSelectNodeClick ----- 2");
		pServer->second->SetVisible(true);
	}
	
	return true;
}

bool CMainFrameUI::OnBtnPushWindowCloseClick(ui::EventArgs* args)
{
	
	std::wstring wsName = args->pSender->GetName();
	static std::wstring arrTabButton[] = { L"close_set_btn",L"cancel_set_btn",L"close_Tip_window_btn",L"operiton_successful_ok_btn",L"close_wx_window_btn"};
	if (wsName==arrTabButton[0]|| wsName == arrTabButton[1])
	{
		ShowOrHideMenuSetWnd(false); 
		return true;
	}
	if (wsName == arrTabButton[2]|| wsName == arrTabButton[3])
	{
		ui::VBox * pSuccessfulTipBox = dynamic_cast<ui::VBox*>(FindControl(L"operiton_successful_tip_label"));
		if (pSuccessfulTipBox)
			pSuccessfulTipBox->SetVisible(false);
		return true;
	}
	if (wsName == arrTabButton[4])
	{
		ui::VBox * pWxBox = dynamic_cast<ui::VBox*>(FindControl(L"wx_bind_wnd_box"));
		if (pWxBox)
			pWxBox->SetVisible(false);
		return true;
	}
	if (m_pPushBox)
		m_pPushBox->SetVisible(false);
	else
		return false;
	
	return true;
}

bool CMainFrameUI::OnBtnChangeCurrentPhotoClick(ui::EventArgs* args)
{
	for (const auto&p : m_mapXmlList)
	{
		if(p.second)
			p.second->SetVisible(false);
	}

	ui::VBox * pMainBk = dynamic_cast<ui::VBox *>(FindControl(L"main_bk"));
	if (pMainBk)
		pMainBk->SetBkImage(L"bk_photo/bk_game_list.png");

	if (-1 == m_nOldSelected)
	{
		if (!ShowSelected(m_nOldSelected,0))
			return false;

		for (const GFGameLIST &p1 : vctGame)
		{
			if (p1.Element)
				p1.Element->SetVisible(true);
			else
				false;
		}
	}
	else
	{
		if (!ShowSelected(-1, m_nOldSelected))
			return false;
	}

	auto pXml = m_mapXmlList.find(L"commom_game_list");
	if (pXml->second)
	{
		pXml->second->SetVisible(true);
		return false;
	}

	return true;
}


bool CMainFrameUI::OnBtnTipEvent(ui::EventArgs* args)
{
	ui::Label * pTipLabel = dynamic_cast<ui::Label *>(FindControl(L"tip_photo"));
	if(pTipLabel)
		pTipLabel->SetVisible(true);
	
	return true;
}

bool CMainFrameUI::OnBtnLeaveEvent(ui::EventArgs* args)
{
	ui::Label * pTipLabel = dynamic_cast<ui::Label *>(FindControl(L"tip_photo"));
	if(pTipLabel)
		pTipLabel->SetVisible(false);
	return true;
}


bool CMainFrameUI::OnNickTextChangeEvent(ui::EventArgs* args)
{
	ui::RichEdit * pEditNick = dynamic_cast<ui::RichEdit *>(FindControl(L"nick"));
	ui::VBox * pTipLabel = dynamic_cast<ui::VBox *>(FindControl(L"nick_window_box"));
	if (pEditNick&&pEditNick)
	{
		vector<STGame> vctGotGame;
		if (pEditNick->GetText().empty())
		{
			pTipLabel->SetVisible(false);
			return true;
		}
		LOGIC_CENTER()->FindGameOnKeyWord(vctGotGame, pEditNick->GetText());
		if (vctGotGame.size())
		{
			
			pTipLabel->SetVisible(true);
			//auto pXml= m_mapXmlList.find(L"nick_list");
			if (m_searchVBox) {
				m_searchVBox->SetVisible(true);
				m_searchVBox->CreateSearchElementList(vctGotGame);
			}
			//auto pXml = m_mapXmlList.find(L"search_list");
			//pXml->second->SetVisible(true);
			//CreateNickElementList(vctGotGame);

		}
	}
	return true;
}
bool CMainFrameUI::OnNickKillFocusEvent(ui::EventArgs* args)
{
	OutputDebugString(L"OnNickKillFocusEvent--->");
	return true;  //想要响应 clear_search 点击事件， 这个就要禁掉。 
	((ui::RichEdit *)args->pSender)->SetPromptText(L"搜索游戏");
	ui::VBox * pTipLabel = dynamic_cast<ui::VBox *>(FindControl(L"nick_window_box"));
	if(pTipLabel)
		pTipLabel->SetVisible(false);
	return true;
}

bool CMainFrameUI::OnBtnAccrateStart(ui::EventArgs* args)
{

	m_nControlArccrate = 1;
#ifndef _DEBUG
	if (m_nTimeCount <= 0)
	{
		CreateTipMsgBox(L"您还未购买时长，请先扫码购买时长");
		return true;
	}
#endif	

	if (!m_bInited)
	{
		CreateTipMsgBox(L"更在更新资源，请稍候重试。");
		return true;
	}
	
	//--------------- 调用流程逻辑 开始加速---------------------------
	if (m_nGameID && m_nSvrID)
	{
		if (m_nIsArccrate)
		{
			return true;
		}
		else
		{
			m_nIsArccrate = 1;
		}
		args->pSender->SetVisible(false);
		ui::Label * pFill = dynamic_cast<ui::Label *>(FindControl(L"accrate_fill"));
		ui::Button * pAccrate = dynamic_cast<ui::Button *>(FindControl(L"accrate_btn"));
		pFill->SetVisible(true);
		pAccrate->SetVisible(true);
		nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&CMainFrameUI::OnBtnAccrateProgress, this, pFill), nbase::TimeDelta::FromMilliseconds(600));
		nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
			LOGIC_CENTER()->StartAcc(m_nGameID, m_nSvrID);
		}));
	}
	else
	{
		CreateTipMsgBox(L"资源加载中，请稍候..");
	}

	
	return true;
}

void CMainFrameUI::SetStopAccrate()
{
	
	nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
		ui::ButtonBox * pBtnStart = dynamic_cast<ui::ButtonBox *>(FindControl(L"start_accrate_btn"));
		ui::ButtonBox * pBtnStop = dynamic_cast<ui::ButtonBox *>(FindControl(L"stop_accrate_btn"));
		pBtnStop->SetVisible(false);
		pBtnStart->SetVisible(true);	
		KillTimer(m_hWnd,1);
		SetArccrateNumbColor(0, 0, 0, 0, L"numb_color");
		LOGIC_CENTER()->StopAcc();
		m_nIsArccrate = 0;
	}));
	
	
}

bool CMainFrameUI::OnBtnAccrateStop(ui::EventArgs* args)
{
	
	SetStopAccrate();
	return true;
}
void CMainFrameUI::OnBtnAccrateProgress(ui::Control* fill)
{

	static int FillLeft = 0;
	static int FillTop = 0;
	static int FillRight = 0;
	static int ProcessNum = 0;
	static int FillBottom = 0; 
	static int RunTime = 40;
	static ui::Button * pAccrate = dynamic_cast<ui::Button *>(FindControl(L"accrate_btn"));
	static ui::Label * pFill = dynamic_cast<ui::Label *>(FindControl(L"accrate_fill"));
	ui::ButtonBox * pBtnStop = dynamic_cast<ui::ButtonBox *>(FindControl(L"stop_accrate_btn"));
	ui::ButtonBox * pBtnStart = dynamic_cast<ui::ButtonBox *>(FindControl(L"start_accrate_btn"));
	if (m_nControlArccrate == 1)
	{
		if (ProcessNum <= 10)
		{
			FillRight += 2;
			ProcessNum += 0.58;
		}
		if (ProcessNum<=72)
		{
			FillRight += 1;
			ProcessNum += 1;
			
		}
		if (ProcessNum>72)
		{
			OverCount++;
		}

	}
	if (m_nControlArccrate == 2)
	{
		FillRight += 1;
		ProcessNum += 1;
		RunTime = 10;
	}
	if (m_nControlArccrate == 3 || OverCount==50)
	{
		pBtnStart->SetVisible(true);
		pBtnStop->SetVisible(false);
		pAccrate->SetVisible(false);
		pFill->SetVisible(false);
		FillRight = 0;
		ProcessNum = 0;
		m_nIsArccrate = 0;
		RunTime = 50;
		OverCount = 0;
		pFill->SetBkImage(L"file='label/accrate_fill_photo.png' dest='0,0,0,0' source='0,0,0,0'");
		pAccrate->SetText(L"正在加速0%");
		CreateTipMsgBox(L"加速失败。"); //加速超时,请稍后重新加速
		return;
	}
	
	std::wstring wstrImage = L"file='label/accrate_fill_photo.png' ";
	std::wstring imageDest = L"dest ='0,0," + std::to_wstring(FillRight) + L",40' ";
	std::wstring imageSource = L"source ='0,0," + std::to_wstring(FillRight) + L",40'";

	std::wstring processNumb = L"正在加速" +std::to_wstring(ProcessNum) + L"%";
	wstrImage += imageDest + imageSource;
	pFill->SetBkImage(wstrImage);

	pAccrate->SetText(processNumb);

	if (ProcessNum >= 100)
	{
		pBtnStop->SetVisible(true);
		pAccrate->SetVisible(false);
		pFill->SetVisible(false);
		FillRight = 0;
		ProcessNum = 0;
		RunTime = 50;
		m_nControlArccrate = 1;
		pFill->SetBkImage(L"file='label/accrate_fill_photo.png' dest='0,0,0,0' source='0,0,0,0'");
		pAccrate->SetText(L"正在加速0%");
		return;
	}

	nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&CMainFrameUI::OnBtnAccrateProgress, this, pFill), nbase::TimeDelta::FromMilliseconds(RunTime));
}

bool CMainFrameUI::OnBtnCloseMainWindows(ui::EventArgs* args)
{
	STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
	if (ConfigSet.nExitFix)
	{
		if (ConfigSet.nExitMode)
			PostQuitMessage(0L);
		else
			::ShowWindow(m_hWnd, SW_HIDE);
		return true;
	}
	CreateChildModel();
	return true;
}

bool CMainFrameUI::ShowORHideMonitorWnd(bool isShow)
{
	auto pParentMonitor = m_mapXmlParentBox.find(L"monitor_box");
	if(pParentMonitor->second)
		pParentMonitor->second->SetVisible(isShow);

	auto pXmlMonitor = m_mapXmlList.find(L"monitor_box");
	if(pXmlMonitor->second)
		pXmlMonitor->second->SetVisible(isShow);

	auto pXmlNetAndlog = m_mapXmlParentBox.find(L"net_and_log_box");
	if(pXmlNetAndlog->second)
		pXmlNetAndlog->second->SetVisible(isShow);

	auto pXmlNet = m_mapXmlList.find(L"monitor_net");
	if(pXmlNet->second)
		pXmlNet->second->SetVisible(isShow);

	if (!isShow)
	{
		auto pXmlLog = m_mapXmlList.find(L"monitor_log");
		pXmlNet->second->SetVisible(false);
	}
	return true;
}

bool CMainFrameUI::ShowMonitorNetORLogWnd(bool isShow)
{
	IsSelectedMonitorBtn(isShow);
	auto pXmlNet = m_mapXmlList.find(L"monitor_net");
	if(pXmlNet->second)
		pXmlNet->second->SetVisible(isShow>0 ? 1:0);

	auto pXmlLog = m_mapXmlList.find(L"monitor_log");
	if(pXmlLog->second)
		pXmlLog->second->SetVisible(isShow>0? 0:1);

	return true;
}

void CALLBACK ShowBar1(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	
		int i = 0;
		if (gBarBox)
			gBarBox->RemoveAll();

		for (uint32_t & p : gListBarControl)
		{
			ui::Control * pBarCtrl = new ui::Control();
			ui::Control * pCtrl = new ui::Control();
			if (gBarBox)
			{
				gBarBox->Add(pBarCtrl);
				gBarBox->Add(pCtrl);
			}
			pCtrl->SetFixedWidth(5);
			pBarCtrl->SetFixedWidth(10);
			pBarCtrl->SetFixedHeight(p*0.15);
			pBarCtrl->SetBkColor(L"blue");
			pBarCtrl->SetVerAlignType(ui::VerAlignType::kVerAlignBottom);

			i++;
			if (i == 30)
				break;
		}
}

void CALLBACK ShowBar5(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{

	int i = 0;
	if (gBarBox)
		gBarBox->RemoveAll();

	for (uint32_t & p : gListBarControl)
	{
		ui::Control * pBarCtrl = new ui::Control();
		ui::Control * pCtrl = new ui::Control();
		if (gBarBox)
		{
			gBarBox->Add(pBarCtrl);
			gBarBox->Add(pCtrl);
		}
		pCtrl->SetFixedWidth(2);
		pBarCtrl->SetFixedWidth(5);
		pBarCtrl->SetFixedHeight(p*0.15);
		pBarCtrl->SetBkColor(L"blue");
		pBarCtrl->SetVerAlignType(ui::VerAlignType::kVerAlignBottom);

		i++;
		if (i == 200)
			break;
	}
}


void CALLBACK ShowBar10(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{

	int i = 0;
	if (gBarBox)
		gBarBox->RemoveAll();

	for (uint32_t & p : gListBarControl)
	{
		ui::Control * pBarCtrl = new ui::Control();
		ui::Control * pCtrl = new ui::Control();
		if (gBarBox)
		{
			gBarBox->Add(pBarCtrl);
			gBarBox->Add(pCtrl);
		}
		pCtrl->SetFixedWidth(2);
		pBarCtrl->SetFixedWidth(3);
		pBarCtrl->SetFixedHeight(p*0.15);
		pBarCtrl->SetBkColor(L"blue");
		pBarCtrl->SetVerAlignType(ui::VerAlignType::kVerAlignBottom);

		i++;
		if (i == 500)
			break;
	}
}

bool CMainFrameUI::MonitorTimeBtnSelected(std::wstring btnName)
{
	static std::wstring arrTimeButton[] = {  L"show_time_1_btn",L"show_time_5_btn",L"show_time_10_btn"};

	if (btnName == L"monitor_btnbox")
		return true;
	for (std::wstring &BtnName : arrTimeButton)
	{
		ui::Button * pBtn = dynamic_cast<ui::Button*>(FindControl(BtnName));
		if (pBtn)
		{
			if(btnName== BtnName)
				pBtn->SetStateTextColor(ui::ControlStateType::kControlStateNormal, L"node_select_bk_color");
			else
				pBtn->SetStateTextColor(ui::ControlStateType::kControlStateNormal, L"monitor_time_btn_color");
		}
	}
	return true;
}
bool CMainFrameUI::OnBtnMonitorWndClick(ui::EventArgs* args)
{
	static std::wstring arrButton[] = { L"monitor_btnbox",L"net_monitor_btn",L"log_btn",L"show_time_1_btn",L"show_time_5_btn",L"show_time_10_btn"};
	wstring boxName = args->pSender->GetName();

	
	gBarBox = dynamic_cast<ui::HBox *>(FindControl(L"bar_box"));
	if (boxName == arrButton[0])
	{
		if (!ShowORHideMonitorWnd(true))
		{
			return false;
		}

		{
			ui::Label * pLbCurGame = dynamic_cast<ui::Label *>(FindControl(L"monitor_game_name"));
			if (pLbCurGame)
			{
				STGame stGame;
				if (LOGIC_CENTER()->FindGameOnID(m_nGameID, stGame))
				{
					pLbCurGame->SetText(stGame.wsGameTitle);
				}
			}

			ui::ButtonBox * pBtnAreaBox = dynamic_cast<ui::ButtonBox*>(FindControl(L"operator_btnbox"));
			if (pBtnAreaBox)
			{
				ui::Label * pCurNode = dynamic_cast<ui::Label*>(FindControl(L"monitor_node_name"));
				if (pCurNode)
				{
					pCurNode->SetText(pBtnAreaBox->GetText());
				}
			}
		}
			
		if (m_nIsArccrate)
		{
			nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
				std::wstring GameName;
				ui::Label * pCrruentGameName = dynamic_cast<ui::Label*>(FindControl(L"current_game_name_label"));
				ui::ButtonBox * pBtnCrruentNodeName = dynamic_cast<ui::ButtonBox*>(FindControl(L"operator_btnbox"));
				ui::Label * pLbGameName = dynamic_cast<ui::Label*>(FindControl(L"monitor_game_name"));
				ui::Label * pLbNodeName = dynamic_cast<ui::Label*>(FindControl(L"monitor_node_name"));
				if (pCrruentGameName)
					GameName=pCrruentGameName->GetText();
				if (pLbGameName)
					pLbGameName->SetText(GameName);
				if (pBtnCrruentNodeName)
					GameName=pBtnCrruentNodeName->GetText();
				if (pLbNodeName)
					pLbNodeName->SetText(GameName);

			}));
		}
		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]()
		{
			SetTimer(m_hWnd, 10, 3000, ShowBar1);
		}));
	}
	if (boxName == arrButton[1])
	{
		ShowMonitorNetORLogWnd(true);
	}
	if (boxName == arrButton[2])
	{
		ShowMonitorNetORLogWnd(false);
	}
	if (boxName == arrButton[3])
	{
		MonitorTimeBtnSelected(boxName);
		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

			SetTimer(m_hWnd,10,3000,ShowBar1);
			KillTimer(gMainWnd->GetHWND(), 11);
			KillTimer(gMainWnd->GetHWND(), 12);
		}));
	}
	if (boxName == arrButton[4])
	{
		MonitorTimeBtnSelected(boxName);
		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

			SetTimer(m_hWnd,11, 3000, ShowBar5);
			KillTimer(gMainWnd->GetHWND(), 10);
			KillTimer(gMainWnd->GetHWND(), 12);
		}));
	}
	if (boxName == arrButton[5])
	{
		MonitorTimeBtnSelected(boxName);
		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

			SetTimer(m_hWnd,12, 3000, ShowBar10);
			KillTimer(gMainWnd->GetHWND(), 10);
			KillTimer(gMainWnd->GetHWND(), 11);
		}));
	}
	return true;
}

bool CMainFrameUI::OnBtnCloseMonitorWnd(ui::EventArgs* args)
{
	ShowORHideMonitorWnd(false);
	KillTimer(gMainWnd->GetHWND(), 10);
	KillTimer(gMainWnd->GetHWND(), 11);
	KillTimer(gMainWnd->GetHWND(), 12);
	return true;
}
bool CMainFrameUI::IsSelectOption(std::wstring optionName,int type,int value)
{
	ui::Option * pOption = dynamic_cast<ui::Option*>(FindControl(optionName));
	if (type==1)
	{
		if (pOption)
		{
			if (pOption->IsSelected())
			{
				m_nModelSelect = value;
			}
		}
	}
	if (type == 2)
	{
		if (pOption)
		{
			if (pOption->IsSelected())
			{
				m_nTransferSelect = value;
			}
			else
			{
				m_nTransferSelect = 1;
			}
		}
	}
	if (type == 3)
	{
		if (pOption)
		{
			if (pOption->IsSelected())
			{
				m_nExitModl = value;
			}
			else
			{
				m_nExitModl = 1;
			}
		}
	}
	return true;
}

bool CMainFrameUI::GetMenuSet(STGameListConfigration & menuSet)
{
	m_nModelSelect = menuSet.nAccMode;
	m_nTransferSelect = menuSet.nTransPortMode;
	m_nExitModl = menuSet.nExitMode;
	m_nExitFix = menuSet.nExitFix;

	IsSelectOption(L"auto_pat_option",1,0);
	IsSelectOption(L"lsp_pat_option",1,1);
	IsSelectOption(L"hook_pat_option",1,2);
	IsSelectOption(L"driver_pat_option",1,3);

	IsSelectOption(L"topspeed_pat_option",2, 0);
	IsSelectOption(L"min_tory_option",3, 0);

	ui::CheckBox * pCheackBoxWarn = dynamic_cast<ui::CheckBox*>(FindControl(L"quit_warn_chbox"));
	if (pCheackBoxWarn)
	{
		if (pCheackBoxWarn->IsSelected())
			m_nExitFix = 1;
		else
			m_nExitFix = 0;
	}

	menuSet.nAccMode = m_nModelSelect;
	menuSet.nTransPortMode = m_nTransferSelect;
	menuSet.nExitMode = m_nExitModl;
	menuSet.nExitFix = m_nExitFix;

	LOGIC_CENTER()->SaveModeIni();
	return true;
}

bool CMainFrameUI::ShowMenu()
{
	if (m_pMenuBox)
	{
		m_pMenuBox->SetVisible(true);
		m_pMenuBox->SetFocus();
		return true;
	}
	return false;
}

bool CMainFrameUI::ShowOrHideMenuSetWnd(bool isShow)
{
	if (m_pMenuBox)
	{
		m_pMenuBox->SetVisible(false);
		auto pParentSet = m_mapXmlParentBox.find(L"set_box");
		if(pParentSet->second)
			pParentSet->second->SetVisible(isShow);

		auto pXmlSet = m_mapXmlList.find(L"set_box");
		if(pXmlSet->second)
			pXmlSet->second->SetVisible(isShow);

		auto pXmlAccrateAndQuit = m_mapXmlParentBox.find(L"accrate_and_quit_box");
		if(pXmlAccrateAndQuit->second)
			pXmlAccrateAndQuit->second->SetVisible(isShow);

		auto pXmlAccrate = m_mapXmlList.find(L"set_accrate_box");
		if(pXmlAccrate->second)
			pXmlAccrate->second->SetVisible(isShow);
		if (!isShow)
		{
			auto pXmlquit = m_mapXmlList.find(L"set_quit_box");
			pXmlquit->second->SetVisible(false);
		}
		return true;
	}
	return false;
}

bool CMainFrameUI::ShowSetArccrateOrQuitWnd(bool isShow)
{
	
	auto pXmlAccrate = m_mapXmlList.find(L"set_accrate_box");
	if(pXmlAccrate->second)
		pXmlAccrate->second->SetVisible(isShow > 0 ? 1 : 0);
	auto pXmlQuit = m_mapXmlList.find(L"set_quit_box");
	if(pXmlQuit->second)
		pXmlQuit->second->SetVisible(isShow > 0 ? 0 : 1);


	return true;
}


bool CMainFrameUI::ShowMenuRepairNetOrCleanBufWnd(std::wstring wndName)
{
	
	if (m_pMenuBox)
	{
		m_pMenuBox->SetVisible(false);
		ui::VBox * pRepairBox = dynamic_cast<ui::VBox*>(FindControl(L"system_tip_box"));
		if (pRepairBox)
			pRepairBox->SetVisible(true);
		ui::Label * pTip = dynamic_cast<ui::Label*>(FindControl(L"sys_tip_text"));
		if (pTip)
		{
			if(wndName==L"net_btnbox")
				pTip->SetText(L"正在修复网路...");
			if(wndName == L"clean_btnbox")
				pTip->SetText(L"正在清除缓存...");
		}
		nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&CMainFrameUI::OnSuccessTip, this, pRepairBox, wndName), nbase::TimeDelta::FromMilliseconds(3000));
	}
	return true;
}

bool CMainFrameUI::ShowArccrateAndQiutSet(int nMode)
{
	std::wstring optionName[] = { L"auto_pat_option",L"lsp_pat_option",L"hook_pat_option",L"driver_pat_option",
		L"topspeed_pat_option",L"com_pat_option",L"min_tory_option",L"quit_app_option" };
	ui::Option * pOption = dynamic_cast<ui::Option*>(FindControl(optionName[nMode]));
	if (pOption)
		pOption->Selected(true);

	return true;
}

void CMainFrameUI::IsSelectedArccrateBtn(bool isShow)
{
	ui::Button * pBtnAcr = dynamic_cast<ui::Button *>(FindControl(L"accrate_set_btn"));
	if (pBtnAcr)
		pBtnAcr->SetStateTextColor(ui::ControlStateType::kControlStateNormal,isShow>0? SELECT:NO_SELECT);
	ui::Button * pBtnQuit = dynamic_cast<ui::Button *>(FindControl(L"quit_set_btn"));
	if (pBtnQuit)
		pBtnQuit->SetStateTextColor(ui::ControlStateType::kControlStateNormal, isShow>0? NO_SELECT: SELECT);
}

void  CMainFrameUI::IsSelectedMonitorBtn(bool isShow)
{
	ui::Button * pBtnAcr = dynamic_cast<ui::Button *>(FindControl(L"net_monitor_btn"));
	if (pBtnAcr)
		pBtnAcr->SetStateTextColor(ui::ControlStateType::kControlStateNormal, isShow > 0 ? SELECT : NO_SELECT);
	ui::Button * pBtnQuit = dynamic_cast<ui::Button *>(FindControl(L"log_btn"));
	if (pBtnQuit)
		pBtnQuit->SetStateTextColor(ui::ControlStateType::kControlStateNormal, isShow > 0 ? NO_SELECT : SELECT);
}

bool CMainFrameUI::OnBtnMenu(ui::EventArgs* args)
{
	wstring boxName = args->pSender->GetName();
	static std::wstring arrButton[] = { L"menu_btn",L"set_btnbox",L"accrate_set_btn",L"quit_set_btn",
										L"cancel_set_btn",L"save_set_btn",L"net_btnbox",L"clean_btnbox",L"auto_btnbox"};

	if (boxName== arrButton[0])
	{
		ShowMenu();
	}
	if (boxName == arrButton[1]||boxName == arrButton[8])
	{
		STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
		ShowArccrateAndQiutSet(ConfigSet.nAccMode);
		if(ConfigSet.nTransPortMode)
			ShowArccrateAndQiutSet(5);
		else
			ShowArccrateAndQiutSet(4);

		if(ConfigSet.nExitMode)
			ShowArccrateAndQiutSet(7);
		else
			ShowArccrateAndQiutSet(6);
		if (ConfigSet.nExitFix)
		{
			ui::CheckBox* pCheackWarn= dynamic_cast<ui::CheckBox*>(FindControl(L"quit_warn_chbox"));
			if (pCheackWarn)
				pCheackWarn->Selected(true);
		}
		
		IsSelectedArccrateBtn(true);
		if (!ShowOrHideMenuSetWnd(true))
			return false;
	}
	if (boxName == arrButton[2])
	{
		IsSelectedArccrateBtn(true);
		if (!ShowSetArccrateOrQuitWnd(true))
			return false;
	}
	if (boxName == arrButton[3])
	{
		IsSelectedArccrateBtn(false);
		if (!ShowSetArccrateOrQuitWnd(false))
			return false;
	}
	if (boxName == arrButton[4])
	{
		if (!ShowOrHideMenuSetWnd(false))
			return false;
	}
	if (boxName == arrButton[5])
	{
		STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
		GetMenuSet(ConfigSet);
		//reset but
		ui::ButtonBox * pBtnAccMode = dynamic_cast<ui::ButtonBox *>(FindControl(L"auto_btnbox"));
		if (pBtnAccMode)
		{
			switch (ConfigSet.nAccMode)
			{
			case 0:
				pBtnAccMode->SetText(L"自动");
				break;
			case 1:
				pBtnAccMode->SetText(L"LSP模式");
				break;
			case 2:
				pBtnAccMode->SetText(L"HOOK模式");
				break;
			case 3:
				pBtnAccMode->SetText(L"Driver模式");
				break;
			default:
				break;
			}
			
		}
		if (!ShowOrHideMenuSetWnd(false))
			return false;
	}
	if (boxName == arrButton[6])
	{
		if (!ShowMenuRepairNetOrCleanBufWnd(boxName))
			return false;
		LOGIC_CENTER()->NetWorkFix();
	}
	if (boxName == arrButton[7])
	{
		if (!ShowMenuRepairNetOrCleanBufWnd(boxName))
			return false;
		LOGIC_CENTER()->ClearBuffer();
	}
		
	return true;
}

bool CMainFrameUI::CreateTipMsgBox(std::wstring TipText)
{
	
	int nSelect = 0;
	uiTipMessageBox *tipChildWnd = new uiTipMessageBox(m_hWnd,TipText);
	nSelect = tipChildWnd->DoModal();
	delete tipChildWnd;
	
	return nSelect;
}

bool CMainFrameUI::OnSuccessTip(ui::VBox * pBox,std::wstring boxName)
{
	pBox->SetVisible(false);
	ui::VBox * pSuccessfulTipBox = dynamic_cast<ui::VBox*>(FindControl(L"operiton_successful_tip_label"));
	if (pSuccessfulTipBox)
	{
		ui::Label * pTipText = dynamic_cast<ui::Label*>(FindControl(L"successful_text_label"));
		if (pTipText)
		{
			if(boxName ==L"clean_btnbox")
				pTipText->SetText(L"缓存已清除");
			if (boxName == L"net_btnbox")
				pTipText->SetText(L"网络修复成功");
		}
		pSuccessfulTipBox->SetVisible(true);
	}
	return true;
}

bool CMainFrameUI::OnBtnWx(ui::EventArgs* args)
{
	if (m_bWXBind)
	{
		return false;
	}

	if (m_pCefBind)
	{
		wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
		wsUrl = BIND_URL + wsUrl;
		m_pCefBind->LoadURL(wsUrl.c_str());
	}
	ui::VBox * pWxBox = dynamic_cast<ui::VBox*>(FindControl(L"wx_bind_wnd_box"));
	if (pWxBox)
		pWxBox->SetVisible(true);
	return true;
}

bool CMainFrameUI::OnNickGetFocusEvent(ui::EventArgs* args)
{
	((ui::RichEdit *)args->pSender)->SetPromptText(L"");
	return true;
}

bool CMainFrameUI::OnBtnMenuKillFocus(ui::EventArgs* args)
{
	if (m_pMenuBox)
		m_pMenuBox->SetVisible(false);

	return true;
}

STGame CMainFrameUI::GetGameClickObj(int platId,int gameId)
{
	STGamePlat &GamePlat = LOGIC_CENTER()->GetGamePlat();
	for (STGamePlatItem &platSelect : GamePlat.vctPlats)
	{
		if (platSelect.wsPlatName == L"国服")
		{
			if (platSelect.nPlatID == platId)
			{
				STGameList & rfChn = LOGIC_CENTER()->GetGameListChn();
				for (auto&p1 : rfChn.vctGames)
				{
					if (p1.nGameID == gameId)
						return p1;
				}
			}
			break;
		}
	}
	STGameList & rfFor = LOGIC_CENTER()->GetGameListFor();
	for (auto&p2 : rfFor.vctGames)
	{
		if (p2.nGameID == gameId)
			return p2;
	}
	return STGame{};
}


void CMainFrameUI::CreateAreaElementList()
{

	if (m_areaVbox) {
		m_areaVbox->CreateAreaElementList();
		return;
	}

	int i = 0;
	std::wstring game_id;
	std::wstring id_numb;
	ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindControl(L"area_listbox"));
	if (pMainAreaListBox)
	{
		pMainAreaListBox->AttachSelect(nbase::Bind(&CMainFrameUI::OnAreaSelected, this, std::placeholders::_1));
		//std::vector<ui::ListContainerElement*> pViewListElementArea(500);
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
				ui::Label * pAreaLabel = dynamic_cast<ui::Label *>(FindSubControlByName(pElem, L"area_prov")) ;
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
						auto pTip = FindSubControlByName(args->pSender, L"loc_tip");
						
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


}

void CMainFrameUI::SelectAreaItem(wstring wsKey)
{
	auto pServer = m_mapXmlList.find(L"area_btnbox");
	if (pServer->second) {
		OutputDebugString(L"SelectAreaItem -----  ");
		 
	}
	else {
		OutputDebugString(L"SelectAreaItem ----- cannot find, return. ");
	}


	//ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindControl(L"area_listbox"));
	ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(pServer->second->FindSubControl(L"area_listbox"));
	if (pMainAreaListBox) {
		OutputDebugString(L"SelectAreaItem ----- find area_listbox ");
	}
	else {
		OutputDebugString(L"SelectAreaItem ----- cannot find area_listbox ");
		return;
	}
	for (int i = 0; i < pMainAreaListBox->GetCount(); i++)
	{
		ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(i));
		if (pElem)
		{
			
			wstring wsName = pElem->GetName();
			if (pElem->GetName() == stralgo::StringHelper::FormatW(L"area_%s", wsKey.c_str()))
			{
				pMainAreaListBox->SelectItem(i);
				pElem->SetBorderColor(L"select_node_border_color");
				ui::Control * pLocIcon = (FindSubControlByName(pElem, L"loc_icon"));
				if (pLocIcon)
				{
					pLocIcon->SetVisible(true);
				}
				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(true);
				}
			}
			else
			{
				pElem->SetBorderColor(L"no_select_node_border_color");
				ui::Control * pLocIcon = (FindSubControlByName(pElem, L"loc_icon"));
				if (pLocIcon)
				{
					pLocIcon->SetVisible(false);
				}
				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(false);
				}
			}
		}
	}
}


bool CMainFrameUI::OnAreaSelected(ui::EventArgs* args)
{
	int current = args->wParam;
	int old =  args->lParam;
	int nTipSelect = 0;

	if (current != 0)
	{
		old = max(0, args->lParam);
	}

	if (m_nIsArccrate)
	{
		if (nTipSelect=CreateTipMsgBox())
		{
			SetStopAccrate();
		}
	}
	bool bDoReplace(false);
	ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindControl(L"area_listbox"));
	if (pMainAreaListBox)
	{
		uint32_t ItemCount = pMainAreaListBox->GetCount();
		if (0 <= current && current < ItemCount)
		{
			ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(current));
			if (pElem)
			{
				
				//pElem->SetBorderColor(L"light_green");
				pElem->SetBorderColor(L"select_node_border_color");
				//ui::Control * pLocIcon = (FindSubControlByName(pElem, L"loc_icon"));
				//if (pLocIcon)
				//{
				//	pLocIcon->SetVisible(true);
				//}
				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					//if (nTipSelect &&m_nIsArccrate)
						pLocTip->SetVisible(true);
				}

				ui::Label * pProvName = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"area_prov"));

				ui::ButtonBox * pBtnAreaBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
				if (pBtnAreaBtnBox && pProvName)
				{
					bDoReplace = true;
					m_wsProvinceName = pProvName->GetText();
					if (m_nIsArccrate)
					{
						if (nTipSelect)
						{
							pBtnAreaBtnBox->SetText(pProvName->GetText());
						}
					}
					else
					{
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
				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(false);
				}
			}
		}

		if (bDoReplace)
		{
			OnGetGroupLineResponse(0);
		}

		auto parentXml = m_mapXmlParentBox.find(L"push_window_box");
		parentXml->second->SetVisible(false);
 	}

	return true;
}




bool CMainFrameUI::OnSvrSelected(ui::EventArgs* args)
{
	int current = args->wParam;
	int old = args->lParam;
	int nTipSelect = 0;
	if (current != 0)
	{
		old = max(0, args->lParam);
	}

	if (m_nIsArccrate)
	{
		if (nTipSelect =CreateTipMsgBox())
		{
			SetStopAccrate();
		}
	}

	ui::ListBox * pMainAreaListBox = dynamic_cast<ui::ListBox*>(FindControl(L"server_listbox"));
	if (pMainAreaListBox)
	{
		uint32_t ItemCount = pMainAreaListBox->GetCount();
		if (0 <= current && current < ItemCount)
		{
			ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(current));
			if (pElem)
			{
				pElem->SetBorderColor(L"select_node_border_color");
	
				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(true);
				}

				ui::Label * pSvrName = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"game_name"));

				ui::ButtonBox * pBtnSvrBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(L"server_btnbox"));
				if (pBtnSvrBtnBox && pSvrName)
				{
					if (m_nIsArccrate)
					{
						if (m_nIsArccrate)
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
					OnAreaSelect(_wtoi(wsName.c_str() + wcslen(L"svr_")));
				}
			}
		}

		if (0 <= old && old < ItemCount)
		{
			ui::ListContainerElement* pElem = dynamic_cast<ui::ListContainerElement*>(pMainAreaListBox->GetItemAt(old));
			if (pElem)
			{
				pElem->SetBorderColor(L"no_select_node_border_color");

				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(false);
				}
			}
		}

		auto parentXml = m_mapXmlParentBox.find(L"push_window_box");
		parentXml->second->SetVisible(false);
	}

	return true;
}


void CMainFrameUI::OnGameSelect(int mGameID)
{
	if (mGameID == m_nGameID)
	{
		return;
	}

	STGame stGame;
	OutputDebugString(wstring(L"OnGameSelect  ------> ").c_str() );
	if (LOGIC_CENTER()->FindGameOnID(mGameID, stGame))
	{
		if (m_serverVbox) {
			m_serverVbox->CreateServerElementList(stGame);
		}
		/*
		ui::ListBox * pMainServerListBox = dynamic_cast<ui::ListBox*>(FindControl(L"server_listbox"));
		if (pMainServerListBox)
		{
			pMainServerListBox->RemoveAll();
			pMainServerListBox->AttachSelect(nbase::Bind(&CMainFrameUI::OnSvrSelected, this, std::placeholders::_1));
			for (STGameArea & itr : stGame.vctAreas)
			{
				ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/server_model_box.xml");
				if (pElem)
				{
					pElem->SetName(stralgo::StringHelper::FormatW(L"svr_%d", itr.nID));
					pMainServerListBox->Add(pElem);
					
					ui::Label * pLbSvr = dynamic_cast<ui::Label *>(FindSubControlByName(pElem, L"game_name")) ;
					if (pLbSvr)
					{
						pLbSvr->SetText(itr.wsTitle);
					}

					ui::Label * pLbSubTitle = dynamic_cast<ui::Label *>(FindSubControlByName(pElem, L"game_server"));
					if (pLbSubTitle)
					{
						pLbSubTitle->SetText(itr.wsSubTitle);
					}

					ui::Label * pAreaLogo = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"game_logo"));
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
							auto pTip = FindSubControlByName(args->pSender, L"loc_tip");

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
		*/
		m_nGameID = mGameID;
	}
}

void CMainFrameUI::OnAreaSelect(int nArea)
{
	if (m_nAreaID != nArea)
	{
		m_nAreaID = nArea;
		ui::ButtonBox * pBtnProv = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
		if (pBtnProv)
		{
			wstring wsProvName = pBtnProv->GetText();
			nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
				LOGIC_CENTER()->GetLineGroup(m_nGameID, nArea, wsProvName);
			}));
		}

		
	}

}

void CMainFrameUI::ClearListCtrl(ui::Control * pCtrl)
{
	ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(pCtrl);
	if (pMainOperatorListBox)
	{
		pMainOperatorListBox->RemoveAll();
		//pMainOperatorListBox->AttachSelect(nbase::Bind(&CMainFrameUI::OnOpertSelected, this, std::placeholders::_1));
		pMainOperatorListBox->AttachSelect(nbase::Bind(&nui::COperVBox::OnOpertSelected, m_operVbox, std::placeholders::_1));
	}
}

void CMainFrameUI::UpdateListCtrl(ui::Control * pCtrl)
{
	ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(pCtrl);
	if (pMainOperatorListBox)
	{
		
	}
}

/*
bool CMainFrameUI::OnOpertSelected(ui::EventArgs* args)
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

	if (m_nIsArccrate)
	{
		if (nTipSelect=CreateTipMsgBox())
		{
			SetStopAccrate();
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

					ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(true);
					}
				}
				else
				{
					pElem->SetBorderColor(L"select_node_border_color");

					ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
					if (pLocTip)
					{
						pLocTip->SetVisible(false);
					}
				}


				wstring wsLineName;
				ui::Label * pLineName = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"line_name"));
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
						m_nSvrID = _wtoi(vctSplt[1].c_str());
						int nIsp = _wtoi(vctSplt[2].c_str());
						ui::ButtonBox * pBtnOperatorBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(L"operator_btnbox"));
						{
							if (m_nIsArccrate)
							{
								if (nTipSelect)
								{
									if (0&&wsLineName.size())
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
								if (0&&wsLineName.size())
								{
									pBtnOperatorBtnBox->SetText(get_isp_name(nIsp) + L" " + wsLineName);
								}
								else
								{
									pBtnOperatorBtnBox->SetText(get_isp_name(nIsp));
								}
								
							}

						}
						if (m_nSvrID)
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

				ui::Control * pLocTip = (FindSubControlByName(pElem, L"loc_tip"));
				if (pLocTip)
				{
					pLocTip->SetVisible(false);
				}
			}
		}
		auto parentXml = m_mapXmlParentBox.find(L"push_window_box");
		parentXml->second->SetVisible(false);
	}

	return true;
}

*/
wstring CMainFrameUI::GetIspNameOnID(int nIsp)
{
	wstring wsRet(L"全服");
	if (nIsp == ISP_TELCOM)
	{
		wsRet = L"电信";
	}
	else if (nIsp == ISP_MOBILE)
	{
		wsRet = L"移动";
	}
	else if (nIsp == ISP_UNICOM)
	{
		wsRet = L"联通";
	}

	return wsRet;
}

wstring CMainFrameUI::GetLineItemName(STLineItem & itrItem)
{
	wstring wsRet;
	vector<wstring> vctSplit;
	stralgo::StringHelper::SplitString(itrItem.wsValue, L'|', vctSplit);
	//OutputDebugString(L"GetLineItemName");
	if (vctSplit.size() > 2)
	{
		ui::ButtonBox * pBtnAreaBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
		if (pBtnAreaBtnBox)
		{
			wsRet = pBtnAreaBtnBox->GetText();
			wsRet += L"-";
			wsRet += GetIspNameOnID(itrItem.nIspType);
			wsRet += L"-";
			wsRet += itrItem.wsLineName;
		}
	}

	return wsRet;

}
/*
void CMainFrameUI::OnLineGroupResponse(int nGameID, int nAreaID)
{
	static wstring arrListTable[] = { L"operator_listbox_other", L"operator_listbox_dx", L"operator_listbox_lt", L"operator_listbox_yd", L"operator_listbox_recommend" };
	//	STLineGroupStrore & groupData = LOGIC_CENTER()->FindLineGroup(nGameID, nAreaID);
	STGroupItem *  pGroupItem = LOGIC_CENTER()->FindGroupItem(nGameID, nAreaID, m_wsProvinceName);
	if (pGroupItem)
	{
		EnableCtrl(this, L"start_accrate_btn", TRUE);
		ClearListCtrl(FindControl(L"operator_listbox_recommend"));
		ClearListCtrl(FindControl(L"operator_listbox_dx"));
		ClearListCtrl(FindControl(L"operator_listbox_yd"));
		ClearListCtrl(FindControl(L"operator_listbox_lt"));
		ClearListCtrl(FindControl(L"operator_listbox_other"));
		ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(FindControl(L"operator_listbox_recommend"));
		if (pMainOperatorListBox)
		{
			STLineItem * pFastLineItem = NULL;
			for (STLineItem & itrf : pGroupItem->vctGroupLines)
			{
				if (pFastLineItem == NULL)
				{
					pFastLineItem = &itrf;
				}
				else
				{
					if (pFastLineItem->nDelay > itrf.nDelay)
					{
						pFastLineItem = &itrf;
					}
				}
			}
			{
				int nDelayGuard(1000);
				if (pGroupItem->vctGroupLines.size()/*groupData.stData.vctGroups.size()*/    /*)
				{
					ui::ListContainerElement * pElemBest(NULL);
		
					STGeoIP & stGeoIP = LOGIC_CENTER()->GetGeoIP();
					STLineItem arrFackItem[6];
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
									arrFackItem[i] = itr;
									bMatch = true;

								}
							}
						}
					}
					for (int i = 0; i < 6 && pFastLineItem; i++)
					{
						if (arrFackItem[i].nLineID == 0)
						{
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


	
					std::sort(arrFackItem, arrFackItem + 6, [&](STLineItem & left, STLineItem & right) {
						return left.nDelay < right.nDelay;
					});

					for (int i = 0; i < 6; i++)
					{
						arrFackItem[i].wsLineName = L'A' + i;
					}
					int nBest(-1);
					int i = 0;
					for (STLineItem & itr : arrFackItem)
					{
						if (1)
						{

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
										auto pTip = FindSubControlByName(args->pSender, L"loc_tip");

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

					if (pElemBest)
					{
						pElemBest->SetBorderColor(L"node_select_bk_color");

						ui::Control * pLocTip = (FindSubControlByName(pElemBest, L"loc_tip"));
						if (pLocTip)
						{
							pLocTip->SetVisible(true);
						}
						ui::Control * pBestTip = (FindSubControlByName(pElemBest, L"best_tip"));
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
							ui::ListBox * pMainOperatorListBoxIsp = dynamic_cast<ui::ListBox*>(FindControl(arrListTable[itr.nIspType]));
							if (pMainOperatorListBoxIsp)
							{
								ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/operator_model_box.xml");
								if (pElem)
								{
									ui::Control * pGameLogo = FindSubControlByName(pElem, L"game_logo");
									if (pGameLogo)
									{
										pGameLogo->SetBkImage(Get_Isp_logo(itr.nIspType));
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
											auto pTip = FindSubControlByName(args->pSender, L"loc_tip");

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
	}
}
*/
void CMainFrameUI::OnOpertEvnBimd()
{
	static wstring arrTable[] = { L"tj_btn_oper", L"dx_btn_oper", L"yd_btn_oper", L"lt_btn_oper" };//, L"other_btn_oper"
	ui::Control * pParent = FindControl(L"push_window_box");
	if (pParent)
	{
		for (wstring & itr : arrTable)
		{
			ui::Button * pBut = dynamic_cast<ui::Button *>(FindSubControlByName(pParent, itr)) ;
			if (pBut)
			{
				pBut->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnOpertTabClick, this, std::placeholders::_1));
			}
		}
		
	}
}

bool CMainFrameUI::OnBtnOpertTabClick(ui::EventArgs* args)
{
	static wstring arrTable[] = { L"tj_btn_oper", L"dx_btn_oper", L"yd_btn_oper", L"lt_btn_oper" };//, L"other_btn_oper"
	static wstring viewTable[] = { L"operator_listbox_recommend", L"operator_listbox_dx", L"operator_listbox_yd", L"operator_listbox_lt", L"operator_listbox_other" };
	int nHit(-1);
	for (int i = 0; i < _countof(viewTable); i++)
	{
		ui::Control * pView = FindControl(viewTable[i]);
		if (pView)
		{
			pView->SetVisible(false);
		}
	}
	for (int i = 0; i < _countof(arrTable); i++)
	{
		if (args->pSender->GetName() == arrTable[i])
		{
			ui::Control * pView = FindControl(viewTable[i]);
			if (pView)
			{
				pView->SetVisible(true);
			}
	
			nHit = i;
			((ui::Button*)args->pSender)->SetStateTextColor(ui::kControlStateNormal, L"node_select_bk_color");
		}
		else
		{
			ui::Button * pBtn = dynamic_cast<ui::Button *>(FindControl(arrTable[i])) ;
			if (pBtn)
			{
				pBtn->SetStateTextColor(ui::kControlStateNormal, L"node_noselect_bk_color");
			}
		}
	}

	return true;
}

void CMainFrameUI::CreateNickElementList(vector<STGame> vctNickGame)
{
	OutputDebugString(L"CreateNickElementList--->");
	int i = 0;
	int spiltIndex = 0;
	std::wstring game_id;
	std::wstring id_numb;
	StringVector nameVct;
	ui::ListBox * pNickListBox = dynamic_cast<ui::ListBox*>(FindControl(L"search_result_listbox0"));
	pNickListBox->RemoveAll();
	auto pNick = vctNickGame.begin();
	for (; i < vctNickGame.size(); i++)
	{
		spiltIndex = stralgo::StringHelper::SplitString(pNick->wsGameTitle,nameVct, L"-");
		game_id = L"nick_game_" + std::to_wstring(pNick->nPlatID) + L"_id_" + std::to_wstring(pNick->nGameID);
		ui::ListContainerElement * NickElement= buildSubListContainerElement(L"leigod/model/nick_model_box.xml");
		if (NickElement)
		{
			pNickListBox->Add(NickElement);

			NickElement->SetName(game_id);
			ui::Label * pGameLogo = dynamic_cast<ui::Label *>(FindControl(L"nick_game_logo"));
			pGameLogo->SetBkImage(pNick->wsLogoPath);

			ui::Label * pGameName = dynamic_cast<ui::Label *>(FindControl(L"nick_game_name"));
			pGameName->SetText(pNick->wsGameTitle);

			ui::Label * pGameServer = dynamic_cast<ui::Label *>(FindControl(L"nick_game_server"));
			pGameServer->SetText(pNick->wsSubTitle);

			NickElement->SetClass(L"listitem2");
			NickElement->AttachSelect(nbase::Bind(&CMainFrameUI::OnGameListClick, this, std::placeholders::_1));
			//NickElement->AttachSetFocus(nbase::Bind(&CMainFrameUI::OnElementGetFocus, this, std::placeholders::_1));
			NickElement->SetVisible(true);
			//m_mapNickList.insert(Insert_Pair_server(game_id, pViewListElementNick[i]));
		}
		
		game_id = L"";
		pNick++;
	}
	OutputDebugString(L"<-----     CreateNickElementList ");
}

void CMainFrameUI::CreateGameElement(ui::ListBox * gameList, vector<STGame>::iterator gameVct)
{
	int spiltIndex = 0;
	std::wstring game_id;
	std::wstring id_numb;
	//std::wstring gameName;
	//std::wstring gameServerName;
	GFGameLIST gameBox;

	game_id = L"game_plat_"+ std::to_wstring(gameVct->nPlatID)+L"_id_"+ std::to_wstring(gameVct->nGameID);

	ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"leigod/model/game_model_box.xml");
	if (ElementBox)
	{
		//spiltIndex = gameVct->wsGameTitle.find(L"-");
		//gameServerName = gameVct->wsGameTitle.substr(0, spiltIndex);
		//gameName = gameVct->wsGameTitle.substr(spiltIndex + 1, gameVct->wsGameTitle.size());

		gameList->Add(ElementBox);
		gameBox.GameName = gameVct->wsGameTitle;
		gameBox.nPlate = gameVct->nPlatID;
		gameBox.nGameId = gameVct->nGameID;
		gameBox.Element = ElementBox;
		vctGame.push_back(gameBox);
		ElementBox->SetName(game_id);
		ui::Label * pGameLogo = dynamic_cast<ui::Label *>(FindControl(L"game_logo"));
		pGameLogo->SetBkImage(gameVct->wsLogoPath);
		ui::Label * pGameName = dynamic_cast<ui::Label *>(FindControl(L"game_name"));
		pGameName->SetText(gameVct->wsGameTitle);
		
		ui::Label * pGameServer = dynamic_cast<ui::Label *>(FindControl(L"game_server"));
		pGameServer->SetText(gameVct->wsSubTitle);
		ElementBox->SetClass(L"listitem");
		ElementBox->AttachSelect(nbase::Bind(&CMainFrameUI::OnGameListClick, this, std::placeholders::_1));
	
	}
	game_id = L"";
}
void CMainFrameUI::CreateGameElementList()
{
	int i = 0;
	int count = 0;
	
	std::wstring game_id;
	std::wstring id_numb;
	ui::ListBox * pAllGameListBox = dynamic_cast<ui::ListBox*>(FindControl(L"all_game_listbox"));
	
	
	STGamePlat &GamePlat = LOGIC_CENTER()->GetGamePlat();

	STGameList & GameFor = LOGIC_CENTER()->GetGameListFor();
	auto pFor = GameFor.vctGames.begin();
	for (; i < GameFor.nCount; i++)
	{
		if (GamePlat.vctPlats.size() < 4)
			return ;

		CreateGameElement(pAllGameListBox,pFor);
		if(pFor!= GameFor.vctGames.end())
			pFor++;
	}

	STGameList & rfChn = LOGIC_CENTER()->GetGameListChn();
	auto pChn = rfChn.vctGames.begin();
	std::vector<ui::ListContainerElement*> pViewListElementGameBoxChn(rfChn.nCount);
	for (i=0; i < rfChn.nCount; i++)
	{
		CreateGameElement(pAllGameListBox, pChn);
		pChn++;
	}
}
void CMainFrameUI::CreateCloseWndBind()
{
	static wstring arrClose[] = {  L"close_set_btn",
								L"cancel_set_btn",L"close_Tip_window_btn",L"operiton_successful_ok_btn",L"close_wx_window_btn"};
	for (wstring &name : arrClose)
	{
	
		ui::Button * pBtnClose = dynamic_cast<ui::Button *>(FindControl(name));
		if (pBtnClose)
		{
			pBtnClose->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnPushWindowCloseClick, this, std::placeholders::_1));
		}
		
	}
	ui::Button * pBtnMonitorClose = dynamic_cast<ui::Button *>(FindControl(L"close_monitor_btn"));
	if (pBtnMonitorClose)
	{
		pBtnMonitorClose->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnCloseMonitorWnd, this, std::placeholders::_1));
	}

	ui::Button * pBtnLogo = dynamic_cast<ui::Button *>(FindControl(L"lg_logo_btn"));
	if (pBtnLogo)
	{
		pBtnLogo->AttachClick([=](ui::EventArgs* args) {
		
			::ShellExecuteA(NULL, "open", "https://jiasu.nn.com/", "", NULL, SW_HIDE);
			return true;
		});
	}
}
void CMainFrameUI::CreateTabBind()
{
	static wstring arrTab[] = { L"myspeed_btn",L"all_btn", L"platform_btn", L"Steam_btn", L"Origin_btn",
								L"Uplay_btn",L"Epic_btn",L"zw_btn",L"gf_btn"};
	for (wstring &name : arrTab)
	{
		
		ui::Button * pBtnTab = dynamic_cast<ui::Button *>(FindControl(name));
		if (pBtnTab)
		{
			pBtnTab->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnTabClick, this, std::placeholders::_1));
		}
		else
		{
			ui::ButtonBox * pBtnBoxTab = dynamic_cast<ui::ButtonBox *>(FindControl(name));
			if (pBtnBoxTab)
			{
				pBtnBoxTab->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnTabClick, this, std::placeholders::_1));
			}
		}
		
	}
}

void CMainFrameUI::CreateMenuBind()
{
	static wstring arrMenu[] = { L"menu_btn", L"set_btnbox", L"net_btnbox", L"clean_btnbox",
								L"cancel_set_btn",L"save_set_btn",L"accrate_set_btn",L"quit_set_btn",L"auto_btnbox"};
	for (wstring &name : arrMenu)
	{
	
		ui::Button * pBtnMenu = dynamic_cast<ui::Button *>(FindControl(name));
		if (pBtnMenu)
		{
			pBtnMenu->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnMenu, this, std::placeholders::_1));
		}
		else
		{
			ui::ButtonBox * pBtnBoxMenu = dynamic_cast<ui::ButtonBox *>(FindControl(name));
			if (pBtnBoxMenu)
			{
				pBtnBoxMenu->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnMenu, this, std::placeholders::_1));
			}
		}
		
	}
	ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"menu_window_box"));
	if (pBox)
	{
		pBox->AttachKillFocus(nbase::Bind(&CMainFrameUI::OnBtnMenuKillFocus, this, std::placeholders::_1));
	}
}
void CMainFrameUI::CreateSvrSelectBind()
{
	static wstring arrSvr[] = { L"server_btnbox", L"area_btnbox", L"operator_btnbox"};
	for (wstring &name : arrSvr)
	{
		
		ui::Button * pBtnSvr = dynamic_cast<ui::Button *>(FindControl(name));
		if (pBtnSvr)
		{
			pBtnSvr->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnSelectNodeClick, this, std::placeholders::_1));
		}
		else
		{
			ui::ButtonBox * pBtnBoxSvr = dynamic_cast<ui::ButtonBox *>(FindControl(name));
			if (pBtnBoxSvr)
			{
				pBtnBoxSvr->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnSelectNodeClick, this, std::placeholders::_1));
			}
		}
	}
	
}

void CMainFrameUI::CreateMonitorBind()
{
	static wstring arrMonitor[] = { L"monitor_btnbox", L"show_time_1_btn",L"net_monitor_btn",L"log_btn",L"show_time_1_btn",L"show_time_5_btn",L"show_time_10_btn" };
	for (wstring &name : arrMonitor)
	{
		
		ui::Button * pBtn = dynamic_cast<ui::Button *>(FindControl(name));
		if (pBtn)
		{
			pBtn->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnMonitorWndClick, this, std::placeholders::_1));
		}
		else
		{
			ui::ButtonBox * pBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(name));
			if (pBtnBox)
			{
				pBtnBox->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnMonitorWndClick, this, std::placeholders::_1));
			}
		}
	}
	
}

void CMainFrameUI::CreateAccrateBind()
{
	ui::ButtonBox * pBtnStartAccrate = dynamic_cast<ui::ButtonBox *>(FindControl(L"start_accrate_btn"));
	if (pBtnStartAccrate)
	{
		pBtnStartAccrate->SetEnabled(false);
		pBtnStartAccrate->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnAccrateStart, this, std::placeholders::_1));
	}
	ui::ButtonBox * pBtnStopAccrate = dynamic_cast<ui::ButtonBox *>(FindControl(L"stop_accrate_btn"));
	if (pBtnStopAccrate)
	{
		pBtnStopAccrate->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnAccrateStop, this, std::placeholders::_1));
	}
}

void CMainFrameUI::CreateNickBind()
{
	ui::RichEdit * pEditNick = dynamic_cast<ui::RichEdit *>(FindControl(L"nick"));
	if (pEditNick)
	{
		pEditNick->AttachTextChange(nbase::Bind(&CMainFrameUI::OnNickTextChangeEvent, this, std::placeholders::_1));
		pEditNick->AttachKillFocus(nbase::Bind(&CMainFrameUI::OnNickKillFocusEvent, this, std::placeholders::_1));
		pEditNick->AttachSetFocus(nbase::Bind(&CMainFrameUI::OnNickGetFocusEvent, this, std::placeholders::_1));
	}
}
void CMainFrameUI::CreateTipBind()
{
	ui::ButtonBox * pTipBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(L"tip_btn"));
	if (pTipBtnBox)
	{
		pTipBtnBox->AttachMouseHover(nbase::Bind(&CMainFrameUI::OnBtnTipEvent, this, std::placeholders::_1));
		pTipBtnBox->AttachMouseLeave(nbase::Bind(&CMainFrameUI::OnBtnLeaveEvent, this, std::placeholders::_1));
	}
}

void CMainFrameUI::CreateNoTypeBind()
{
	ui::Button * pBtnChangeGamePhoto = dynamic_cast<ui::Button *>(FindControl(L"cruuent_photo_btn"));
	if (pBtnChangeGamePhoto)
	{
		pBtnChangeGamePhoto->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnChangeCurrentPhotoClick, this, std::placeholders::_1));
	}

	ui::Button * pBtnWx = dynamic_cast<ui::Button *>(FindControl(L"wx_btn"));
	if (pBtnWx)
	{
		pBtnWx->AttachClick(nbase::Bind(&CMainFrameUI::OnBtnWx, this, std::placeholders::_1));
	}

	ui::Button * pBtnSMTailB1 = dynamic_cast<ui::Button *>(FindControl(L"sm_g1"));
	if (pBtnSMTailB1)
	{
		pBtnSMTailB1->AttachClick([=](ui::EventArgs* args) {
			STSvrSet & rfData = LOGIC_CENTER()->GetSvrSetInfo();
			if (StrStrIA(rfData.ssTargetUrlSM[0].c_str(), REWARD_KEYWORD)|| StrStrIA(rfData.ssTargetUrlSM[0].c_str(), "choujiang"))
			{
				ShowRewardPage();	//ShowCtrl(this, L"lottery_box");
			}
			else
			{
				::ShellExecuteA(NULL, "open", rfData.ssTargetUrlSM[0].c_str(), "", NULL, SW_HIDE);
			}
	
			return true;
		});
	}

	ui::Button * pBtnSMTailB2 = dynamic_cast<ui::Button *>(FindControl(L"sm_g2"));
	if (pBtnSMTailB2)
	{
		pBtnSMTailB2->AttachClick([=](ui::EventArgs* args) {
			STSvrSet & rfData = LOGIC_CENTER()->GetSvrSetInfo();
			if (StrStrIA(rfData.ssTargetUrlSM[1].c_str(), REWARD_KEYWORD) || StrStrIA(rfData.ssTargetUrlSM[0].c_str(), "choujiang"))
			{
				ShowRewardPage();	//ShowCtrl(this, L"lottery_box");
			}
			else
			{
				::ShellExecuteA(NULL, "open", rfData.ssTargetUrlSM[1].c_str(), "", NULL, SW_HIDE);
			}
			
			return true;
		});
	}

	ui::Button * pBtnBYTailB1 = dynamic_cast<ui::Button *>(FindControl(L"by_g1"));
	if (pBtnBYTailB1)
	{
		
		pBtnBYTailB1->AttachClick([=](ui::EventArgs* args) {
			STSvrSet & rfData = LOGIC_CENTER()->GetSvrSetInfo();
			::ShellExecuteA(NULL, "open", rfData.ssTargetUrlBY[0].c_str(), "", NULL, SW_HIDE);
			return true;
		});
	}

	ui::Button * pBtnBYTailB2 = dynamic_cast<ui::Button *>(FindControl(L"by_g2"));
	if (pBtnBYTailB2)
	{
		pBtnBYTailB2->AttachClick([=](ui::EventArgs* args) {
			STSvrSet & rfData = LOGIC_CENTER()->GetSvrSetInfo();
			::ShellExecuteA(NULL, "open", rfData.ssTargetUrlBY[1].c_str(), "", NULL, SW_HIDE);
			return true;
		});
	}

	ui::Button * pBtnKF = dynamic_cast<ui::Button *>(FindControl(L"kf_btn"));
	if (pBtnKF)
	{
		pBtnKF->AttachClick([=](ui::EventArgs* args) {
			//::ShellExecuteA(NULL, "open", TOOLBAR_KF_URL, "", NULL, SW_HIDE);
			 
			if(m_cefVBox) {
				m_cefVBox->construct(0, 0, 0, 0);
				m_cefVBox->SetVisible(true); 
				m_cefVBox->LodUrl(L"https://www.baidu.com");
			} 
			else {
				OutputDebugString(L"can not find cef...........................");
			}
			return true;
		});
	}

	ui::Button * pBtnCopyClipBox = dynamic_cast<ui::Button *>(FindControl(L"btn_copy_log_clipbox")); 
	if (pBtnCopyClipBox)
	{
		pBtnCopyClipBox->AttachClick([=](ui::EventArgs* args) {
			if (OpenClipboard(GetHWND()))//打开剪贴板
			{
				ui::RichEdit * pEditLog = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_aoo_log"));
				if (pEditLog)
				{
					HANDLE hClip;
					wchar_t* pBuf;
					EmptyClipboard();//清空剪贴板
					std::wstring ssText = pEditLog->GetText();
					hClip = GlobalAlloc(GMEM_MOVEABLE, 2*ssText.length() + 1);
					pBuf = (wchar_t*)GlobalLock(hClip);
					wcscpy(pBuf, ssText.c_str());
					GlobalUnlock(hClip);//解锁
					SetClipboardData(CF_UNICODETEXT, hClip);//设置格式

				  //关闭剪贴板
				}

				CloseClipboard();
				::MessageBoxExW(GetHWND(), L"已复制到剪贴板。", L"提示", MB_OK, 0);
			}
			return true;
		});
	}
}

void CMainFrameUI::CreateBtnBindClick()
{

	CreateMenuBind();
	CreateCloseWndBind();
	CreateTabBind();
	CreateSvrSelectBind();
	CreateMonitorBind();
	CreateAccrateBind();
	//OnOpertEvnBimd();
	CreateTipBind();
	CreateNickBind();
	CreateNoTypeBind();

}

void CMainFrameUI::ShowGameBox(std::wstring platName)
{
	
	STGamePlat &GamePlat = LOGIC_CENTER()->GetGamePlat();
	for (STGamePlatItem &pLat : GamePlat.vctPlats)
	{
		if (pLat.wsPlatName == platName)
		{
			for (const auto&p1 : vctGame)
			{
				if (p1.Element)
				{
					if (p1.nPlate == pLat.nPlatID)
						p1.Element->SetVisible(true);
					else
						p1.Element->SetVisible(false);
				}
				
			}
		}
	}
}

void CMainFrameUI::CreateChildModel()
{
	uiMessageBox *childWnd = new uiMessageBox(m_hWnd,m_nTimeCount,m_bWXBind);
	childWnd->DoModal();
	//delete childWnd;
}

LRESULT CMainFrameUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	using namespace  nim_comp;
	if (uMsg == WUM_TARYICON_MSG)
	{
		if (lParam == WM_LBUTTONUP)
		{
			ActiveWindow();
		}
		else if (lParam == WM_RBUTTONUP)
		{
			ui::CPoint point;
			GetCursorPos(&point);
			ClientToScreen(NULL, &point);

			CMenuWnd* pMenu = new CMenuWnd(NULL);
			ui::STRINGorID xml(L"tray_menu.xml");
			pMenu->Init(xml, _T("xml"), point);
			CMenuElementUI * pChangeUser = dynamic_cast<CMenuElementUI *>(pMenu->FindControl(L"mu_showframe"));
			if (pChangeUser)
			{
				pChangeUser->AttachClick([this](ui::EventArgs* args) {

					ActiveWindow();
					return true;
				});
			}
			CMenuElementUI * pChangeQuit = dynamic_cast<CMenuElementUI *>(pMenu->FindControl(L"mu_exit"));
			if (pChangeQuit)
			{
				pChangeQuit->AttachClick([this](ui::EventArgs* args) {

					Close();
					return true; });
			}
			pMenu->Show();
			return true;
		}
	}
	else if (uMsg == WM_NCLBUTTONDOWN)
	{
		//int i=0;
		//i++;
		ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"menu_window_box"));
		pBox->SetVisible(false);
	}
	//else if (WM_COPYDATA == uMsg)
	//{
	//	COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*)lParam;
	//	string ss = (char*)pCDS->lpData;
	//	MessageBox(gMainWnd->GetHWND(), U2W(ss).c_str(), L"", MB_OK);
	//}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

ui::Control* CMainFrameUI::CreateControl(const std::wstring& pstrClass)
{
	// 扫描 XML 发现有名称为 CefControl 的节点，则创建一个 ui::CefControl 控件
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}

	return NULL;
}

void CMainFrameUI::OnEnchageUrlChange(const std::wstring& url)
{
	LOG_INFO(WEB_LOG, L"sm log:" << url);
}


CefRequestHandler::ReturnValue CMainFrameUI::OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect)
{
	wstring wsNow = request->GetReferrerURL();
	wstring wsTo = request->GetURL();
	LOG_INFO(WEB_LOG, L"sm log:" << wsTo);
	if (StrStrIW(wsTo.c_str(), WEB_ENCHARGE_SUCC))
	{
		int nSubId = 0;
		int nHour = 0;
		int nMin = 0;
		int nTotalMin = 0;
		int nWxID = 0;
		leigod_netpub::UnPraseUrl(wsTo, nSubId, nHour, nMin, nTotalMin, nWxID);

		//LOGIC_CENTER()->GetPinRes().nWxID = nWxID;

		if (nSubId || nHour || nMin)
		{
			OnWebTimeArrive(nHour, nMin);
			LOGIC_CENTER()->GetPinRes().nSubUserID = nSubId;
			OnBtnWx(NULL);
			return RV_CANCEL;
		}

		LOG_INFO(WEB_LOG, L"sm ok:" << wsTo);
	}
	else if (StrStrIW(wsTo.c_str(), WEB_ENCHARGE_FAIL))
	{
		LOG_INFO(WEB_LOG, L"sm fail:" << wsTo);
		return RV_CANCEL;
	}
	else if (StrStrIW(wsTo.c_str(), L"get_order_result"))
	{
		return RV_CONTINUE;
	}
	return RV_CONTINUE;
}

void CMainFrameUI::OnLoadEnd(int httpStatusCode)
{
	/*
		m_pCefEnCharge->RegisterCppFunc(L"client_call", ToWeakCallback([this](const std::string& params, nim_comp::ReportResultFunction callback) {

		wstring wsTo = U2W(params);
		bool bCancel(false);
		if (StrStrIW(wsTo.c_str(), WEB_BIND_SUCC))
		{
			//wsTo = L"core://game/subuser_success:425131:648%e5%b0%8f%e6%97%b64%e5%88%86%e9%92%9f";
			m_bWXBind = true;
			int nHour = 0;
			int nMin = 0;
			int nTotalMin = 0;
			int nWxID = 0;
			LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
			leigod_netpub::GET_H_M(wsTo, nHour, nMin, nTotalMin, nWxID);
			LOGIC_CENTER()->GetPinRes().nWxID = nWxID;
			OnWebTimeArrive(nHour, nMin);
			bCancel = true;
			ui::Label * pWebLogMode = dynamic_cast<ui::Label*>(FindControl(L"login_tip_label"));
			if (pWebLogMode)
			{
				pWebLogMode->SetText(SM_MODE_TIP);
			}
			if (m_pCefEnCharge)
			{
				wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
				wsUrl = PAY_URL + wsUrl;
				m_pCefEnCharge->LoadURL(wsUrl.c_str());
			}
			LOG_INFO(WEB_LOG, L"bind ok:" << wsTo);
		}
		else if (StrStrIW(wsTo.c_str(), WEB_BIND_ERR))
		{
			wstring wsErr;
			LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo, &wsErr);
			if (wsErr.size())
			{
				::MessageBoxExW(GetHWND(), wsErr.c_str(), L"提示", MB_OK, 0);
			}
			bCancel = true;
			LOG_INFO(WEB_LOG, L"bind err:" << wsTo);
		}
		else if (StrStrIW(wsTo.c_str(), WEB_BIND_BINDTEL))
		{
			LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
			bCancel = true;
		}
		else if (StrStrIW(wsTo.c_str(), WEB_BIND_FEE))
		{
			m_bWXBind = true;
			LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
			bCancel = true;
			ui::Label * pWebLogMode = dynamic_cast<ui::Label*>(FindControl(L"login_tip_label"));
			if (pWebLogMode)
			{
				pWebLogMode->SetText(SM_MODE_TIP);
			}
			if (m_pCefEnCharge)
			{
				wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
				wsUrl = PAY_URL + wsUrl;
				m_pCefEnCharge->LoadURL(wsUrl.c_str());
			}
			LOG_INFO(WEB_LOG, L"bind fee:" << wsTo);
		}
		if (m_bWXBind)
		{
			EnableCtrl(this, L"wx_btn", FALSE);
			SetCtrlText<ui::Button>(this, L"wx_btn", L"已登录");
		}
		if (bCancel)
		{
			ui::VBox * pWxBox = dynamic_cast<ui::VBox*>(FindControl(L"wx_bind_wnd_box"));
			if (pWxBox)
				pWxBox->SetVisible(false);
			LOG_INFO(MAINLOG_DEF, L"" << wsTo);
		}
		//shared::Toast::ShowToast(nbase::UTF8ToUTF16(ssRaw), 3000, GetHWND());
		//callback(false, R"({ "message": "Success." })");
	}));
	*/

}

CefRequestHandler::ReturnValue CMainFrameUI::OnBeforeNavigateBind(CefRefPtr<CefRequest> request, bool is_redirect)
{
	wstring wsNow = request->GetReferrerURL();
	wstring wsTo = request->GetURL();
	bool bCancel(false);
	if (StrStrIW(wsTo.c_str(), WEB_BIND_SUCC))
	{
		//wsTo = L"core://game/subuser_success:425131:648%e5%b0%8f%e6%97%b64%e5%88%86%e9%92%9f";
		m_bWXBind = true;
		int nHour = 0;
		int nMin = 0;
		int nTotalMin = 0;
		wstring nWxID ;
		LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
		int * ptr = &LOGIC_CENTER()->GetPinRes().nSubUserID;
		leigod_netpub::GET_H_M(wsTo, nHour, nMin, nTotalMin, nWxID);
		LOGIC_CENTER()->GetPinRes().nWxID = nWxID;
		OnWebTimeArrive(nHour, nMin);
		bCancel = true;
		ui::Label * pWebLogMode = dynamic_cast<ui::Label*>(FindControl(L"login_tip_label"));
		if (pWebLogMode)
		{
			pWebLogMode->SetText(SM_MODE_TIP);
		}
		if (m_pCefEnCharge)
		{
			wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
			wsUrl = PAY_URL + wsUrl;
			m_pCefEnCharge->LoadURL(wsUrl.c_str());
		}
		
		LOG_INFO(WEB_LOG, L"bind ok:" << wsTo);
	}
	else if (StrStrIW(wsTo.c_str(), WEB_BIND_ERR))
	{
		wstring wsErr;
		LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo, &wsErr);
		if (StrStrIW(wsTo.c_str(), L"subuser_error:1002"))
		{
			ui::VBox * pWxBox = dynamic_cast<ui::VBox*>(FindControl(L"wx_bind_wnd_box"));
			if (pWxBox)
				pWxBox->SetVisible(false);
			::MessageBoxExW(GetHWND(), L"当前网吧尚未注册!", L"提示", MB_OK, 0);
			bCancel = true;
		}
		else
		{
			if (wsErr.size())
			{
				::MessageBoxExW(GetHWND(), wsErr.c_str(), L"提示", MB_OK, 0);
			}
		}

		bCancel = true;
		LOG_INFO(WEB_LOG, L"bind err:" << wsTo);
	}
	else if (StrStrIW(wsTo.c_str(), WEB_BIND_BINDTEL))
	{
		LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
		bCancel = true;
	}
	else if (StrStrIW(wsTo.c_str(), WEB_BIND_FEE))
	{
		m_bWXBind = true;
		LOGIC_CENTER()->GetPinRes().nSubUserID = leigod_netpub::GET_SUBID(wsTo);
		bCancel = true;
		ui::Label * pWebLogMode = dynamic_cast<ui::Label*>(FindControl(L"login_tip_label"));
		if (pWebLogMode)
		{
			pWebLogMode->SetText(SM_MODE_TIP);
		}
		if (m_pCefEnCharge)
		{
			wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
			wsUrl = PAY_URL + wsUrl;
			m_pCefEnCharge->LoadURL(wsUrl.c_str());
		}
		LOG_INFO(WEB_LOG, L"bind fee:" << wsTo);
	}
	if (m_bWXBind)
	{
		EnableCtrl(this, L"wx_btn", FALSE);
		SetCtrlText<ui::Button>(this, L"wx_btn", L"已登录");//微信登录
		SetCtrlText<ui::ButtonBox>(this, L"tip_btn", L"微信登录"); 
		EnableCtrl(this, L"tip_btn", FALSE);
		ClearButImage(this, L"tip_btn");
	}
	if (bCancel)
	{
		ui::VBox * pWxBox = dynamic_cast<ui::VBox*>(FindControl(L"wx_bind_wnd_box"));
		if (pWxBox)
			pWxBox->SetVisible(false);
		LOG_INFO(MAINLOG_DEF, L"" << wsTo);
		return RV_CANCEL;
	}

	return RV_CONTINUE;

}

CefRequestHandler::ReturnValue CMainFrameUI::OnBeforeNavigateReward(CefRefPtr<CefRequest> request, bool is_redirect)
{
	wstring wsNow = request->GetReferrerURL();
	wstring wsTo = request->GetURL();

	if (StrStrIW(wsTo.c_str(), REWARD_BACK))
	{
		HideCtrl(this, L"lottery_box");
		return RV_CANCEL;
	}
	else if (StrStrIW(wsTo.c_str(), REWARD_CDK))
	{
		wstring wsCDK;
		leigod_netpub::UnPraseCDK(wsTo, wsCDK);
		if (wsCDK.size())
		{
			wstring wsDeskPath = ui::PathUtil::GetDesktopDir();
			wsDeskPath += L"兑奖码.txt";
			string ssContent = W2U(wsCDK);
			leigod_netpub::LOG_FILE(wsDeskPath, ssContent);
		}
		return RV_CANCEL;
	}
	else if (StrStrIW(wsTo.c_str(), WEB_JUMP_HOME))
	{
		::ShellExecuteA(NULL, "open", "https://jiasu.nn.com/", "", NULL, SW_HIDE);
		return RV_CANCEL;
	}
	return RV_CONTINUE;
}

int64_t CMainFrameUI::m_nTimeCount = 0;

void TimeConverMorS(int tCount, ui::Label * pLbObj)
{
	std::wstring wstrTime;
	if (tCount < 10)
		wstrTime = L"0" + std::to_wstring(tCount);
	else
		wstrTime= std::to_wstring(tCount);

	pLbObj->SetText(wstrTime);
}

void TimeConverH(int tCount, ui::Label * pLbObj)
{
	std::wstring wstrTime;
	if (tCount < 10)
		wstrTime = L"000" + std::to_wstring(tCount);
	else if(10<=tCount&&tCount<100)
	{
		wstrTime = L"00" + std::to_wstring(tCount);
	}
	else if (100 <= tCount && tCount < 1000)
	{
		wstrTime = L"0" + std::to_wstring(tCount);
	}
	else 
	{
		wstrTime = std::to_wstring(tCount);
	}

	pLbObj->SetText(wstrTime);
}



void CALLBACK  CMainFrameUI::OnTimeCountConverse(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	int64_t nTimeH = 0;
	int64_t nTimeM = 0;
	int64_t nTimeS = 0;
	int64_t ntemp = 0;
	int64_t nTime=0;
	int nCurrentTime = 0;
	int64_t dwEndTime= GetTickCount();
	nCurrentTime = dwEndTime - m_dwStartTimeConverse;
	nTime = m_nTimeCount - nCurrentTime;

	if (nTime <= 0)
		KillTimer(gMainWnd->m_hWnd, 2);

	nTimeH = nTime / (3600*1000);
	TimeConverH(nTimeH, m_pLbHConver);
	ntemp = nTime %(3600*1000);
	nTimeM= ntemp/ (1000 * 60);
	TimeConverMorS(nTimeM,m_pLbMConver);
	ntemp= ntemp %(1000 * 60);
	nTimeS = ntemp / 1000;
	TimeConverMorS(nTimeS, m_pLbSConver);

	//if (m_nTimeCount == 0)
	//	KillTimer(gMainWnd->m_hWnd,2);
}
void CMainFrameUI::OnWebTimeArrive(int nHour, int nMin)
{
	m_pLbHConver = dynamic_cast<ui::Label *>(FindControl(L"time_h_label"));
	m_pLbMConver= dynamic_cast<ui::Label *>(FindControl(L"time_m_label"));
	m_pLbSConver= dynamic_cast<ui::Label *>(FindControl(L"time_s_label"));
	if (m_pLbHConver&&m_pLbMConver&&m_pLbSConver)
	{

		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

			int64_t nTimeM = 0;
			int64_t nTimeS = 0;
			nTimeM = nHour * 60;
			nTimeM += nMin;
			nTimeS = nTimeM * 60;
			m_nTimeCount = nTimeS * 1000;


			m_dwStartTimeConverse = GetTickCount();
			SetTimer(this->m_hWnd, 2, 150, OnTimeCountConverse);

		}));
	}
}

bool CMainFrameUI::ShowSignalGame(int nGameid)
{
	STGame stGame;

	if (LOGIC_CENTER()->FindGameOnID(nGameid, stGame))
	{
		if (stGame.nPlatID == 1)
		{
			ShowCtrl(this, L"current_game_stem_label");
		}
		else
		{
			HideCtrl(this, L"current_game_stem_label");
		}
	
			int index = 0;
			int spiltIndex = 0;
			WCHAR  *stopStr;
			STGame STGameGot;
			StringVector nameVct;
			StringVector idVct;
			std::wstring wstrGameId, gameServerName, gameName;
			std::wstring BigPhotoPath;

			ui::VBox * pMainBk = dynamic_cast<ui::VBox *>(FindControl(L"main_bk"));
			if (pMainBk)
				pMainBk->SetBkImage(L"bk_photo/bk_main.png");

			ui::Button * pCruentPhotoBtn = dynamic_cast<ui::Button*>(FindControl(L"cruuent_photo_btn"));
			if (pCruentPhotoBtn)
				pCruentPhotoBtn->SetStateImage(ui::kControlStateNormal, stGame.wsBgPath);

			ui::Label * pCruentGameName = dynamic_cast<ui::Label*>(FindControl(L"current_game_name_label"));
			if (pCruentGameName)
			{
				pCruentGameName->SetText(stGame.wsGameTitle);
			}
			else
			{
				return false;
			}

			for (const auto&p : m_mapXmlList)
			{
				std::wstring wstrShowSmOrBy = L"pay_and_advertiseing_box_sm";
				if (p.first == L"myspeed_btn")
				{
					p.second->SetVisible(true);
					continue;
				}
				if (!m_SmSelect)
				{
					wstrShowSmOrBy = L"pay_and_advertiseing_box_by";
				}
				if (p.first == wstrShowSmOrBy)
				{
					p.second->SetVisible(true);
					continue;
				}

				p.second->SetVisible(false);
			}

			if (!ShowSelected(m_nOldSelected, -1))
			{
				return false;
			}

			ui::VBox * pTipLabel = dynamic_cast<ui::VBox *>(FindControl(L"nick_window_box"));
			if (pTipLabel)
				pTipLabel->SetVisible(false);

			//---------------------- 业务逻辑部分 ------------------
			{
				int nGameID = stGame.nGameID;
				if (nGameID)
				{
					
					if (stGame.vctAreas.size())
					{
						OnGameSelect(nGameID);
						m_nGameID = nGameID;
						m_nAreaID = 0;

						if (LOGIC_CENTER()->IsLogined())
						{
							m_bWaitLogin = false;

							ui::EventArgs args;
							args.wParam = 0;
							args.lParam = -1;
							OnSvrSelected(&args);
							OnAreaSelect(stGame.vctAreas[0].nID);

						}
						else
						{
							m_bWaitLogin = true;
						}
					}

				}
			}		
	}
	return false;
}

//---------------------------------------------  logic ---------------------------------------------------
void CMainFrameUI::OnLazyInitResponse(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		if (ERRCODE_OK == nCode)
		{
			m_bInited = true;
			CreateGameElementList();
			CreateAreaElementList();

			SetCtrlText<ui::ButtonBox>(this, L"area_btnbox", LOGIC_CENTER()->GetGeoIP().wsProvince);
			m_wsProvinceName = LOGIC_CENTER()->GetGeoIP().wsProvince;
			SetCtrlText<ui::Label>(this, L"oper_isp_tip", stralgo::StringHelper::FormatW(L"运营商为 %s 使用'推荐'节点更快更稳定", U2W(LOGIC_CENTER()->GetGeoIP().ssIsp).c_str()));

		}

		if (m_SmSelect == 1)
		{
			//ShowSignalGame(41304);
			if (m_bWaitLogin)
			{
				ui::ButtonBox * pBtnProv = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
				if (pBtnProv)
				{
					wstring wsProvName = pBtnProv->GetText();
					nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
						LOGIC_CENTER()->GetLineGroup(m_nGameID, m_nAreaID, wsProvName);
					}));
				}
			}

		}
		else
		{
			//ShowSignalGame(41304);
		}

	}));

}

void CMainFrameUI::CallJsBack(const std::string & ss)
{

}

void CMainFrameUI::OnLoginPinResponse(int nCode)
{
	
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		if (ERRCODE_OK == nCode)
		{
			SetCtrlText<ui::ButtonBox>(this, L"area_btnbox", LOGIC_CENTER()->GetGeoIP().wsProvince);
			m_wsProvinceName = LOGIC_CENTER()->GetGeoIP().wsProvince;
			ShowSignalGame(41304);
			m_SmSelect = 1;
			//if (m_bWaitLogin)
			//{
			//	ui::ButtonBox * pBtnProv = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
			//	if (pBtnProv)
			//	{
			//		wstring wsProvName = pBtnProv->GetText();
			//		nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
			//			LOGIC_CENTER()->GetLineGroup(m_nGameID, m_nAreaID, wsProvName);
			//		}));
			//	}
			//}

			if (m_pCefEnCharge)
			{
				//wstring wsFun = LR"({ "evn": "clac","url":"ss" })";
				//m_pCefEnCharge->CallJSFunction(L"WebCall", wsFun
				//	, std::bind(&CMainFrameUI::CallJsBack, this, std::placeholders::_1));
				wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", 
	
					LOGIC_CENTER()->GetPinRes().nUserID, 

					LOGIC_CENTER()->GetPinRes().nSubUserID);
				wsUrl = PAY_URL + wsUrl;
				LOG_INFO(MAINLOG_DEF, L"pin login encharge: " << wsUrl);
				m_pCefEnCharge->LoadURL(wsUrl.c_str());
				m_pCefEnCharge->SetVisible(true);

			}

			SetCtrlText<ui::Label>(this, L"wb_name_label_sm", LOGIC_CENTER()->GetPinRes().wsNetPubTitle);
			SetCtrlText<ui::Label>(this, L"version_name_label_sm", L"扫码版");
			auto itrSM = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
			auto itrBY = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
			if (itrSM->second && itrBY->second)
			{
				
				itrBY->second->SetVisible(false);
				itrSM->second->SetVisible(true);
			}
		}
		else
		{
			::MessageBoxExW(GetHWND(), L"Pin登录失败，请联系服务提供商。", L"提示", MB_OK, 0);
		}

	}));
}

void CMainFrameUI::OnPinImageArrive(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		STSvrSet & rfSvrSet = LOGIC_CENTER()->GetSvrSetInfo();
		for (int i = 0; i < 2; i++)
		{
			ui::Button * pBut = dynamic_cast<ui::Button*>(FindControl(stralgo::StringHelper::FormatW(L"sm_g%d", i + 1)));
			if (pBut && leigod_netpub::ImageValid(rfSvrSet.wsPngPathSM[i]))
			{
				pBut->SetStateImage(ui::kControlStateNormal, rfSvrSet.wsPngPathSM[i]);
				//wstring wsHoverStatus = stralgo::StringHelper::FormatW(L"file = '%s' fade = '128'", rfSvrSet.wsPngPathSM[i].c_str());
				//pBut->SetStateImage(ui::kControlStateHot, wsHoverStatus);
				//wstring wsPushStatus = stralgo::StringHelper::FormatW(L"file = '%s' fade = '178'", rfSvrSet.wsPngPathSM[i].c_str());
				//pBut->SetStateImage(ui::kControlStatePushed, wsPushStatus);
			}
		}
	}));
	//STSvrSet & GetSvrSetInfo() { return m_stSvrSet; }
}

void CMainFrameUI::OnKeyImageArrive(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		STSvrSet & rfSvrSet = LOGIC_CENTER()->GetSvrSetInfo();
		for (int i = 0; i < 2; i++)
		{
			ui::Button * pBut = dynamic_cast<ui::Button*>(FindControl(stralgo::StringHelper::FormatW(L"by_g%d", i + 1)));
			if (pBut && leigod_netpub::ImageValid(rfSvrSet.wsPngPathBY[i]))
			{
				pBut->SetBkImage(rfSvrSet.wsPngPathBY[i]);
			}
		}
	}));
}

void CMainFrameUI::OnLoginKeyResponse(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		if (ERRCODE_OK == nCode)
		{
			SetCtrlText<ui::ButtonBox>(this, L"area_btnbox", LOGIC_CENTER()->GetGeoIP().wsProvince);
			m_wsProvinceName = LOGIC_CENTER()->GetGeoIP().wsProvince;
			ShowSignalGame(41304);
			m_SmSelect = 0;
			bool bExpired(false);
			if (LOGIC_CENTER()->GetPinRes().nMinuteLeft <= 0)
			{
				bExpired = true;
			}
			else
			{
				int64_t nMin = LOGIC_CENTER()->GetPinRes().nMinuteLeft;
				m_nTimeCount = nMin * 60 * 1000;
			}
			
			if (m_bWaitLogin && !bExpired)
			{
				ui::ButtonBox * pBtnProv = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btnbox"));
				if (pBtnProv)
				{
					wstring wsProvName = pBtnProv->GetText();
					nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
						LOGIC_CENTER()->GetLineGroup(m_nGameID, m_nAreaID, wsProvName);
					}));
				}
			}

			if (m_pCefEnCharge && bExpired)
			{
				wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d", LOGIC_CENTER()->GetPinRes().nUserID, LOGIC_CENTER()->GetPinRes().nSubUserID);
				wsUrl = PAY_URL + wsUrl;
				LOG_INFO(MAINLOG_DEF, L"key login encharge: " << wsUrl);
				m_pCefEnCharge->LoadURL(wsUrl.c_str());
				m_pCefEnCharge->SetVisible(true);

			}

			if (bExpired)
			{
				m_SmSelect = 1;
				SetCtrlText<ui::Label>(this, L"wb_name_label_sm", LOGIC_CENTER()->GetPinRes().wsNetPubTitle);
				SetCtrlText<ui::Label>(this, L"version_name_label_sm", L"扫码版");
				auto itrSM = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
				auto itrBY = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
				if (itrSM->second && itrBY->second)
				{

					itrBY->second->SetVisible(false);
					itrSM->second->SetVisible(true);
				}
			}
			else
			{
				SetCtrlText<ui::Label>(this, L"wb_name_label_by", LOGIC_CENTER()->GetPinRes().wsNetPubTitle);
				SetCtrlText<ui::Label>(this, L"version_name_label_by", L"包月版");
				wstring wsExpireTime = L"到期时间：";
				wsExpireTime += LOGIC_CENTER()->GetPinRes().wsExpireTime.size() ? LOGIC_CENTER()->GetPinRes().wsExpireTime : LOGIC_CENTER()->GetPinRes().wsExpireData;
				SetCtrlText<ui::Label>(this, L"time_game", wsExpireTime);

				auto itrSM = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
				auto itrBY = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
				if (itrSM->second && itrBY->second)
				{
					itrSM->second->SetVisible(false);
					itrBY->second->SetVisible(true);

				}
			}



		}
		else
		{
			auto itrSM = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
			auto itrBY = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
			if (itrSM->second && itrBY->second)
			{
				itrSM->second->SetVisible(false);
				itrBY->second->SetVisible(true);

			}
			::MessageBoxExW(GetHWND(), L"Key登录失败，请联系服务提供商。", L"提示", MB_OK, 0);
		}

	}));
}

void CMainFrameUI::OnGetGroupLineResponse(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		//OnLineGroupResponse(m_nGameID, m_nAreaID);   fxf
		if (m_operVbox)
			m_operVbox->CreateOperatorElementList(m_nGameID, m_nAreaID);
	}));
	
	//STLineGroupStrore & groupData =	LOGIC_CENTER()->FindLineGroup(42204, 1181);
}

void CMainFrameUI::OnDelayResponse(int nGameID, int nAreaID)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

		vector<STLineItem> vctTemp;
		STLineGroupStrore & rfData = LOGIC_CENTER()->FindLineGroup(nGameID, nAreaID);
		for (STGroupItem & itr : rfData.stData.vctGroups)
		{
			for (STLineItem & itrl : itr.vctGroupLines)
			{
				ui::ListContainerElement * pElem = dynamic_cast<ui::ListContainerElement *>(FindControl(stralgo::StringHelper::FormatW(L"line_%d_%d", itrl.nLineID, itrl.nIspType)));
				if (pElem)
				{
					ui::Label * pLbDelay = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"local_delay"));
					if (pLbDelay)
					{
						pLbDelay->SetText(stralgo::StringHelper::FormatW(L"本地延迟: %dms", itrl.nDelay));
					}
					vctTemp.push_back(itrl);
				}
			}
		}

		std::sort(vctTemp.begin(), vctTemp.end(), [&](const STLineItem & left, const STLineItem & right) {return left.nDelay < right.nDelay; });

		ClearListCtrl(FindControl(L"operator_listbox_recommend"));

		ui::ListBox * pMainOperatorListBox = dynamic_cast<ui::ListBox*>(FindControl(L"operator_listbox_recommend"));
		if (pMainOperatorListBox)
		{
			for (int i = 0; i < vctTemp.size() && i < 6; i++)
			{
				ui::ListContainerElement * pElem = buildSubListContainerElement(L"leigod/model/operator_model_box.xml");
				if (pElem)
				{
					ui::Control * pGameLogo = FindSubControlByName(pElem, L"game_logo");
					if (pGameLogo)
					{
						pGameLogo->SetBkImage(Get_Isp_logo(vctTemp[i].nIspType));
					}
					pElem->SetName(stralgo::StringHelper::FormatW(L"line_%d_%d", vctTemp[i].nLineID, vctTemp[i].nIspType));
					ui::Label * pLbDelay = dynamic_cast<ui::Label*>(FindSubControlByName(pElem, L"local_delay"));
					if (pLbDelay)
					{
						pLbDelay->SetText(stralgo::StringHelper::FormatW(L"本地延迟: %dms", vctTemp[i].nDelay));
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

	}));
}

void CMainFrameUI::OnGetAccInfoResponse(int nCode)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {
		CreateTipMsgBox(L"获取加速信息失败！");
	}));
}

void CMainFrameUI::OnEngineLog(int flag, string ssGbk)
{
	ui::RichEdit * pEditLog = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_aoo_log"));
	if (pEditLog)
	{
		pEditLog->AppendText(stralgo::StringHelper::FormatW(L"%d-%s\r\n", flag, AS2W(ssGbk).c_str()));
	}
}

void CMainFrameUI::OnExpireed(int nSrc)
{
	if (m_pCefEnCharge)
	{

		wstring wsUrl = stralgo::StringHelper::FormatW(L"?userid=%d&subuserid=%d",
			LOGIC_CENTER()->GetPinRes().nUserID,
			LOGIC_CENTER()->GetPinRes().nSubUserID);
		wsUrl = PAY_URL + wsUrl;
		LOG_INFO(MAINLOG_DEF, L"pin login encharge: " << wsUrl);
		m_pCefEnCharge->LoadURL(wsUrl.c_str());
		m_pCefEnCharge->SetVisible(true);
	}
	m_SmSelect = 1;
	SetCtrlText<ui::Label>(this, L"wb_name_label_sm", LOGIC_CENTER()->GetPinRes().wsNetPubTitle);
	SetCtrlText<ui::Label>(this, L"version_name_label_sm", L"扫码版");
	auto itrSM = m_mapXmlList.find(L"pay_and_advertiseing_box_sm");
	auto itrBY = m_mapXmlList.find(L"pay_and_advertiseing_box_by");
	if (itrSM->second && itrBY->second)
	{

		itrBY->second->SetVisible(false);
		itrSM->second->SetVisible(true);
	}
	CreateTipMsgBox(L"该网吧购买的加速服务已到期，客户端将转成扫码版\r\n扫码购买后可继续使用");
}


int TimeCount(int num,ui::Label *pLbTime)
{
	std::wstring wstrShow;
	if (num < 10)
		wstrShow = L"0" + std::to_wstring(num);
	else
		wstrShow = std::to_wstring(num);

	if (pLbTime)
		pLbTime->SetText(wstrShow);
	return false;
}

void CALLBACK CMainFrameUI::OnTimeCount(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	static int nTimeH = 0;
	static int nTimeM = 0;
	static int nTimeS = 0;
	DWORD dwEndTime= GetTickCount();
	DWORD dwCrruent = 0;
	DWORD dwTemp = 0;
	DWORD dwCrruentH = 0;
	DWORD dwCrruentM = 0;
	DWORD dwCrruentS = 0;

	std::wstring wstrShow;
	dwCrruent = dwEndTime - m_dwStartTime;
	

	dwCrruentH=dwCrruent/ (1000 * 3600);
		TimeCount(dwCrruentH, m_pLbH);
	dwTemp = dwCrruent % (1000 * 3600);
	dwCrruentM = dwTemp / (1000 * 60);
		TimeCount(dwCrruentM, m_pLbM);
	dwTemp = dwTemp % (1000 * 60);
	dwCrruentS = dwTemp / 1000;
		TimeCount(dwCrruentS, m_pLbS);
	
}



void CMainFrameUI::OnEnginAccResult(int nCode, string ssMsg)
{
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

		m_pLbH = dynamic_cast<ui::Label *>(FindControl(L"game_h_time_label"));
		m_pLbM = dynamic_cast<ui::Label *>(FindControl(L"game_m_time_label"));
		m_pLbS = dynamic_cast<ui::Label *>(FindControl(L"game_s_time_label"));
		if (m_pLbH&&m_pLbS&&m_pLbS)
		{

			if (nCode == 2)
			{
				m_nIsArccrate = 2;
				m_nControlArccrate = 2;
				OverCount = 0;
				m_pLbH->SetText(L"00");
				m_pLbM->SetText(L"00");
				m_pLbS->SetText(L"00");
				m_dwStartTime= GetTickCount();
				SetTimer(this->m_hWnd,1,150,OnTimeCount);
			}
			else
			{
				m_nControlArccrate = 3;
				KillTimer(this->m_hWnd,1);
			}
		}
		else
			return;
	}));
}

void CMainFrameUI::SetArccrateNumbColor(uint32_t nFlush_, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate,std::wstring ColorName)
{
	int64_t nFlush = nFlush_;
	nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

		if (m_pLbH&&m_pLbM&&m_pLbS)
		{
			m_pLbH->SetText(L"00");
			m_pLbM->SetText(L"00");
			m_pLbS->SetText(L"00");
		}

		ui::Label * pLbFlush = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_flush_mb"));
		if (pLbFlush)
		{
			pLbFlush->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
			if (nFlush)
			{
				double dbFlush = (nFlush / (1024.0 * 1024.0 * 1.0));
				if (dbFlush > 1024.0)
				{
					dbFlush = (dbFlush / 1024.0);
				}
				pLbFlush->SetText(stralgo::StringHelper::FormatW(L"%.2f", dbFlush));
			}
				
			else
				pLbFlush->SetText(L"00");
		}

		ui::Label * pLbFlushMs = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_flush_ms"));
		if (pLbFlushMs)
		{
			pLbFlushMs->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
			if (nFlush > (1024 * 1024 * 1024))
			{
				pLbFlushMs->SetText(L"G");
			}
			else
			{
				pLbFlushMs->SetText(L"M");
			}
		}

		ui::Label * pLbDelay = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_delay"));
		if (pLbDelay)
		{
			pLbDelay->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
			if(nDelay)
				pLbDelay->SetText(std::to_wstring(nDelay));
			else
				pLbDelay->SetText(L"00");
		}
		ui::Label * pLbDelayMs = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_delay_ms"));
		{
			pLbDelayMs->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
		}

		ui::Label * pLbImprove = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_improve"));
		if (pLbImprove)
		{
			pLbImprove->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
			if(nDelay)
				pLbImprove->SetText(std::to_wstring(nAccRate));
			else
				pLbImprove->SetText(L"00");
		}
		ui::Label * pLbImprovePercent = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_improve_percent"));
		if (pLbImprovePercent)
		{
			pLbImprovePercent->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
		}
		
		ui::Label * pLbLoss = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_loss"));
		if (pLbLoss)
		{
			pLbLoss->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
			if(nDelay)
				pLbLoss->SetText(std::to_wstring(nLossRate));
			else
				pLbLoss->SetText(L"00");
		}
		ui::Label * pLbLossPercent = dynamic_cast<ui::Label *>(FindControl(L"acc_engin_loss_percent"));
		if (pLbLossPercent)
		{
			pLbLossPercent->SetStateTextColor(ui::ControlStateType::kControlStateNormal, ColorName);
		}

	}));
}

void CMainFrameUI::ShowRewardPage()
{
	//LOGIC_CENTER()->GetPinRes().nUserID
	//if (LOGIC_CENTER()->GetPinRes().nSubUserID)
	{
		int * ptr = &LOGIC_CENTER()->GetPinRes().nSubUserID;
		wstring wsUrl = stralgo::StringHelper::FormatW(L"%s?subuserid=%d", REWARD_URL, LOGIC_CENTER()->GetPinRes().nSubUserID);
		if (LOGIC_CENTER()->GetPinRes().nWxID.size())
		{
			wsUrl += L"&openid=" + (LOGIC_CENTER()->GetPinRes().nWxID);
		}
		//if (LOGIC_CENTER()->GetSvrSetInfo().nActiveID)
		{
			wsUrl += L"&actid=";
			wsUrl += std::to_wstring(LOGIC_CENTER()->GetSvrSetInfo().nActiveID);
			if (m_pCefReward)
			{
				m_pCefReward->LoadURL(wsUrl.c_str());
				m_pCefReward->SetVisible(true);
				LOG_INFO(MAINLOG_DEF, L"choujiang:" << wsUrl);
			}
		}

		ShowCtrl(this, L"lottery_box");
	}

}

string GetGateWay();
void CMainFrameUI::OnEnginData(uint32_t nFlush, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate)
{
	if (gListBarControl.size() == 500)
	{
		gListBarControl.pop_front();
	}
	gListBarControl.push_front(nDelay);

	//m_nControlArccrate = 2;
	
	LOG_INFO(MAINLOG_DEF, L"OnEnginData:" << nFlush << L": " << nDelay);
	SetArccrateNumbColor(nFlush, nDelay, nAccRate, nLossRate, L"set_arccrate_numb_color");

	auto pParentMonitor = m_mapXmlParentBox.find(L"monitor_box");
	if (pParentMonitor->second && pParentMonitor->second->IsVisible())
	{
		{
			int nLoss(0);

			uint32_t nGateDelay = leigod_netpub::GET_AVG_DELAY(GetGateWay(), 4, &nLoss);
			ui::Label * plbGateDelay = dynamic_cast<ui::Label*>(FindControl(L"lb_gate_delay"));
			if (plbGateDelay)
			{
				plbGateDelay->SetText(stralgo::StringHelper::FormatW(L"%uMS", nGateDelay));
			}

			ui::Label * plbGateLoss = dynamic_cast<ui::Label*>(FindControl(L"lb_gate_loss"));
			if (plbGateLoss)
			{
				plbGateLoss->SetText(stralgo::StringHelper::FormatW(L"%d%%", nLoss * 25));
			}

			SetCtrlText<ui::Label>(this, L"lb_mn_total_delay", std::to_wstring(nGateDelay + nDelay));
			SetCtrlText<ui::Label>(this, L"lb_mn_total_loss", std::to_wstring(nLoss + nLossRate));
		}

		{
			ui::Label * plbOutSvrDelay = dynamic_cast<ui::Label*>(FindControl(L"lb_outsvr_delay"));
			if (plbOutSvrDelay)
			{
				plbOutSvrDelay->SetText(stralgo::StringHelper::FormatW(L"%uMS", nDelay));
			}
			
			ui::Label * plbGateLoss = dynamic_cast<ui::Label*>(FindControl(L"lb_outsvr_loss"));
			if (plbGateLoss)
			{
				plbGateLoss->SetText(stralgo::StringHelper::FormatW(L"%d%%", nLossRate));
			}
			
		}

	}
}

void SetLableText(ui::WindowImplBase * pWnd, wstring wsCtrlID, wstring wsText)
{
	ui::Label * pLbCtrl = dynamic_cast<ui::Label *>(pWnd->FindControl(wsCtrlID));
	if (pLbCtrl)
	{
		pLbCtrl->SetText(wsText);
	}
}

void ShowCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID)
{
	auto itr = pWnd->FindControl(wsCtrlID);
	if (itr)
	{
		itr->SetVisible(true);
	}
}

void HideCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID)
{
	auto itr = pWnd->FindControl(wsCtrlID);
	if (itr)
	{
		itr->SetVisible(false);
	}
}

void ShowCtrl(ui::WindowImplBase * pWnd, ui::Control * pParent, wstring wsCtrlID)
{
	auto itr = pWnd->FindSubControlByName(pParent, wsCtrlID);
	if (itr)
	{
		itr->SetVisible(true);
	}
}

void HideCtrl(ui::WindowImplBase * pWnd, ui::Control * pParent, wstring wsCtrlID)
{
	auto itr = pWnd->FindSubControlByName(pParent, wsCtrlID);
	if (itr)
	{
		itr->SetVisible(false);
	}
}

void EnableCtrl(ui::WindowImplBase * pWnd, wstring wsCtrlID, BOOL bEnable)
{
	auto itr = pWnd->FindControl(wsCtrlID);
	if (itr)
	{
		itr->SetEnabled(bEnable);
	}
}

void ClearButImage(ui::WindowImplBase * pWnd, wstring wsCtrlID)
{
	ui::ButtonBox * pBtn = dynamic_cast<ui::ButtonBox *>(pWnd->FindControl(wsCtrlID)) ;
	if (pBtn)
	{
		pBtn->SetStateImage(ui::kControlStateNormal, L"");
		pBtn->SetStateImage(ui::kControlStateHot, L"");
		pBtn->SetStateImage(ui::kControlStatePushed, L"");
		pBtn->SetStateImage(ui::kControlStateDisabled, L"");

	}
}

wstring Get_Isp_logo(int nIspType)
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
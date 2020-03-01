#include "stdafx.h"
#include "CreateXmlObj.h"
#include "StringHelper.h"
#include "CAccountLogin.h"
#include "ui_components/ui_components.h"


CreateXmlObj::CreateXmlObj()
{
	BH_LOGIC_CENTER()->initLogin();
} 

CreateXmlObj::~CreateXmlObj()
{

}


bool CreateXmlObj::InitCreate()
{
	if (!AddXml())
	{
		return false;
	}
	BindClick();
	//AddGameBox();
	BindCef();
	CreateGameElementList();
	m_downloadVBox->CreateElementList(0,0);
	return true;
}

bool CreateXmlObj::AddXml()
{
	if (!AddXmlToXmlVct(BH_NAV_TAB_XML, L"nav_bar_vb", L"bh/bh_nav_tabpage_xml/nav_myspeed_tbpage.xml", true))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"tabpage_vb", L"bh/bh_tabpage_xml/myspeed_tbpage.xml", true))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"tabpage_vb", L"bh/bh_tabpage_xml/plat_tabpage.xml", false))
	{
		return false;
	}
 
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"select_svr_area_node_vb", L"bh/bh_floatframe_xml/svr_float.xml",false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"select_svr_area_node_vb", L"bh/bh_floatframe_xml/area_float.xml",false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"select_svr_area_node_vb", L"bh/bh_floatframe_xml/node_float.xml",false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"set_game_path_vb", L"bh/bh_floatframe_xml/set_game_path_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"monitor_vb", L"bh/bh_floatframe_xml/monitor_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"monitor_net_log_vb", L"bh/bh_tabpage_xml/monitor_net_tabpage.xml", true))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"monitor_net_log_vb", L"bh/bh_tabpage_xml/monitor_log_tabpage.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"arccrate_tool_vb", L"bh/bh_floatframe_xml/arccrate_tool_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"system_set_vb", L"bh/bh_floatframe_xml/system_set_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"set_tabpage_vb", L"bh/bh_tabpage_xml/system_general_set_tabpage.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"set_tabpage_vb", L"bh/bh_tabpage_xml/system_arccrate_set_tabpage.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_TAB_XML, L"set_tabpage_vb", L"bh/bh_tabpage_xml/system_down_set_tabpage.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"nick_vb", L"bh/bh_floatframe_xml/nick_float.xml", false))
	{
		return false;
	}

	if (!AddXmlToXmlVct(BH_NAV_TAB_XML, L"nav_bar_vb", L"bh/bh_nav_tabpage_xml/nav_download_tbpage.xml",false))
	{
		return false;
	}

	if (!AddXmlToXmlVct(BH_TAB_XML, L"download_tabpage_vbox", L"bh/bh_tabpage_xml/download_tabpage.xml", true))
	{
		return false;
	}

	/*
	if (!AddXmlToXmlVct(BH_TAB_XML, L"download_null_vb", L"bh/bh_list_mode_xml/download_null_mode.xml", false))
	{
		return false;
	}
	*/

	if (!AddXmlToXmlVct(BH_NAV_TAB_XML, L"nav_bar_vb", L"bh/bh_nav_tabpage_xml/nav_content_arccrate_tbpage.xml",false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"content_tip_vb", L"bh/bh_floatframe_xml/content_tip_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_NAV_TAB_XML, L"nav_bar_vb", L"bh/bh_nav_tabpage_xml/nav_notice_tbpage.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"update_tip_vb", L"bh/bh_floatframe_xml/update_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"login_vb", L"bh/bh_floatframe_xml/login_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"menu_vb", L"bh/bh_floatframe_xml/menu_float.xml", false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"child_login_regedit_vb", L"bh/bh_floatframe_xml/child_login_float.xml", true))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"child_login_regedit_vb", L"bh/bh_floatframe_xml/child_regedit_float.xml",false))
	{
		return false;
	}
	if (!AddXmlToXmlVct(BH_FLOAT_XML, L"thridparty_vb", L"bh/bh_floatframe_xml/thridparty_float.xml", true))
	{
		return false;
	}
	return true;
}


bool CreateXmlObj::AddXmlToXmlVct(int xmlType,std::wstring parentBox, std::wstring xmlName, bool isShow)
{
	StringVector vctXml;
	StringVector vctName;
	int vctSize = 0;
	vctSize = stralgo::StringHelper::SplitString(xmlName, vctXml, L"/");
	vctSize = stralgo::StringHelper::SplitString(vctXml[vctSize - 1], vctName, L".");

	ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(parentBox));
	if (pBox)
	{
		
		
		ui::VBox * pViewXml = buildSubView(xmlName.c_str());
		if (pViewXml)
		{
			switch (xmlType)
			{
				case BH_FLOAT_XML:
				{
					m_mapFloatParentVb.insert(Insert_Pair(parentBox, pBox));
					m_mapFloatXmlList.insert(Insert_Pair(vctName[0], pViewXml));	
				}
				break;
				case BH_TAB_XML:
				{
					m_mapTabParentVb.insert(Insert_Pair(parentBox, pBox));
					m_mapTabXmlList.insert(Insert_Pair(vctName[0], pViewXml));
				}
				break;
				case BH_NAV_TAB_XML:
				{
					m_mapNavTabParentVb.insert(Insert_Pair(parentBox, pBox));
					m_mapNavTabXmlList.insert(Insert_Pair(vctName[0], pViewXml));
				}
				break;
				default:
					break;
			}
			OutputDebugString(wstring(L"AddXmlToXmlVct   ").append(parentBox).c_str());
			pBox->Add(pViewXml);
			pViewXml->SetVisible(isShow);
			
			  
			return true;
		}
	}
	return false;
}

ui::VBox * CreateXmlObj::buildSubView(const wchar_t * xmlName)
{

	StringVector vctXml;
	StringVector vctName;
	int vctSize = 0;
	vctSize = stralgo::StringHelper::SplitString(xmlName, vctXml, L"/");
	vctSize = stralgo::StringHelper::SplitString(vctXml[vctSize - 1], vctName, L".");

	ui::VBox * item = NULL;
	std::wstring  areacontrol = L"area_btnbox";
	std::wstring  servercontrol = L"server_btnbox";
	std::wstring  operatorcontrol = L"operator_btnbox";
	std::wstring  searchcontrol = L"search_list";
	std::wstring  cefcontrol = L"cef_loadbox";
	std::wstring  downloadcontrol = L"download_tabpage";

	auto callback = nbase::Bind(&WindowImplBase::CreateControl, this, std::placeholders::_1);
	 
 
	if (downloadcontrol.compare(vctName[0]) == 0) {
		OutputDebugString(wstring(L"create CDownloadVBox      ").append(xmlName).c_str());
		item = new   nui::CDownloadVBox(this);
		m_downloadVBox = dynamic_cast<nui::CDownloadVBox*>(item);
	}
	else {
		item = new ui::VBox;
	}
 
 
	ui::GlobalManager::FillBoxWithCache(item, xmlName,callback);
	return item;
}

ui::ListContainerElement * CreateXmlObj::buildSubListContainerElement(const wchar_t * xmlName)
{
	ui::ListContainerElement * item = new ui::ListContainerElement;
	ui::GlobalManager::FillBoxWithCache(item, xmlName);
	return item;
}

void CreateXmlObj::BindClick()
{
	std::wstring navBtnArry[] = { L"main_myspeed_btn",L"game_download_btn",L"content_arccrate_btn",L"main_recharge_btn",L"bh_notice_btn"};
	std::wstring mySpeedTabBtnArry[] = {L"myspeed_btnb",L"hot_btnb" ,L"often_btnb" ,L"all_btnb" ,L"for_btnb",L"chnn_btnb",L"free_btnb"};
	std::wstring NodeBtnArry[] = { L"svr_btn",L"area_btn" ,L"node_btn" ,L"mode_btn"};
	std::wstring DialogCloseBtnArry[] = { L"login_close_btn",L"server_close_btn",L"monitor_close_btn",L"set_path_close_btn",L"thrid_close_btn"};
	std::wstring LoginBtnArry[] = {L"main_login_btn",L"reg_btn",L"go_login_btn"}; 
	std::wstring MonitorArry[] = {L"monitor_btn",L"monitor_net_btnb",L"monitor_log_btnb"};
	std::wstring GameOperationArry[] = { L"get_game_path_btn",L"game_path_dialog_btn" };
	std::wstring MainPageBtnArry[] = { L"main_game_btn" };
	std::wstring ThridLoginBtnArry[] = {L"QQ_login_btn",L"WX_login_btn",L"WB_login_btn"};
	for (std::wstring &navBtnName : navBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnNavTabClick, this, std::placeholders::_1);
		BindBtn(NAV_BTN,navBtnName, OnBack);
	}
	for (std::wstring &mySpeedBtnName : mySpeedTabBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnTabClick, this, std::placeholders::_1);
		BindBtn(MYSPEED_BTN,mySpeedBtnName, OnBack);
	}
	for (std::wstring &CloseBtnName : DialogCloseBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnDialogCloseClick, this, std::placeholders::_1);
		BindBtn(0,CloseBtnName, OnBack);
	}
	for (std::wstring &LoginBtnName : LoginBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnLoginClick, this, std::placeholders::_1);
		BindBtn(0,LoginBtnName, OnBack);
	}
	for (std::wstring &NodeBtnName : NodeBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnNodeClick, this, std::placeholders::_1);
		BindBtn(0, NodeBtnName, OnBack);
	}
	for (std::wstring &NodeBtnName : MonitorArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnMonitorClick, this, std::placeholders::_1);
		BindBtn(0, NodeBtnName, OnBack);
	}
	for (std::wstring &GameOperationBtnName : GameOperationArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnGamePathClick, this, std::placeholders::_1);
		BindBtn(0, GameOperationBtnName, OnBack);
	}
	for (std::wstring &MainBtnName : MainPageBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnMainPageBtnClick, this, std::placeholders::_1);
		BindBtn(0, MainBtnName, OnBack);
	}
	for (std::wstring &ThridBtnName : ThridLoginBtnArry)
	{
		OnCallBackFunction OnBack = nbase::Bind(&CreateXmlObj::OnThridLoginClick, this, std::placeholders::_1);
		BindBtn(0, ThridBtnName, OnBack);
	}
}

void CreateXmlObj::BindEvent()
{

	ui::RichEdit * pEditPhoneNumber= dynamic_cast<ui::RichEdit *>(FindControl(L"phonenumber_edit"));
	if (pEditPhoneNumber)
	{
		pEditPhoneNumber->AttachTextChange(nbase::Bind(&CreateXmlObj::OnPhoneNumberTextChangeEvent, this, std::placeholders::_1));
		pEditPhoneNumber->AttachKillFocus(nbase::Bind(&CreateXmlObj::OnPhoneNumberKillFocusEvent, this, std::placeholders::_1));
		pEditPhoneNumber->AttachSetFocus(nbase::Bind(&CreateXmlObj::OnPhoneNumberGetFocusEvent, this, std::placeholders::_1));
	}
	ui::RichEdit * pEditPwd = dynamic_cast<ui::RichEdit *>(FindControl(L"pwd_edit"));
	if (pEditPwd)
	{
		pEditPwd->AttachTextChange(nbase::Bind(&CreateXmlObj::OnPwdTextChangeEvent, this, std::placeholders::_1));
		pEditPwd->AttachKillFocus(nbase::Bind(&CreateXmlObj::OnPwdKillFocusEvent, this, std::placeholders::_1));
		pEditPwd->AttachSetFocus(nbase::Bind(&CreateXmlObj::OnPwdGetFocusEvent, this, std::placeholders::_1));
	}
}

void CreateXmlObj::BindBtn(int btnType,std::wstring btnName,OnCallBackFunction pOnFun)
{
	Navbtn btn;

	ui::ButtonBox * pBtnBox = dynamic_cast<ui::ButtonBox *>(FindControl(btnName));
	if (pBtnBox)
	{
		pBtnBox->AttachClick(pOnFun);
		btn.btnName = btnName;
		btn.pBtn = pBtnBox;
		switch (btnType)
		{
			case NAV_BTN:
				m_btnVct.push_back(btn);
				break;
			case MYSPEED_BTN:
				m_mySpeedTabVct.push_back(btn);
				break;
			default:
				break;
		}
	}
	else
	{
		ui::Button * pBtn = dynamic_cast<ui::Button *>(FindControl(btnName));
		if (pBtn)
		{
			pBtn->AttachClick(pOnFun);
			btn.btnName = btnName;
			btn.pBtn = pBtn;
			switch (btnType)
			{
			case NAV_BTN:
				m_btnVct.push_back(btn);
				break;
			case MYSPEED_BTN:
				m_mySpeedTabVct.push_back(btn);
				break;
			default:
				break;
			}
		}
	}
		
}

void CreateXmlObj::BindCef()
{
	m_ThridCef = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_thrid"));
	if (m_ThridCef)
	{
		//m_ThridCef->AttachBeforeNavigate();
	}
}
bool CreateXmlObj::AddGameBox()
{
	GameMode mode;
	ui::ListBox * pAllGameListBox = dynamic_cast<ui::ListBox*>(FindControl(L"all_game_tlb"));
	int i = 0;
	for (; i < 10; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/game_mode.xml");
		if (ElementBox)
		{
			pAllGameListBox->Add(ElementBox);
			mode.GameElement = ElementBox;
			m_GameVct.push_back(mode);
			ElementBox->SetName(std::to_wstring(i));
			ui::Label * pGameLogo = dynamic_cast<ui::Label *>(FindControl(L"game_logo"));
			pGameLogo->SetBkImage(L"D:\\W公司项目\\new_bh_pc\\leigod_netbar_luo\\bin\\Debug\\resources\\themes\\default\\bh\\test_pic\\t1.png");
			ui::Label * pGameName = dynamic_cast<ui::Label *>(FindControl(L"game_name"));
			pGameName->SetText(L"test1");
			ElementBox->AttachSelect(nbase::Bind(&CreateXmlObj::OnGameSelect, this, std::placeholders::_1));

			ui::Label * pGameServer = dynamic_cast<ui::Label *>(FindControl(L"game_server"));
			pGameServer->SetText(L"test1");
			ElementBox->SetClass(L"listitem");

		}
	}
	return true;
}

bool CreateXmlObj::AddNickBox()
{
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"history_nick_text_vlb"));
	int i = 0;
	for (; i < 500; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/nick_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
			ui::Label * pGameName = dynamic_cast<ui::Label *>(FindControl(L"game_name_lb"));
			//ElementBox->SetClass(L"listitem");
			pGameName->SetText(L"薄荷测试1");
		}
	}
	i = 0;
	ui::ListBox * p2 = dynamic_cast<ui::ListBox*>(FindControl(L"hot_nick_vlb"));
	for (; i < 500; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/nick_mode.xml");
		if (ElementBox)
		{
			p2->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
			ui::Label * pGameName = dynamic_cast<ui::Label *>(FindControl(L"game_name_lb"));
			//ElementBox->SetClass(L"listitem");
			pGameName->SetText(L"薄荷测试2");
			pGameName->AttachButtonUp(nbase::Bind(&CreateXmlObj::OnNickSelect, this, std::placeholders::_1));
			//ElementBox->AttachClick(nbase::Bind(&CreateXmlObj::OnNickSelect, this, std::placeholders::_1));
			//ElementBox->AttachSelect(nbase::Bind(&CreateXmlObj::OnNickSelect, this, std::placeholders::_1));
			//ElementBox->AttachButtonDown(nbase::Bind(&CreateXmlObj::OnNickSelect, this, std::placeholders::_1));
		}
	}
	return false;
}

bool CreateXmlObj::AddDownLoadBox()
{
	/*
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"download_game_vlb"));
	int i = 0;
	for (; i < 20; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/download_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
		}
	}
	*/
	return true;
}

void CreateXmlObj::OnAreaSelect(int nArea)
{
	if (m_nAreaID != nArea)
	{
		m_nAreaID = nArea;
		ui::ButtonBox * pBtnProv = dynamic_cast<ui::ButtonBox *>(FindControl(L"area_btn"));
		if (pBtnProv)
		{
			wstring wsProvName = pBtnProv->GetText();
			//nbase::ThreadManager::PostTask(kThreadDown, ToWeakCallback([=]() {
			//	LOGIC_CENTER()->GetLineGroup(m_nGameID, nArea, wsProvName);
			//}));
		}
	}

}
/*
bool CreateXmlObj::AddSvrBox()
{
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"svr_tlb"));
	int i = 0;
	for (; i < 20; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/server_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
		}
	}
	return true;
}
*/

bool CreateXmlObj::AddAreaBox()
{
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"area_tlb"));
	int i = 0;
	for (; i < 20; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/area_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
		}
	}
	return true;
}

bool CreateXmlObj::AddNodeBox()
{
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"operator_listbox_recommend"));
	int i = 0;
	for (; i < 20; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/node_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
		}
	}
	return true;
}


bool CreateXmlObj::AddNoticeText()
{
	ui::ListBox * p1 = dynamic_cast<ui::ListBox*>(FindControl(L"notice_vlb"));
	int i = 0;
	for (; i < 20; i++)
	{
		ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"bh/bh_list_mode_xml/notice_mode.xml");
		if (ElementBox)
		{
			p1->Add(ElementBox);
			ElementBox->SetName(std::to_wstring(i));
		}
	}
	return true;
}
ui::Control* CreateXmlObj::CreateControl(const std::wstring& pstrClass)
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

void CreateXmlObj::CreateGameElement(ui::ListBox * gameList, vector<STGame>::iterator gameVct)
{
	int spiltIndex = 0;
	std::wstring game_id;
	std::wstring id_numb;
	GFGameLIST gameBox;

	game_id = L"game_plat_" + std::to_wstring(gameVct->nPlatID) + L"_id_" + std::to_wstring(gameVct->nGameID);
	 
	ui::ListContainerElement*ElementBox = buildSubListContainerElement(L"leigod/model/game_model_box.xml");
	if (ElementBox)
	{

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
		ElementBox->AttachSelect(nbase::Bind(&CreateXmlObj::OnGameSelect, this, std::placeholders::_1));

	}
	game_id = L"";
}



void CreateXmlObj::CreateGameElementList()
{
	int i = 0;
	int count = 0;

	std::wstring game_id;
	std::wstring id_numb;
	ui::ListBox * pAllGameListBox = dynamic_cast<ui::ListBox*>(FindControl(L"all_game_tlb"));


	STGamePlat &GamePlat = BH_LOGIC_CENTER()->GetGamePlat();
	STGameList & GameFor = BH_LOGIC_CENTER()->GetGameListFor();
	auto pFor = GameFor.vctGames.begin();
	for (; i < GameFor.nCount; i++)
	{
		if (GamePlat.vctPlats.size() < 4)
			return;

		CreateGameElement(pAllGameListBox, pFor);
		if (pFor != GameFor.vctGames.end())
			pFor++;
	}

	STGameList & rfChn = BH_LOGIC_CENTER()->GetGameListChn();
	auto pChn = rfChn.vctGames.begin();
	std::vector<ui::ListContainerElement*> pViewListElementGameBoxChn(rfChn.nCount);
	for (i = 0; i < rfChn.nCount; i++)
	{
		CreateGameElement(pAllGameListBox, pChn);
		pChn++;
	}
}
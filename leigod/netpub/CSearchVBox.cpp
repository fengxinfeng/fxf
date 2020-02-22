#include "stdafx.h"
#include "CSearchVBox.h"
#include "CAccLogicCenter.h"
#include "StringHelper.h"
#include "CMainFrameUI.h"

namespace nui {

	CSearchVBox::CSearchVBox(CMainFrameUI *p) : CSubVBox(p) {
		 
	}

	CSearchVBox::~CSearchVBox() {

	}

	void   CSearchVBox::CreateSearchElementList(vector<STGame> vctNickGame) {
		OutputDebugString(L"CSearchVBox::CreateSearchElementList--->");
		int i = 0;
		int spiltIndex = 0;
		std::wstring game_id;
		std::wstring id_numb;
		StringVector nameVct;
		ui::ListBox * pNickListBox = dynamic_cast<ui::ListBox*>(FindSubControl(L"search_result_listbox0"));
		pNickListBox->RemoveAll();
		auto pNick = vctNickGame.begin();
		for (; i < vctNickGame.size(); i++)
		{
			spiltIndex = stralgo::StringHelper::SplitString(pNick->wsGameTitle, nameVct, L"-");
			game_id = L"nick_game_" + std::to_wstring(pNick->nPlatID) + L"_id_" + std::to_wstring(pNick->nGameID);
			ui::ListContainerElement * NickElement = buildSubListContainerElement(L"leigod/model/nick_model_box.xml");
			if (NickElement)
			{
				pNickListBox->Add(NickElement);

				NickElement->SetName(game_id);
				ui::Label * pGameLogo = dynamic_cast<ui::Label *>(m_parent->FindControl(L"nick_game_logo"));
				pGameLogo->SetBkImage(pNick->wsLogoPath);

				ui::Label * pGameName = dynamic_cast<ui::Label *>(m_parent->FindControl(L"nick_game_name"));
				pGameName->SetText(pNick->wsGameTitle);

				ui::Label * pGameServer = dynamic_cast<ui::Label *>(m_parent->FindControl(L"nick_game_server"));
				pGameServer->SetText(pNick->wsSubTitle);

				NickElement->SetClass(L"listitem2");
				NickElement->AttachSelect(nbase::Bind(&CSearchVBox::OnGameListClick, this, std::placeholders::_1));
				//NickElement->AttachSetFocus(nbase::Bind(&CMainFrameUI::OnElementGetFocus, this, std::placeholders::_1));
				NickElement->SetVisible(true);
				//m_mapNickList.insert(Insert_Pair_server(game_id, pViewListElementNick[i]));
			}

			game_id = L"";
			pNick++;
		}
		OutputDebugString(L"<-----     CSearchVBox::CreateSearchElementList ");
	}


	bool CSearchVBox::OnGameListClick(ui::EventArgs* args)
	{
		int index = 0;
		int spiltIndex = 0;
		int nSelect = 0;
		WCHAR  *stopStr;
		STGame STGameGot;
		StringVector nameVct;
		StringVector idVct;
		std::wstring wstrGameId, gameServerName, gameName;
		std::wstring BigPhotoPath;

		if (m_parent->m_nIsArccrate > 0)
		{
			/*uiTipMessageBox *tipChildWnd = new uiTipMessageBox(m_hWnd);
			nSelect = tipChildWnd->DoModal();
			delete tipChildWnd;*/
			nSelect = m_parent->CreateTipMsgBox();
		}
		if (nSelect == 1 || m_parent->m_nIsArccrate == 0)
		{
			wstring boxName = args->pSender->GetName();
			if (boxName.empty())
				return false;
			if (nSelect == 1)
			{
				m_parent->SetStopAccrate();
			}
			spiltIndex = stralgo::StringHelper::SplitString(boxName, idVct, L"_");
			if (spiltIndex > 0)
				m_parent->ShowSignalGame(std::wcstol(idVct[4].c_str(), &stopStr, 0));
			//STGameGot = GetGameClickObj(std::wcstol(idVct[2].c_str(), &stopStr, 0), std::wcstol(idVct[4].c_str(), &stopStr, 0));
			else
				return false;
		}

 
 
		return true;
	}


}

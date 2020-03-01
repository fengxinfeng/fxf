#include "stdafx.h"
#include "uiMessageBox.h"
#include "resource.h"
#include "CAccLogicCenter.h"

const std::wstring uiMessageBox::kClassName = L"MsgBox";
uiMessageBox::uiMessageBox(HWND parentHwnd,int nTime,bool BindWx)
{
	m_parentHwnd = parentHwnd;
	m_ntime = nTime;
	m_isBindWx = BindWx;
	m_isSelectedCheckBox = 0;
	m_SelectedOption = 1;
}


uiMessageBox::~uiMessageBox()
{
}

std::wstring uiMessageBox::GetSkinFolder()
{
	return L"leigod";
}

std::wstring uiMessageBox::GetSkinFile()
{
	return L"one/close_window.xml";
}

std::wstring uiMessageBox::GetWindowClassName() const
{
	return kClassName;
}

void uiMessageBox::InitWindow()
{
	ui::Button * pBtnOneOk = dynamic_cast<ui::Button *>(FindControl(L"one_ok_btn"));
	if (pBtnOneOk)
	{
		pBtnOneOk->AttachClick(nbase::Bind(&uiMessageBox::OnBtnOneOkClick, this, std::placeholders::_1));
	}
	ui::Button * pBtnTwoOk = dynamic_cast<ui::Button *>(FindControl(L"two_ok_btn"));
	if (pBtnTwoOk)
	{
		pBtnTwoOk->AttachClick(nbase::Bind(&uiMessageBox::OnBtnTwoOkClick, this, std::placeholders::_1));
	}
	ui::Button * pBtnTwoCancel= dynamic_cast<ui::Button *>(FindControl(L"two_cancel_btn"));
	if (pBtnTwoCancel)
	{
		pBtnTwoCancel->AttachClick(nbase::Bind(&uiMessageBox::OnBtnCancelClick, this, std::placeholders::_1));

	}
	ui::Button * pBtnClose2 = dynamic_cast<ui::Button *>(FindControl(L"close_btn2"));
	if (pBtnClose2)
	{
		pBtnClose2->AttachClick(nbase::Bind(&uiMessageBox::OnBtnCancelClick, this, std::placeholders::_1));
	}
	STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
	if (ConfigSet.nExitMode == 0)
	{
		ui::Option * pOptionMin = dynamic_cast<ui::Option*>(FindControl(L"min_to_tory_option"));
		if (pOptionMin)
			pOptionMin->Selected(true);
		else
			pOptionMin->Selected(false);
	}
	if (ConfigSet.nExitFix == 1)
	{
		ui::CheckBox * pTipCheackBox = dynamic_cast<ui::CheckBox*>(FindControl(L"close_warn_chbox"));
		if (pTipCheackBox)
			pTipCheackBox->Selected(true);
	}
	

}

LRESULT uiMessageBox::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void uiMessageBox::Close(UINT nRet)
{
	__super::Close(nRet);
}

void uiMessageBox::CreateWnd()
{

}

void uiMessageBox::DirectShow()
{
}

bool uiMessageBox::OnBtnOneOkClick(ui::EventArgs* args)
{
	ui::Option * pTipCheackBox1 = dynamic_cast<ui::Option*>(FindControl(L"min_to_tory_option"));
	if (pTipCheackBox1)
	{
		if (!pTipCheackBox1->IsSelected())
		{
			m_SelectedOption = 0;
		}
		else
			m_SelectedOption = 1;
	}

	ui::CheckBox * pTipCheackBox= dynamic_cast<ui::CheckBox*>(FindControl(L"close_warn_chbox"));
	if (pTipCheackBox)
		m_isSelectedCheckBox = pTipCheackBox->IsSelected();
	else
		return false;

	
	
	STGameListConfigration &ConfigSet = LOGIC_CENTER()->GetCinfigIni();
	ConfigSet.nExitMode = m_SelectedOption;
	ConfigSet.nExitFix = m_isSelectedCheckBox;
	LOGIC_CENTER()->SaveModeIni();

	if ((m_ntime <= 0 || m_isBindWx==0) || m_isBindWx)
	{
		if (m_SelectedOption)
		{
			ui::Box * pWindowBox = dynamic_cast<ui::Box*>(FindControl(L"close_window_box"));
			if (pWindowBox)
				pWindowBox->SetVisible(false);
			Close(0);
			::ShowWindow(m_parentHwnd, SW_HIDE);
			return 0;
		}
		else
		{
			::ShowWindow(m_hWnd, SW_HIDE);
			Close(0);
			PostQuitMessage(0L);
			return 0;
		}
	}
	
	ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"quit_window"));
	if (pBox)
		pBox->SetVisible(false);

	ui::VBox * pBoxagain = dynamic_cast<ui::VBox*>(FindControl(L"again_tip_quit_window"));
	if(pBoxagain)
		pBoxagain->SetVisible(true);

	return true;
}

bool uiMessageBox::OnBtnTwoOkClick(ui::EventArgs* args)
{
	ui::Option * pTipCheackBox1 = dynamic_cast<ui::Option*>(FindControl(L"min_to_tory_option"));
	if (pTipCheackBox1)
	{
		if (pTipCheackBox1->IsSelected())
			m_SelectedOption = 0;
		else
			m_SelectedOption = 1;
	}

	ui::CheckBox * pTipCheackBox = dynamic_cast<ui::CheckBox*>(FindControl(L"close_warn_chbox"));
	if (pTipCheackBox)
		m_isSelectedCheckBox = pTipCheackBox->IsSelected();
	else
		return false;

	STGameListConfigration &ConfigSet=LOGIC_CENTER()->GetCinfigIni();
	ConfigSet.nExitMode = m_SelectedOption;
	ConfigSet.nExitFix = m_isSelectedCheckBox;
	LOGIC_CENTER()->SaveModeIni();
	Close(0);
	PostQuitMessage(0L);
}
bool uiMessageBox::OnBtnCancelClick(ui::EventArgs* args)
{
	Close(0);
	return true;
}

void uiMessageBox::CenterFrame()
{

}

UINT uiMessageBox::DoModal()
{
	//m_parentHwnd = parteHwnd;
	Create(m_parentHwnd,uiMessageBox::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	CenterWindow();
	ShowModal();
	//ShowModalFake(m_parentHwnd);
	return 0;
}

void uiMessageBox::OnFinalMessage(HWND hWnd)
{

}

void uiMessageBox::SetText(LPCTSTR lpText)
{
	if (lpText)
	{
		m_wsShowText = lpText;
	}

}


#include "stdafx.h"
#include "uiTipMessageBox.h"
#include "resource.h"
#include "CAccLogicCenter.h"

const std::wstring uiTipMessageBox::kClassName = L"TipMsgBox";
uiTipMessageBox::uiTipMessageBox(HWND hParent, std::wstring OutText)
{
	m_hParent = hParent;
	m_wstrOutText = OutText;
}


uiTipMessageBox::~uiTipMessageBox()
{
}

std::wstring uiTipMessageBox::GetSkinFolder()
{
	return L"leigod";
}

std::wstring uiTipMessageBox::GetSkinFile()
{
	return L"one/tip_window.xml";
}

std::wstring uiTipMessageBox::GetWindowClassName() const
{
	return kClassName;
}

void uiTipMessageBox::InitWindow()
{
	ui::Button * pBtnCancel = dynamic_cast<ui::Button *>(FindControl(L"tip_cancel_btn"));
	if (pBtnCancel)
	{
		pBtnCancel->AttachClick(nbase::Bind(&uiTipMessageBox::OnBtnCancelClick, this, std::placeholders::_1));
	}
	ui::Button * pBtnOk = dynamic_cast<ui::Button *>(FindControl(L"tip_ok_btn"));
	if (pBtnOk)
	{
		pBtnOk->AttachClick(nbase::Bind(&uiTipMessageBox::OnBtnOkClick, this, std::placeholders::_1));
	}
	if (!m_wstrOutText.empty())
	{
		ui::Label * pLbTip = dynamic_cast<ui::Label *>(FindControl(L"msg_tip_text"));
		if (pLbTip)
			pLbTip->SetText(m_wstrOutText);
	}
}

LRESULT uiTipMessageBox::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void uiTipMessageBox::Close(UINT nRet)
{
	__super::Close(nRet);
}

bool uiTipMessageBox::OnBtnCancelClick(ui::EventArgs* args)
{
	Close(0);
	m_nSelectBtn = 0;
	return false;
}
bool uiTipMessageBox::OnBtnOkClick(ui::EventArgs* args)
{
	Close(0);
	m_nSelectBtn = 1;
	return true;
}
void uiTipMessageBox::CenterFrame()
{

}

UINT uiTipMessageBox::DoModal()
{
	
	Create(m_hParent,uiTipMessageBox::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	CenterWindow();
	ShowModal();
	//ShowModalFake(m_parentHwnd);
	return m_nSelectBtn;
}

void uiTipMessageBox::OnFinalMessage(HWND hWnd)
{

}


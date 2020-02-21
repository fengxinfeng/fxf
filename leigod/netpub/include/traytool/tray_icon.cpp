#include "tray_icon.h"
#include <strsafe.h>


TrayIconUI::TrayIconUI()
{
    ZeroMemory(&icon_data_,sizeof(NOTIFYICONDATA));
    icon_data_.cbSize   = sizeof(NOTIFYICONDATA);
    icon_data_.uFlags   = NIF_ICON;
}

TrayIconUI::~TrayIconUI()
{
    Delete();
}

// 显示托盘
BOOL TrayIconUI::Show()
{
    return (0 != Shell_NotifyIcon(NIM_ADD,&icon_data_));
}

// 删除托盘
BOOL TrayIconUI::Delete()
{
    return (0 != Shell_NotifyIcon(NIM_DELETE,&icon_data_));
}

// 修改托盘显示的图标
BOOL TrayIconUI::ModifyIcon(HINSTANCE hInst,UINT idIcon)
{
    if(hInst == NULL)
        return FALSE;

    HICON hIcon = LoadIcon(hInst,MAKEINTRESOURCE(idIcon));
    icon_data_.hIcon = hIcon;

    return (0 != Shell_NotifyIcon(NIM_MODIFY,&icon_data_));
}

RECT TrayIconUI::GetWinRect()
{
	RECT rcBack = { 0 };
	NOTIFYICONIDENTIFIER notifyer;
	notifyer.cbSize = sizeof(NOTIFYICONIDENTIFIER);
	notifyer.hWnd = icon_data_.hWnd;
	notifyer.guidItem = icon_data_.guidItem;
	notifyer.uID = icon_data_.uID;
	::Shell_NotifyIconGetRect(&notifyer, &rcBack);

	return rcBack;
}

BOOL TrayIconUI::ModifyIcon(HICON hIcon)
{
    icon_data_.hIcon = hIcon;

    return (0 != Shell_NotifyIcon(NIM_MODIFY,&icon_data_));
}

BOOL TrayIconUI::ModifyIcon(HINSTANCE hInst,LPCTSTR lpszIcon)
{
    if(hInst == NULL)
        return FALSE;

    HICON hIcon = LoadIcon(hInst,lpszIcon);
    icon_data_.hIcon = hIcon;

    return (0 != Shell_NotifyIcon(NIM_MODIFY,&icon_data_));
}

// 设置接收托盘消息的窗口
BOOL TrayIconUI::SetNotifyWnd(HWND hWnd,UINT uCallbackMsg)
{
    if(hWnd != NULL && IsWindow(hWnd))
    {
        icon_data_.uFlags |= NIF_MESSAGE;
        icon_data_.hWnd = hWnd;
        icon_data_.uCallbackMessage = uCallbackMsg;

        return TRUE;
    }

    return FALSE;
}

// 设置托盘提示信息
void TrayIconUI::SetToolTip(LPCTSTR lpszTip /*= NULL*/)
{
    if(lpszTip != NULL)
    {
        icon_data_.uFlags |= NIF_TIP;
#if (NTDDI_VERSION < NTDDI_WIN2K)
        StringCchCopy(icon_data_.szTip, 64, lpszTip);
#else
        StringCchCopy(icon_data_.szTip, 128, lpszTip);
#endif
    }
    else
    {
        icon_data_.uFlags |= ~NIF_TIP;
    }
}

// 设置托盘气球提示
void TrayIconUI::SetBallonToolTip(LPCTSTR lpszTitle /*= NULL*/,LPCTSTR lpszTip /*= NULL*/)
{
    icon_data_.uFlags |= NIF_INFO;

    if(lpszTip != NULL)
    {
        int srcLen = _tcslen(lpszTip);
        int tipLen = (srcLen >= sizeof(icon_data_.szInfo) ? sizeof(icon_data_.szInfo)-1 : srcLen);
        _tcscpy_s(icon_data_.szInfo, 256, lpszTip);

        if(lpszTitle != NULL)
        {
            srcLen = _tcslen(lpszTitle);
            tipLen = (srcLen >= sizeof(icon_data_.szInfoTitle) ? sizeof(icon_data_.szInfoTitle)-1 : srcLen);
            lstrcpyn(icon_data_.szInfoTitle,lpszTitle,tipLen);
        }
    }
    else
    {
        lstrcpy(icon_data_.szInfo,_T("")); 
    }
}

// 设置托盘图标
BOOL TrayIconUI::SetIcon(HINSTANCE hInst,UINT idIcon)
{
    if(hInst == NULL)
        return FALSE;

    HICON hIcon = LoadIcon(hInst,MAKEINTRESOURCE(idIcon));
    if(hIcon == NULL)
        return FALSE;

    icon_data_.hIcon = hIcon;

    return TRUE;
}

BOOL TrayIconUI::SetIcon(HICON hIcon)
{
    if(hIcon == NULL)
        return FALSE;

    icon_data_.hIcon = hIcon;

    return TRUE;
}

BOOL TrayIconUI::SetIcon(HINSTANCE hInst,LPCTSTR lpszIcon)
{
    if(hInst == NULL)
        return FALSE;

    HICON hIcon = LoadIcon(hInst,lpszIcon);
    if(hIcon == NULL)
        return FALSE;

    icon_data_.hIcon = hIcon;

    return TRUE;
}

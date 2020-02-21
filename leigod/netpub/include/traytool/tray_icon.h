#ifndef __TRAY_ICON_H__
#define __TRAY_ICON_H__
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <Windows.h>
#include <tchar.h>
#include <ShellAPI.h>

class TrayIconUI
{
public:
    TrayIconUI();
    ~TrayIconUI();

public:
    BOOL Show();
    BOOL Delete();

    BOOL ModifyIcon(HINSTANCE hInst,UINT idIcon);
    BOOL ModifyIcon(HICON hIcon);
    BOOL ModifyIcon(HINSTANCE hInst,LPCTSTR lpszIcon);

    BOOL SetNotifyWnd(HWND hWnd,UINT uCallbackMsg);

    void SetToolTip(LPCTSTR lpszTip = NULL);
    void SetBallonToolTip(LPCTSTR lpszTitle = NULL,LPCTSTR lpszTip = NULL);

    BOOL SetIcon(HINSTANCE hInst,UINT idIcon);
    BOOL SetIcon(HICON hIcon);
    BOOL SetIcon(HINSTANCE hInst,LPCTSTR lpszIcon);
	RECT GetWinRect();

private:
    NOTIFYICONDATA icon_data_;
};

#endif

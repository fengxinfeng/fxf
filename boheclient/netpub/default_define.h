#pragma once
#include "stdafx.h"
#include<string>


#define BH_FLOAT_XML     1
#define BH_TAB_XML       2
#define BH_NAV_TAB_XML   3


#define NAV_BTN          50
#define MYSPEED_BTN      51


struct Navbtn
{
	std::wstring btnName;
	ui::Control * pBtn;
};

struct TabBtnLb
{
	std::wstring btnName;
	std::wstring btnParentName;
	ui::Control * pBtn;
};

struct GameMode
{
	int PlatId;
	int GameId;
	std::wstring GameName;
	ui::ListContainerElement* GameElement;
};


typedef std::function<bool(ui::EventArgs* args)> OnCallBackFunction;
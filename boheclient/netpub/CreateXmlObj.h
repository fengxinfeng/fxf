#pragma once
#include "default_define.h"
#include "engine_define.h"
#include "CDownloadVBox.h"
//#include "CAreaVBox.h"
#include "CServerVBox.h"
//#include "COperVBox.h"
//#include "CSearchVBox.h"
//#include "CCefVBOX.h"

const bool kEnableOffsetRenderBh = true;
class UILIB_API CreateXmlObj:public std::conditional<kEnableOffsetRenderBh, ui::WindowImplBase, nim_comp::ShadowWndBase>::type
{
public:
	CreateXmlObj();
	~CreateXmlObj();
public:
	virtual bool OnNavTabClick(ui::EventArgs* args) { return 0; };
	virtual bool OnTabClick(ui::EventArgs* args) { return 0; };
	virtual bool OnNickSelect(ui::EventArgs* args) { return 0; };
	virtual bool OnLoginClick(ui::EventArgs* args) { return 0; };
	virtual bool OnDialogCloseClick(ui::EventArgs* args) { return 0;};
	virtual bool OnNodeClick(ui::EventArgs* args) { return 0; };
	virtual bool OnMonitorClick(ui::EventArgs* args) { return 0; };
	virtual bool OnGamePathClick(ui::EventArgs* args) { return 0; };
	virtual bool OnMainPageBtnClick(ui::EventArgs* args) { return 0; };
	virtual bool OnThridLoginClick(ui::EventArgs* args) { return 0; };
	virtual bool OnGameSelect(ui::EventArgs* args) { return 0; };
public:
	virtual bool OnPhoneNumberTextChangeEvent(ui::EventArgs* args) { return 0; };
	virtual bool OnPhoneNumberKillFocusEvent(ui::EventArgs* args) { return 0; };
	virtual bool OnPhoneNumberGetFocusEvent(ui::EventArgs* args) { return 0; };

	virtual bool OnPwdTextChangeEvent(ui::EventArgs* args) { return 0; };
	virtual bool OnPwdKillFocusEvent(ui::EventArgs* args) { return 0; };
	virtual bool OnPwdGetFocusEvent(ui::EventArgs* args) { return 0; };

	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
private:
	ui::VBox *buildSubView(const wchar_t * xmlName);
	ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
	bool AddXmlToXmlVct(int xmlType,std::wstring parentBox, std::wstring xmlName, bool isShow);
	void CreateGameElement(ui::ListBox * gameList,vector<STGame>::iterator gameVct);
	void CreateGameElementList();
	void   CreateDownloadtList(int count, int type);
protected:
	bool InitCreate();
	void OnAreaSelect(int nArea);
public:
	bool AddXml();
	bool AddNickBox();
	bool AddDownLoadBox();
	bool AddSvrBox();
	bool AddAreaBox();
	bool AddNodeBox();
	bool AddNoticeText();
	bool AddGameBox();
public:
	void BindClick();
	void BindEvent();
	void BindCef();
	void BindBtn(int btnType,std::wstring btnName,OnCallBackFunction pOnFun);
public:
	std::map<std::wstring, ui::VBox*> m_mapFloatParentVb;
	std::map<std::wstring, ui::VBox*> m_mapFloatXmlList;
	std::map<std::wstring, ui::VBox*> m_mapTabParentVb;
	std::map<std::wstring, ui::VBox*> m_mapTabXmlList;
	std::map<std::wstring, ui::VBox*> m_mapNavTabParentVb;
	std::map<std::wstring, ui::VBox*> m_mapNavTabXmlList;
	std::vector<Navbtn> m_btnVct;
	std::vector<Navbtn> m_mySpeedTabVct;
	std::vector<TabBtnLb> m_TabLbVct;
	std::vector<TabBtnLb> m_MonitorTabLbVct;
	std::vector<GameMode> m_GameVct;
	vector<GFGameLIST> vctGame; 

protected:
	int 	m_nIsArccrate = 0;              //¼ÓËÙ×´Ì¬
	int   m_nAreaID = 0;
public:
	nim_comp::CefControlBase* m_ThridCef;
public:
	//friend class nui::CAreaVBox;
	friend class nui::CServerVBox;
	//friend class nui::COperVBox;
	//friend class nui::CSearchVBox;
	//friend class nui::CCefVBOX;
	friend class nui::CDownloadVBox;
	//nui::CAreaVBox  *m_areaVbox;
	nui::CServerVBox *m_serverVbox;
	//nui::COperVBox *m_operVbox;
	//nui::CSearchVBox *m_searchVBox;
	//nui::CCefVBOX *m_cefVBox; 
	nui::CDownloadVBox *m_downloadVBox;

};



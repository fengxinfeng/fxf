#pragma once
#include <vector>
#include <map>
#include "CAccLogicCenter.h"
#include "traytool/tray_icon.h"

const bool kEnableOffsetRender = true;
class CMainFrameUI : public std::conditional<kEnableOffsetRender, ui::WindowImplBase, nim_comp::ShadowWndBase>::type,
					 public ILogicCallback
{
public:
	CMainFrameUI();
	~CMainFrameUI();

	typedef bool (CMainFrameUI:: *CallBtnClick)(ui::EventArgs* args);
	CallBtnClick  m_btnclick;
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	//virtual void InitWindow1() override;
	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;
	void ReleaseOnExit();
public:
	virtual void OnLazyInitResponse(int nCode);
	virtual void OnLoginPinResponse(int nCode);
	virtual void OnPinImageArrive(int nCode);
	virtual void OnKeyImageArrive(int nCode);
	virtual void OnLoginKeyResponse(int nCode);
	virtual void OnGetGroupLineResponse(int nCode);
	virtual void OnGetAccInfoResponse(int nCode);
	virtual void OnEngineLog(int, string);
	//virtual void OnTokenExpire(int);				//时间耗尽
	virtual void OnDelayResponse(int nGameID, int nAreaID);
	virtual void OnEnginAccResult(int, string);
	virtual void OnEnginData(uint32_t nFlush, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate);
	virtual void OnExpireed(int nSrc);
private:

	void CallJsBack(const std::string & ss);
	//控件对象创建
	bool XmlToXmlObjVct(std::wstring parentBox,std::wstring xmlName,bool isShow=false);
	bool CreateXmlObject();
	void CreateAreaElementList();
	void CreateNickElementList(vector<STGame> vctNickGame);
	void CreateGameElement(ui::ListBox * gameList,vector<STGame>::iterator gameVct);
	void CreateGameElementList();

	//响应绑定
	void CreateBtnBindClick();
	void CreateCloseWndBind();
	void CreateTabBind();
	void CreateMenuBind();
	void CreateSvrSelectBind();
	void CreateMonitorBind();
	void CreateAccrateBind();
	void CreateNickBind();
	void CreateTipBind();
	void CreateNoTypeBind();
	//响应函数
	//窗口关闭btn
	bool OnBtnCloseMainWindows(ui::EventArgs* args);
	bool OnBtnPushWindowCloseClick(ui::EventArgs* args);
	bool OnBtnCloseMonitorWnd(ui::EventArgs* args);
	//加速响应btn
	void OnBtnAccrateProgress(ui::Control* fill);
	bool OnBtnAccrateStart(ui::EventArgs* args);
	bool OnBtnAccrateStop(ui::EventArgs* args);
	void SetStopAccrate();
	static void CALLBACK OnTimeCount(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	static void CALLBACK OnTimeCountConverse(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

	//搜索响应
	bool OnNickTextChangeEvent(ui::EventArgs* args);
	bool OnNickKillFocusEvent(ui::EventArgs* args);
	bool OnNickGetFocusEvent(ui::EventArgs* args);
	//主页游戏图片切换响应btn
	bool OnBtnChangeCurrentPhotoClick(ui::EventArgs* args);
	//头部(myspeed 平台 steam origin uplay epic 国服)btn响应
	bool OnBtnTabClick(ui::EventArgs* args);
	//游戏块点击响应btn
	bool OnGameListClick(ui::EventArgs* args);
	//(区服 地区 运营商)btn 响应
	bool OnBtnSelectNodeClick(ui::EventArgs* args);
	//运营商(推荐 移动 联通 电信 其他)btn 响应
	//bool OnBtnOperatorClick(ui::EventArgs* args);
	void SelectAreaItem(wstring wsKey);
	bool OnAreaSelected(ui::EventArgs* args);
	bool OnSvrSelected(ui::EventArgs* args);
	bool OnOpertSelected(ui::EventArgs* args);
	void OnGameSelect(int nGameID);
	void OnAreaSelect(int nArea);
	void OnLineGroupResponse(int nGameID, int nAreaID);
	void ClearListCtrl(ui::Control *);
	void UpdateListCtrl(ui::Control *);
	void OnOpertEvnBimd();
	bool OnBtnOpertTabClick(ui::EventArgs* args);
	//游客登录提示响应
	bool OnBtnTipEvent(ui::EventArgs* args);
	bool OnBtnLeaveEvent(ui::EventArgs* args);
	//监控btn
	bool OnBtnMonitorWndClick(ui::EventArgs* args);
	//菜单btn 
	bool OnBtnMenu(ui::EventArgs* args);
	bool OnBtnMenuKillFocus(ui::EventArgs* args);
	//操作成功提示框响应
	bool OnSuccessTip(ui::VBox * pBox, std::wstring boxName);
	bool CreateTipMsgBox(std::wstring TipText=L"");
	//微信绑定
	bool OnBtnWx(ui::EventArgs* args);
	//充值页面转跳
	void OnEnchageUrlChange(const std::wstring& url);
	CefRequestHandler::ReturnValue OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect);
	void OnLoadEnd(int httpStatusCode);
	CefRequestHandler::ReturnValue OnBeforeNavigateBind(CefRefPtr<CefRequest> request, bool is_redirect);
	CefRequestHandler::ReturnValue OnBeforeNavigateReward(CefRefPtr<CefRequest> request, bool is_redirect);

	void OnWebTimeArrive(int nHour, int nMin);

	//消息拦截处理
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual ui::Control* CreateControl(const std::wstring& pstrClass);

	vector<GFGameLIST> vctGame;
	std::map<std::wstring, ui::VBox*> m_mapXmlParentBox;
	std::map<std::wstring, ui::VBox*> m_mapXmlList;
	std::map<std::wstring, ui::ListContainerElement*> m_mapServerList;
	std::map<std::wstring, ui::ListContainerElement*> m_mapAreaList;
	std::map<std::wstring, ui::ListContainerElement*> m_mapOperatorList;
	std::map<std::wstring, ui::ListContainerElement*> m_mapGameList_f;
	std::map<std::wstring, ui::ListContainerElement*> m_mapNickList;
	std::map<int, ui::ListContainerElement*> m_mapGameList; 
	
	//辅助函数
	bool  ShowArccrateAndQiutSet(int nMode);

	void  IsSelectedArccrateBtn(bool isShow);
	void  IsSelectedMonitorBtn(bool isShow);

	bool  MonitorTimeBtnSelected(std::wstring btnName);
	void SetArccrateNumbColor(uint32_t nFlush, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate, std::wstring ColorName);
	bool ShowSignalGame(int nGameid);
	bool CleanGameElement();
	bool ShowSelected(int oldSelect, int newSelect);
	bool ShowORHideMonitorWnd(bool isShow);
	bool ShowMonitorNetORLogWnd(bool isShow);
	bool ShowMenu();
	bool ShowOrHideMenuSetWnd(bool isShow);
	bool ShowSetArccrateOrQuitWnd(bool isShow);
	bool ShowMenuRepairNetOrCleanBufWnd(std::wstring wndName);
	void ShowGameBox(std::wstring platName);
	void CreateChildModel();
	bool IsSelectOption(std::wstring optionName, int type, int value);
	bool GetMenuSet(STGameListConfigration & menuSet);
	STGame GetGameClickObj(int platId, int gameId);
	ui::VBox * buildSubView(const wchar_t * xmlName);
	ui::ListContainerElement * buildSubListContainerElement(const wchar_t * xmlName);
	void ShowRewardPage();
	wstring GetLineItemName(STLineItem & itrItem);
	wstring GetIspNameOnID(int nIsp);

	volatile int m_nControlArccrate;      //进度条状态
	int m_SmSelect = 0;;
	int m_nGameID = 0;
	int m_nAreaID = 0;
	int m_nSvrID = 0;
	int m_nModelSelect;
	int m_nTransferSelect;
	int m_nExitModl;
	int m_nExitFix;
	int m_nOldSelected=-1;
	int m_nOldGameSelected= -1;
	int	m_nIsArccrate = 0;              //加速状态
	bool m_bWaitLogin = false;
	bool m_bWXBind = false;					//是否已经微信绑定
	ui::VBox * m_pPushBox;
	ui::VBox * m_pMenuBox;
	TrayIconUI m_trayTool;
	nim_comp::CefControlBase * m_pCefEnCharge = NULL;
	nim_comp::CefControlBase * m_pCefBind = NULL; 
	nim_comp::CefControlBase * m_pCefReward = NULL;
	int OverCount = 0;
public:
	static ui::Label * m_pLbH;
	static ui::Label * m_pLbM;
	static ui::Label * m_pLbS;
	static ui::Label * m_pLbHConver;
	static ui::Label * m_pLbMConver;
	static ui::Label * m_pLbSConver;
	static DWORD m_dwStartTime;
	static DWORD m_dwStartTimeConverse;
	static int64_t m_nTimeCount;
	wstring m_wsProvinceName;
	bool m_bInited = false;
};


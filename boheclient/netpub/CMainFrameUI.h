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
	//virtual void OnTokenExpire(int);				//ʱ��ľ�
	virtual void OnDelayResponse(int nGameID, int nAreaID);
	virtual void OnEnginAccResult(int, string);
	virtual void OnEnginData(uint32_t nFlush, uint32_t nDelay, uint32_t nAccRate, uint32_t nLossRate);
	virtual void OnExpireed(int nSrc);
private:

	void CallJsBack(const std::string & ss);
	//�ؼ����󴴽�
	bool XmlToXmlObjVct(std::wstring parentBox,std::wstring xmlName,bool isShow=false);
	bool CreateXmlObject();
	void CreateAreaElementList();
	void CreateNickElementList(vector<STGame> vctNickGame);
	void CreateGameElement(ui::ListBox * gameList,vector<STGame>::iterator gameVct);
	void CreateGameElementList();

	//��Ӧ��
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
	//��Ӧ����
	//���ڹر�btn
	bool OnBtnCloseMainWindows(ui::EventArgs* args);
	bool OnBtnPushWindowCloseClick(ui::EventArgs* args);
	bool OnBtnCloseMonitorWnd(ui::EventArgs* args);
	//������Ӧbtn
	void OnBtnAccrateProgress(ui::Control* fill);
	bool OnBtnAccrateStart(ui::EventArgs* args);
	bool OnBtnAccrateStop(ui::EventArgs* args);
	void SetStopAccrate();
	static void CALLBACK OnTimeCount(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	static void CALLBACK OnTimeCountConverse(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

	//������Ӧ
	bool OnNickTextChangeEvent(ui::EventArgs* args);
	bool OnNickKillFocusEvent(ui::EventArgs* args);
	bool OnNickGetFocusEvent(ui::EventArgs* args);
	//��ҳ��ϷͼƬ�л���Ӧbtn
	bool OnBtnChangeCurrentPhotoClick(ui::EventArgs* args);
	//ͷ��(myspeed ƽ̨ steam origin uplay epic ����)btn��Ӧ
	bool OnBtnTabClick(ui::EventArgs* args);
	//��Ϸ������Ӧbtn
	bool OnGameListClick(ui::EventArgs* args);
	//(���� ���� ��Ӫ��)btn ��Ӧ
	bool OnBtnSelectNodeClick(ui::EventArgs* args);
	//��Ӫ��(�Ƽ� �ƶ� ��ͨ ���� ����)btn ��Ӧ
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
	//�ο͵�¼��ʾ��Ӧ
	bool OnBtnTipEvent(ui::EventArgs* args);
	bool OnBtnLeaveEvent(ui::EventArgs* args);
	//���btn
	bool OnBtnMonitorWndClick(ui::EventArgs* args);
	//�˵�btn 
	bool OnBtnMenu(ui::EventArgs* args);
	bool OnBtnMenuKillFocus(ui::EventArgs* args);
	//�����ɹ���ʾ����Ӧ
	bool OnSuccessTip(ui::VBox * pBox, std::wstring boxName);
	bool CreateTipMsgBox(std::wstring TipText=L"");
	//΢�Ű�
	bool OnBtnWx(ui::EventArgs* args);
	//��ֵҳ��ת��
	void OnEnchageUrlChange(const std::wstring& url);
	CefRequestHandler::ReturnValue OnBeforeNavigate(CefRefPtr<CefRequest> request, bool is_redirect);
	void OnLoadEnd(int httpStatusCode);
	CefRequestHandler::ReturnValue OnBeforeNavigateBind(CefRefPtr<CefRequest> request, bool is_redirect);
	CefRequestHandler::ReturnValue OnBeforeNavigateReward(CefRefPtr<CefRequest> request, bool is_redirect);

	void OnWebTimeArrive(int nHour, int nMin);

	//��Ϣ���ش���
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
	
	//��������
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

	volatile int m_nControlArccrate;      //������״̬
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
	int	m_nIsArccrate = 0;              //����״̬
	bool m_bWaitLogin = false;
	bool m_bWXBind = false;					//�Ƿ��Ѿ�΢�Ű�
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


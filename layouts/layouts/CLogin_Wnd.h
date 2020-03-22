#pragma once

 
 
// �����Ƿ�ʹ��������Ⱦ����Ϊ true  ʱ��ʹ��Ĭ�ϴ�����Ӱ������������Ⱦģʽ�´����� WS_EX_LAYERED ����
// ��Ϊ false ʱ��ʹ�����洰��ģʽ��֧�ִ��� WS_EX_LAYERED ���Դ��ڣ�����ʹ�����ô�����Ӱ��������Ҫ�� xml �н����� shadowattached ��������Ϊ false
const bool kEnableOffsetRender = false;
class CLoginWnd : public std::conditional<kEnableOffsetRender, ui::WindowImplBase, nim_comp::ShadowWndBase>::type
//class CLoginWnd : public ui::WindowImplBase 
{
public:
	CLoginWnd();
	~CLoginWnd();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const std::wstring	kClassName;
public:
	static CLoginWnd* ShowLoginWindow( ui::UiRect pos,HWND hwnd, nui::CLoginVBOX *);

public:
	void CloseWnd();
	void LoadURL(std::wstring url);
private:
	bool OnClicked(ui::EventArgs* msg);
	bool OnNavigate(ui::EventArgs* msg);
	void OnLoadEnd(int httpStatusCode);
	void  OnAfterCreated(CefRefPtr<CefBrowser> browser);
	bool OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect);

private:
	nim_comp::CefControlBase* cef_control_;
	nim_comp::CefControlBase* cef_control_dev_;
	ui::Button*			btn_dev_tool_;
	nui::CLoginVBOX * login_box_;
	//ui::RichEdit*		edit_url_;
};


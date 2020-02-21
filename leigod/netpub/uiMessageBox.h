#pragma once
class uiMessageBox : public ui::WindowImplBase
{
public:
	uiMessageBox(HWND parentHwnd, int nTime,bool BindWx);
	~uiMessageBox();
	void DirectShow();
	void CreateWnd();
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Close(UINT nRet);
	static const std::wstring kClassName;
	void CenterFrame();
	UINT DoModal();
	void SetText(LPCTSTR lpText);

protected:
	void OnFinalMessage(HWND hWnd);
	bool OnBtnOneOkClick(ui::EventArgs* args);
	bool OnBtnTwoOkClick(ui::EventArgs* args);
	bool OnBtnCancelClick(ui::EventArgs* args);
	std::wstring m_wsShowText;
	int m_isSelectedCheckBox;
	int m_SelectedOption;
	bool m_isBindWx;
	HWND m_parentHwnd;
	int m_ntime;
};


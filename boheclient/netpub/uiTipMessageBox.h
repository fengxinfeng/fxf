#pragma once
class uiTipMessageBox : public ui::WindowImplBase
{
public:
	uiTipMessageBox(HWND hParent, std::wstring OutText);
	~uiTipMessageBox();
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Close(UINT nRet);
	static const std::wstring kClassName;
	void CenterFrame();
	UINT DoModal();

protected:
	void OnFinalMessage(HWND hWnd);
	bool OnBtnCancelClick(ui::EventArgs* args);
	bool OnBtnOkClick(ui::EventArgs* args);
private:
	HWND m_hParent;
	UINT m_nSelectBtn;
	std::wstring m_wstrOutText;
};


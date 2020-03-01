#pragma once


class UILIB_API wMainFrame :public CreateXmlObj
{
public:
	wMainFrame();
	~wMainFrame();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

public:
	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;

public:
	virtual bool OnNavTabClick(ui::EventArgs* args) override;
    virtual bool OnLoginClick(ui::EventArgs* args) override;
	virtual bool OnDialogCloseClick(ui::EventArgs* args)override;
	virtual bool OnTabClick(ui::EventArgs* args)override;
	virtual bool OnNodeClick(ui::EventArgs* args)override;
	virtual bool OnMonitorClick(ui::EventArgs* args)override;
	virtual bool OnGamePathClick(ui::EventArgs* args)override;
	virtual bool OnMainPageBtnClick(ui::EventArgs* args)override;
	virtual bool OnThridLoginClick(ui::EventArgs* args)override;
public:
	virtual bool OnPhoneNumberTextChangeEvent(ui::EventArgs* args) override;
	virtual bool OnPhoneNumberKillFocusEvent(ui::EventArgs* args) override;
	virtual bool OnPhoneNumberGetFocusEvent(ui::EventArgs* args) override;

	virtual bool OnPwdTextChangeEvent(ui::EventArgs* args) override;
	virtual bool OnPwdKillFocusEvent(ui::EventArgs* args) override;
	virtual bool OnPwdGetFocusEvent(ui::EventArgs* args) override;
public:
	virtual bool OnGameSelect(ui::EventArgs* args) override;
public:
	bool OnNickSelect(ui::EventArgs* args);
	bool NavTabShow(std::wstring btnName);
	void CreateTabLb();
	
private:
	bool ToMySpeed();
	bool  Switch_Download_Tabpage_VBox(const WCHAR *boxname);
	bool ToGame();

private:
	//friend class nui::CAreaVBox;
	//friend class nui::CServerVBox;
	//friend class nui::COperVBox;
	//friend class nui::CSearchVBox;
	//friend class nui::CCefVBOX;
	friend class nui::CDownloadVBox;
	//nui::CAreaVBox  *m_areaVbox;
	//nui::CServerVBox *m_serverVbox;
	//nui::COperVBox *m_operVbox;
	//nui::CSearchVBox *m_searchVBox;
	//nui::CCefVBOX *m_cefVBox; 
};




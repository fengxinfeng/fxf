#pragma once

#include "CTurnImage.h"
#include "CSetting.h"

 
const bool kEnableOffsetRender = false;
class LayoutsForm : public std::conditional<kEnableOffsetRender, ui::WindowImplBase, nim_comp::ShadowWndBase>::type
//class LayoutsForm : public ui::WindowImplBase
{
public:
	LayoutsForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);
	~LayoutsForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//OVERRIDE
	//virtual LRESULT  HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


public:
	static void ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml);

private:
	 
	std::wstring class_name_;
	std::wstring theme_directory_;
	std::wstring layout_xml_;
	nui::CTurnImage *m_TurnImage;
	nui::CSetting * m_setting;
};


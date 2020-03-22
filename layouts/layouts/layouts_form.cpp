#include "stdafx.h"
#include "layouts_form.h"
#include "CPlayImage.h"
#include "CPhotoWall.h"
#include "CPhotoViewer.h"
#include "CMyCenter.h"
#include "CLoginVBOX.h" 
#include <array>

LayoutsForm::LayoutsForm(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
	: class_name_(class_name)
	, theme_directory_(theme_directory)
	, layout_xml_(layout_xml)
{
}


LayoutsForm::~LayoutsForm()
{
}

std::wstring LayoutsForm::GetSkinFolder()
{
	return theme_directory_;
}

std::wstring LayoutsForm::GetSkinFile()
{
	return layout_xml_;
}

std::wstring LayoutsForm::GetWindowClassName() const
{
	return class_name_;
}

 
ui::Control* LayoutsForm::CreateControl(const std::wstring& pstrClass)
{
	// 扫描 XML 发现有名称为 CefControl 的节点，则创建一个 ui::CefControl 控件
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}

	return NULL;
} 

void LayoutsForm::InitWindow()
{
	//ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"turnimage_container"));
 //   nui::CPhotoWall *photowall = new nui::CPhotoWall(pBox);
	//std::vector<std::wstring > arr = { L"banar/b1.png" ,L"banar/b2.png",L"banar/b3.png" , L"banar/b4.png" , L"banar/b1-.png", L"banar/b2-.png", L"banar/b3-.png", L"banar/b4-.png" };//, L"b1-.png", L"b2-.png", L"b3-.png", L"b4-.png"
 //   photowall->Construct(arr);

	/*
	ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"photoview_container"));
	nui::CPhotoViewer *photoviewer = new nui::CPhotoViewer(pBox); 
	std::vector<nui::NMedia> datas;
	 
		nui::NMedia m;
		m.imageurl = L"b1.png";
		m.mediatype = 1;
		m.videourl = L"C:/work/big-buck-bunny_trailer.webm";
		datas.push_back(m);

 
		m.imageurl = L"b2.png";    // this is video
		m.mediatype = 1;
		m.videourl = L"C:/work/big-buck-bunny_trailer.webm";
		datas.push_back(m);
 
		m.imageurl = L"b3.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);

		m.imageurl = L"b4.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);

		m.imageurl = L"b1-.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);

		m.imageurl = L"b2-.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);

		m.imageurl = L"b3-.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);

		m.imageurl = L"b4-.png";
		m.mediatype = 0;
		m.videourl = L"";
		datas.push_back(m);


	photoviewer->Construct(datas);
	//photoviewer->Start();
	 
	 */

	//m_TurnImage = new nui::CTurnImage(pBox);
	//m_TurnImage->Start();
	//nui::CPlayImage * image = new nui::CPlayImage(pBox);
	//image->Start();

	//ui::VBox * pSettingContainer = dynamic_cast<ui::VBox*>(FindControl(L"setting_container"));
	//m_setting = new nui::CSetting(pSettingContainer);


	//ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"mycenter_container"));
	//nui::CMyCenter *mycenter = new nui::CMyCenter(pBox); 
	//mycenter->Construct( );

	

     ui::VBox * pBox = dynamic_cast<ui::VBox*>(FindControl(L"cef_container"));
	 nui::CLoginVBOX *login = new nui::CLoginVBOX(pBox);
	 login->construct( 0,0,800,600);
	 login->LoadUrl(L"http://dev-box.nn.com/");
	 login->SetVisible(true);
}


/*
LRESULT  LayoutsForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_USER + 1:
		OutputDebugString(L"WM_USER + 1 ");
		return true;
		break;
	default:
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);

  }
  */
 
LRESULT LayoutsForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void LayoutsForm::ShowCustomWindow(const std::wstring& class_name, const std::wstring& theme_directory, const std::wstring& layout_xml)
{
	LayoutsForm* window = new LayoutsForm(class_name, theme_directory, layout_xml);
	window->Create(NULL, class_name.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0, nim_comp::CefManager::GetInstance()->IsEnableOffsetRender());
	window->CenterWindow();
	window->ShowWindow();
}

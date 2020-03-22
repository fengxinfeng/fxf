/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CPhotoViewer.h"  
#include <math.h>
 

/*
example:
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


	photoviewer->Construct(datas);s
*/

#define  TIMER_PLAY_VIEWER 1005

namespace nui {
	 
	static const int g_childmargin = 10;
	static std::wstring g_schildmargin = L"10";
	static const int  g_numberperscreen = 6;
	static CPhotoViewer * g_player;
	static bool g_active = false;
    
	/*
*  timer callback
*/
	void CALLBACK Play(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
	{

		if (!g_active)  return;
		g_player->DisplayNext();

	}

	CPhotoViewer::CPhotoViewer(ui::VBox *p) :CSubVBox(p)
	{  
		//ui::GlobalManager::FillBoxWithCache(this, L"layouts/banar/photoview.xml", NULL);
		//p->Add(this); 
		m_parent = p;
		m_hasinited = false;
		g_player = this;
		m_ismoving = false;
		m_isloadend = false;
		m_taskwaittoload = -1;
		auto callback = nbase::Bind(&CPhotoViewer::CreateControl, this, std::placeholders::_1);
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/banar/photoview.xml", callback);
		m_parent->Add(this);
	}
	

	CPhotoViewer::~CPhotoViewer() {}


	void CPhotoViewer::Construct(std::vector<NMedia> data) {
		OutputDebugString(L"CPhotoViewer::Construct   ------->");

		if (isBindEvent) {  // 二次构造数据，先停止视频播放
			PauseVideo();
		}

		m_taskwaittoload = -1;
		m_data = data;
		m_hwnd = this->GetWindow()->GetHWND();

		int size = data.size();
		m_PlayControl = dynamic_cast<ui::Label*>(FindSubControl(L"playcontrol"));
 
		ui::HBox *container = dynamic_cast<ui::HBox*>(FindSubControl(L"image_container"));
		container->RemoveAll();
		container->SetAttribute(L"childmargin", g_schildmargin);

		//m_labelserno = dynamic_cast<ui::Label*>(FindSubControl(L"serno"));
		m_bigimage = dynamic_cast<ui::Label*>(FindSubControl(L"bigimage"));
		 
		int fixedwidth = container->GetFixedWidth();
		int itemwidth = (fixedwidth - g_childmargin * (g_numberperscreen - 1)) / g_numberperscreen;
		m_StepLength = itemwidth + g_childmargin;  // 步长
		m_Serno = 0;
		m_LeftMargin = itemwidth * size + g_childmargin * (size - 1) - fixedwidth; //向左移动到此位置，已到底
		m_MiddleNO = g_numberperscreen / 2;  // 到此号，应该移动图片，保持border在中间


		WCHAR dbuf[256];
		wsprintf(dbuf, L"  itemwidth=%d   fixedwidth=%d", itemwidth, fixedwidth);
		OutputDebugString(dbuf);

		int i = 0;
		for (auto &s : m_data) {
			ui::Label  *photo = new ui::Label;
			WCHAR namebuf[32];
			wsprintf(namebuf, L"%d", i++);
			photo->SetName(namebuf);
			photo->SetAttribute(L"name", namebuf);

			photo->SetFixedWidth(itemwidth);

			photo->SetAttribute(L"height", L"stretch");

			photo->SetBkImage(s.imageurl);
			photo->SetVisible(true);
			photo->AttachButtonUp([this](ui::EventArgs* args) {
				OutputDebugString(args->pSender->GetName().c_str());
				wchar_t * pEnd;
				m_Serno = wcstol(args->pSender->GetName().c_str(), &pEnd, 10);
				SetBorder(m_Serno);
				return true;
			});
			container->Add(photo);
			m_vector.push_back(photo);
		}


		if (!isBindEvent) {

			m_cefvideo = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_video"));
			if (m_cefvideo) {
				OutputDebugString(L"CPhotoViewer::Construct  find cef_video.");         //gamevideo.html   cef.html
				m_cefvideo->AttachLoadEnd(nbase::Bind(&CPhotoViewer::OnLoadEnd, this, std::placeholders::_1));
				m_cefvideo->AttachAfterCreated(nbase::Bind(&CPhotoViewer::OnAfterCreated, this, std::placeholders::_1));

				std::wstring url = nbase::win32::GetCurrentModuleDirectory() + L"resources\\themes\\default\\layouts\\banar\\gamevideo.html";
				m_cefvideo->LoadURL(url);
			}

			cef_control_dev_ = dynamic_cast<nim_comp::CefControlBase*>(FindSubControl(L"cef_control_dev"));
			if (!nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
				cef_control_dev_->SetVisible(false);
			// 打开开发者工具
			//m_cefvideo->AttachDevTools(cef_control_dev_);

			m_PlayControl->AttachButtonUp([this](ui::EventArgs* args) {
				OutputDebugString(L"CPhotoViewer::  play video");
				m_cefvideo->SetVisible(true);

				std::wstring url;
				url.append(L"{ \"url\":\"");
				url.append(m_data.at(m_Serno).videourl);
				url.append(L"\"}");
				m_cefvideo->CallJSFunction(L"setsrc", url, ToWeakCallback([this](const std::string& params) {
					nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_hwnd);
				}));
				return true;
			});

			ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
			ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));

			if (leftbtn) {
				if (m_ismoving)  return;
				OutputDebugString(L"CPhotoViewer::CPhotoViewer find left button");
				leftbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CPhotoViewer::CPhotoViewer   left button");
					ClickToLeft();
					return true;
				});
			}

			if (rightbtn) {
				if (m_ismoving)  return;
				OutputDebugString(L"CPhotoViewer::CPhotoViewer find right button");
				rightbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CPhotoViewer::CPhotoViewer   right button");
					ClickToRight();
					return true;
				});
			}
			isBindEvent = true;
		}

		SetBorder(0);
		OutputDebugString(L"CPhotoViewer::Construct   -------> ");
	}


	void CPhotoViewer::Start() {
		SetTimer(m_hwnd, TIMER_PLAY_VIEWER, 3000, Play );
		g_active = true;
	}

	void CPhotoViewer::Stop() {
		g_active = false;
		m_ismoving = false;
		KillTimer(m_hwnd, TIMER_PLAY_VIEWER);
	}

	void CPhotoViewer::DisplayNext() {
		m_ismoving = true;
		WCHAR buf[256];
		wsprintf(buf, L"CPhotoViewer::DisplayNex--->  %d", m_Serno);
		OutputDebugString(buf);
		if (m_Serno >= (int)(m_vector.size()) -1) {
			OutputDebugString(L"has arrived end, reset to start point.");
			ui::UiRect r =m_vector.at(0)->GetMargin();
			r.left = 0;
			m_vector.at(0)->SetMargin(r);
			m_Serno = -1;
		}

		m_Serno++;		 
		SetBorder(m_Serno);
		if (m_Serno > m_MiddleNO) {
			AutoToLeft();
		}
		WCHAR dbuf[256];
		wsprintf(dbuf, L"CPhotoViewer::DisplayNex  %d", m_Serno);
		OutputDebugString(dbuf);
		m_ismoving = false;
	}

 
 
	void CPhotoViewer::SetVisible(bool value) {
		if (m_parent) {
			m_parent->SetVisible(value);
		}
		else {
			__super::SetVisible(value);
		}
	}

 

	void CPhotoViewer::ClickToLeft() {
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();
		WCHAR dbuf[128];
		wsprintf(dbuf, L"CPhotoViewer::ToLeft  m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(dbuf);

		if (m_Serno >= (int)(m_vector.size()) - 1) {
			OutputDebugString(L" border to right end, return.");
			return;
		}
		m_Serno++;
		SetBorder(m_Serno);

		if (abs(r.left) >= m_LeftMargin) {
			OutputDebugString(L" to left value, return");
			return;
		}

		int morevalue = abs(r.left) + m_StepLength - m_LeftMargin;
		if (morevalue > 0) {
			r.left = r.left - m_StepLength + morevalue;
		}
		else {
			r.left = r.left - m_StepLength;
		}


		p->SetMargin(r);
 
		WCHAR d2buf[128];
		wsprintf(d2buf, L"CPhotoViewer::ToLeft m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(d2buf);
	}

	void CPhotoViewer::ClickToRight() {
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();

		WCHAR d2buf[128];
		wsprintf(d2buf, L"CPhotoViewer::ToRight  m_Serno = %d    %d,    ", m_Serno, r.left);
		OutputDebugString(d2buf);

		if (m_Serno <= 0 ) {
			OutputDebugString(L" border to right end, return.");
			return;
		}
		m_Serno--;
		SetBorder(m_Serno);

		if (r.left >= 0) {     
			r.left = 0;
			p->SetMargin(r);
			OutputDebugString(L" to right value, return");
			return;
		}

		int morevalue = r.left + m_StepLength;
		if (morevalue > 0) {
			r.left = 0;
		}
		else {
			r.left = r.left + m_StepLength;
		}

		p->SetMargin(r);
 

		WCHAR dbuf[128];
		wsprintf(dbuf, L"CPhotoViewer::ToRight m_Serno = %d    %d ", m_Serno, r.left);
		OutputDebugString(dbuf);
	}

	void CPhotoViewer::ToLeft() {
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();
		WCHAR dbuf[128];
		wsprintf(dbuf, L"CPhotoViewer::ToLeft  m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(dbuf);
 
		if (abs(r.left) >= m_LeftMargin) {
			OutputDebugString(L" to left value, return");
			return;
		}

		int morevalue = abs(r.left) + m_StepLength - m_LeftMargin;
		if (morevalue > 0) {
			r.left = r.left - m_StepLength + morevalue;
		}
		else {
			r.left = r.left - m_StepLength;
		}

		 
		p->SetMargin(r);
		m_Serno++;

		SetBorder(m_Serno);
		WCHAR d2buf[128];
		wsprintf(d2buf, L"CPhotoViewer::ToLeft m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(d2buf);
	}


	void CPhotoViewer::AutoToLeft() {
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();
		WCHAR dbuf[128];
		wsprintf(dbuf, L"CPhotoViewer::AutoToLeft  m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(dbuf);

		if (abs(r.left) >= m_LeftMargin) {
			OutputDebugString(L" to left value, return");
			return;
		}

		int morevalue = abs(r.left) + m_StepLength - m_LeftMargin;
		if (morevalue > 0) {
			r.left = r.left - m_StepLength + morevalue;
		}
		else {
			r.left = r.left - m_StepLength;
		}


		p->SetMargin(r); 
		WCHAR d2buf[128];
		wsprintf(d2buf, L"CPhotoViewer::AutoToLeft m_Serno = %d    %d,   m_LeftMargin=%d", m_Serno, r.left, m_LeftMargin);
		OutputDebugString(d2buf);
	}


	void CPhotoViewer::ToRight() {
		ui::Label *p = m_vector.at(0);
		ui::UiRect  r = p->GetMargin();

		WCHAR d2buf[128];
		wsprintf(d2buf, L"CPhotoViewer::ToRight  m_Serno = %d    %d,    ", m_Serno, r.left  );
		OutputDebugString(d2buf);
		if (r.left >= 0) {     //此处border不会继续左移
			r.left = 0;
			p->SetMargin(r);
			OutputDebugString(L" to right value, return");
			return;
		}

		int morevalue = r.left + m_StepLength;
		if (morevalue > 0) {
			r.left = 0;
		}
		else {
			r.left = r.left + m_StepLength;
		}

		p->SetMargin(r);
		m_Serno--;
		SetBorder(m_Serno);

		WCHAR dbuf[128];
		wsprintf(dbuf, L"CPhotoViewer::ToRight m_Serno = %d    %d ", m_Serno, r.left );
		OutputDebugString(dbuf);
	}

	void CPhotoViewer::SetBorder( int serno) {

		for (auto label : m_vector) {
			label->SetBorderColor(L"");
		}

		m_vector.at(serno)->SetBorderSize(2);
		m_vector.at(serno)->SetBorderColor(L"red");
		m_cefvideo->SetVisible(false);

		PauseVideo();
		if (m_data.at(serno).mediatype == 0) {   // picutre		
			m_cefvideo->SetVisible(false);
			m_PlayControl->SetVisible(false);
		}
		else {      // video
			m_PlayControl->SetVisible(true);
			m_cefvideo->SetVisible(true);
			// load vieo and play
			if (serno == 0 && !m_isloadend) {
				m_taskwaittoload = serno;
			}
			else {
				LoadVideo(serno);
			}
 
		}
 

		WCHAR dbuf[128];
		wsprintf(dbuf, L"  %d   ", serno);
		//m_labelserno->SetText(dbuf);
		m_bigimage->SetBkImage(m_data.at(serno).imageurl);
	}
		 

	void CPhotoViewer::LoadVideo(int serno) {
		 
		std::wstring url;
		url.append(L"{ \"url\":\"");
		url.append(m_data.at(serno).videourl);
		url.append(L"\"}");
		OutputDebugString(L"CPhotoViewer::SetBorder  m_cefvideo setsrc");
		m_cefvideo->CallJSFunction(L"setsrc", url, ToWeakCallback([this](const std::string& params) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_hwnd);
		}));
	}


	ui::Control* CPhotoViewer::CreateControl(const std::wstring& pstrClass)
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

	void CPhotoViewer::CallJS() {
		m_cefvideo->CallJSFunction(L"outnative", L"{\"first\":\"hello\"}", ToWeakCallback([this](const std::string& params) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_hwnd);
		}));
	}

	void CPhotoViewer::PlayVideo() {
		m_cefvideo->CallJSFunction(L"playvideo", L"{\"first\":\"hello\"}", ToWeakCallback([this](const std::string& params) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_hwnd);
		}));
	}


	void CPhotoViewer::PauseVideo() {
		m_cefvideo->CallJSFunction(L"pausevideo", L"{\"first\":\"hello\"}", ToWeakCallback([this](const std::string& params) {
			nim_comp::Toast::ShowToast(nbase::UTF8ToUTF16(params), 3000, m_hwnd);
		}));
	}

	void CPhotoViewer::OnLoadEnd(int httpStatusCode)
	{
		m_isloadend = true;
		if (m_taskwaittoload >= 0) {
			LoadVideo(m_taskwaittoload);
			m_taskwaittoload = -1;
		}
		OutputDebugString(L"CPhotoViewer::OnLoadEnd ------->  %d" ); 
	}

	void  CPhotoViewer::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		OutputDebugString(L"CPhotoViewer::OnAfterCreated ------->");
	}

	/*
	*
	*  @return   false, 正常加载网页；    true， 不加载网页
	*/

	//bool CLoginVBOX::OnBeforeBrowser(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
	//{
	//	//nim_comp::Toast::ShowToast(  request->GetURL().ToWString(), 3000, GetHWND());

	//	OutputDebugString(std::wstring(request->GetURL().ToWString()).c_str());
	//	return false;
	//}

 
 
}

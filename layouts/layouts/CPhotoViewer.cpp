/*
 *date: 2020.03.08
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CPhotoViewer.h"  
#include <math.h>
 

#define  TIMER_PLAY_VIEWER 1005

namespace nui {
	 
	static const int g_childmargin = 20;
	static std::wstring g_schildmargin = L"20";
	static const int  g_numberperscreen = 5;
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
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/banar/photoview.xml", NULL);
		p->Add(this); 
		m_parent = p;
		m_hasinited = false;
		g_player = this;
		m_ismoving = false;
	}
	 
	CPhotoViewer::~CPhotoViewer() {}

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
		if (m_Serno >= m_vector.size() -1) {
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

	void CPhotoViewer::Construct(std::vector<std::wstring> arr) {
		OutputDebugString(L"CPhotoWall::Construct   ------->");

		m_data = arr;
		m_hwnd = this->GetWindow()->GetHWND();
 
		int size = arr.size();

		ui::HBox *container = dynamic_cast<ui::HBox*>(FindSubControl(L"image_container"));
		container->RemoveAll();
		container->SetAttribute(L"childmargin", g_schildmargin);

		m_labelserno = dynamic_cast<ui::Label*>(FindSubControl(L"serno"));
		m_bigimage = dynamic_cast<ui::Label*>(FindSubControl(L"bigimage"));
	  
		int fixedwidth = container->GetFixedWidth();
		int itemwidth = fixedwidth/g_numberperscreen; 
		m_StepLength = itemwidth + g_childmargin;  // 步长
		m_Serno = 0;
		m_LeftMargin = itemwidth * size + g_childmargin * (size-1) - fixedwidth; //向左移动到此位置，已到底
		m_MiddleNO = g_numberperscreen / 2;  // 到此号，应该移动图片，保持border在中间


		WCHAR dbuf[256]; 
		wsprintf(dbuf, L"  itemwidth=%d   fixedwidth=%d",   itemwidth, fixedwidth);
		OutputDebugString(dbuf); 

		int i = 1;
		for (auto &s : arr) {
			ui::Label  *photo = new ui::Label;
			WCHAR namebuf[32]; 
			wsprintf(namebuf, L"name:%d", i++);
			photo->SetName(namebuf);
			photo->SetAttribute(L"name", namebuf);
 
			photo->SetFixedWidth(itemwidth);
 
			photo->SetAttribute(L"height", L"stretch");
 
			photo->SetBkImage(s);  
			photo->SetVisible(true); 
			container->Add(photo); 
			m_vector.push_back(photo); 
		}
		 
 
		SetBorder(0);

		if (!isBindEvent) {
			ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
			ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));

			if (leftbtn) {
				if (m_ismoving)  return;
				OutputDebugString(L"CPhotoWall::CPhotoWall find left button");
				leftbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CPhotoWall::CPhotoWall   left button");
					ToLeft();
					return true;
				});
			}

			if (rightbtn) {
				if (m_ismoving)  return;
				OutputDebugString(L"CPhotoWall::CPhotoWall find right button");
				rightbtn->AttachClick([this](ui::EventArgs* args) {
					OutputDebugString(L"CTurnImage::CTurnImage   right button");
					ToRight();
					return true;
				});
			}
			isBindEvent = true;
		}
 
		OutputDebugString(L"CPhotoWall::Construct   -------> ");
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


		WCHAR dbuf[128];
		wsprintf(dbuf, L"  %d   ", serno);
		m_labelserno->SetText(dbuf);
		m_bigimage->SetBkImage(m_data.at(serno));
	}
		 
 
}

/*
 *date: 2020.03.05
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CTurnImage.h"  
#include "duilib/Animation/AnimationPlayer.h"

#define  TIMER_PLAY_NEXT 1001


namespace nui {

	static CTurnImage * g_player;
	static bool g_active = false;
	CTurnImage::CTurnImage(ui::VBox *p) :CSubVBox(p)
	{ 

		//ui::Box *item = buildSubView(L"layouts/turnimage.xml");
		//p->Add(item);
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/turnimage.xml", NULL);
		p->Add(this);
		g_player = this;
		m_hasinited = false;
	}
	 
	CTurnImage::~CTurnImage() {}

	void CALLBACK PlayNext(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
	{
		if (!g_active)  return;
		OutputDebugString(L"PlayNext ---->");
		g_player->ToRight();
	}

	void CTurnImage::Start() {
		if (!m_hasinited) {
			Construct();
			m_hasinited = true;
		}
		 
		//nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

		//	OutputDebugString(L"settimer ");
		//	g_active = true;
		//	SetTimer(m_hwnd, TIMER_PLAY_NEXT, 3000, PlayNext);
		//}));
    }

	void CTurnImage::Construct() {
 
		m_hwnd = this->GetWindow()->GetHWND();

		WCHAR buf[32];
		wsprintf(buf, L"handler =  %lx", m_hwnd);
		OutputDebugString(buf);

		ui::Control *image = dynamic_cast<ui::Control*>(FindSubControl(L"one"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Control*>(FindSubControl(L"two"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Control*>(FindSubControl(L"three"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Control*>(FindSubControl(L"four"));
		if (image)   m_vector.push_back(image);
 

		ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
		ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));

		if (leftbtn) {
			OutputDebugString(L"CTurnImage::CTurnImage find left button");
			leftbtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CTurnImage::CTurnImage   left button");
				ToLeft();
				//ui::Control *image = dynamic_cast<ui::Control*>(FindSubControl(L"one")); 
				//ui::AnimationPlayer * player = image->GetAnimationManager().GetAnimationPlayer(ui::AnimationType::kAnimationInoutXFromLeft);
				//player->SetStartValue(0);
				//player->SetEndValue(-100);
				//player->SetTotalMillSeconds(3000);
				//player->Start();

				return true;
				//SendMessage(m_hwnd,
				//	WM_USER+1,
				//	NULL,
			});
		}
		else {
			OutputDebugString(L"CTurnImage::CTurnImage can nnot find left button");
		}


		if (rightbtn) {
			OutputDebugString(L"CTurnImage::CTurnImage find right button");
			rightbtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CTurnImage::CTurnImage   right button");
				ToRight();
				return true;
			});
		}

	}
	    
	void CTurnImage::Stop() {
		g_active = false;
		KillTimer(m_hwnd, TIMER_PLAY_NEXT);
	}

	void CTurnImage::ToLeft() {
		int i = 0;
		for (auto v : m_vector) {
			if (v->IsVisible()) {
				v->SetVisible(false);
				break;
			}
			i++;
		}
		i--;

		WCHAR buf[32];
		wsprintf(buf, L"i= %d, size = %d", i, m_vector.size());
		OutputDebugString(buf);
		if (i < 0) {
			i = m_vector.size() - 1;
		}

		m_vector.at(i)->SetVisible(true);
	}

	void CTurnImage::ToRight() {
		int i = 0;
		for (auto v : m_vector) {
			i++;
			if (v->IsVisible()) {
				v->SetVisible(false);
				break;
			}

		}

		WCHAR buf[32];
		wsprintf(buf, L"i= %d, size = %d", i, m_vector.size());
		OutputDebugString(buf);
		if (i >= m_vector.size()) {
			i = 0;
		}

		m_vector.at(i)->SetVisible(true);
	}
		 
 
 

}

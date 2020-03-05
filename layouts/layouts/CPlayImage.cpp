/*
 *date: 2020.03.05
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CPlayImage.h"  
#include "duilib/Animation/AnimationPlayer.h"

#define  TIMER_PLAY_TOLEFT 1002


namespace nui {

	static CPlayImage * g_player;
	static bool g_active = false;
	CPlayImage::CPlayImage(ui::VBox *p) :CSubVBox(p)
	{ 

		//ui::Box *item = buildSubView(L"layouts/turnimage.xml");
		//p->Add(item);
		ui::GlobalManager::FillBoxWithCache(this, L"layouts/playimage.xml", NULL);
		p->Add(this);
		g_player = this;
		m_hasinited = false;
		m_steps = 10;
		m_leftvalue = 0;
		m_turnNumber = 0;
		m_isAnimationRunning = false;
		m_isFirstCircle = true;
		m_animationThread = new AnimationThread(kThreadAni, "Animation Thread");
	}
	 
	CPlayImage::~CPlayImage() {
		delete m_animationThread;
	}

	void CALLBACK PlayToLeft(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
	{
		
		if (!g_active)  return;

		//WCHAR buf[32];
		//wsprintf(buf, L"  timer callback runs in %d", std::this_thread::get_id());
		//OutputDebugString(buf);
		//OutputDebugString(L"PlayNext ---->");
		//g_player->ToLeft();
		g_player->StartMove();
 
	}

	 
	void CPlayImage::StartMove() {

		int  n = 4;
		if (m_isFirstCircle) {
			n = 3;
			m_isFirstCircle = false;
		}
		WCHAR dbuf[128];
        wsprintf(dbuf, L"animation n=  %d        m_turnNumber: %d", n, m_turnNumber);
		OutputDebugString(dbuf);
		if (m_turnNumber >= n) { //全部图片播完，图片复位
			OutputDebugString(L"全部图片播完，图片复位");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(one->GetFixedWidth(), 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 0;
		}
		m_turnNumber++;
		for (int count = 0; count < m_steps; count++) {
			nbase::ThreadManager::PostDelayedTask(kThreadAni, ToWeakCallback([=]() {
 
				nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback ( [this,count]() {
					//WCHAR dbuf[128];
					//wsprintf(dbuf, L"animation task  %d          thread id: %d", count, std::this_thread::get_id());
					int v = m_stepLength;
					if (count == 9)  v += m_leftvalue;
					MoveOneStopToLeft(v);
					//OutputDebugString(dbuf);
				}));
			}) , nbase::TimeDelta::FromMilliseconds(100));
		}
	}

	void CPlayImage::Construct() {
 
		m_hwnd = this->GetWindow()->GetHWND();

 

		ui::Label *image = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Label*>(FindSubControl(L"two"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Label*>(FindSubControl(L"three"));
		if (image)   m_vector.push_back(image);
		image = dynamic_cast<ui::Label*>(FindSubControl(L"four"));
		if (image)   m_vector.push_back(image);
 

		int width = image->GetFixedWidth();
		m_stepLength = width / m_steps;
		m_leftvalue = width % 10;


		WCHAR buf[256];
		wsprintf(buf, L"CPlayImage: handler =  %lx   image width=%d,  stepLegth=%d,  leftvalue=%d", m_hwnd,width,m_stepLength,m_leftvalue);
		OutputDebugString(buf);

		ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
		ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));
		ui::Button * movebtn = dynamic_cast<ui::Button*>(FindSubControl(L"move"));


		if (movebtn) {
			movebtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CPlayImage:::CPlayImage   movebtn  ");
				ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
				ui::UiRect r(-100,0,0,0);
				one->SetMargin(r);

				return true;
			});
		}

		if (leftbtn) { 
			leftbtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CPlayImage:::CPlayImage   left button");
				ToLeft(); 

				return true; 
			});
		}
		else {
			OutputDebugString(L"CPlayImage:::CPlayImage can nnot find left button");
		}


		if (rightbtn) {
			OutputDebugString(L"CPlayImage:::CPlayImage find right button");
			rightbtn->AttachClick([this](ui::EventArgs* args) {
				OutputDebugString(L"CPlayImage:::CPlayImage   right button");
				ToRight();
				return true;
			});
		}

	}
	    
	void CPlayImage::Start() {
		if (!m_hasinited) {
			Construct();
			m_hasinited = true;
		}

		m_isFirstCircle = true;
		WCHAR buf[32];
		wsprintf(buf, L" UI thread id is  %d", std::this_thread::get_id());
		OutputDebugString(buf);
		m_animationThread->Start();
		 
		nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([=]() {

			OutputDebugString(L"settimer ");
			g_active = true;
			SetTimer(m_hwnd, TIMER_PLAY_TOLEFT, 5000, PlayToLeft);
		}));
	}

	void CPlayImage::Stop() {
		g_active = false;
		m_animationThread->Stop();
		 
	}

	void CPlayImage::MoveOneStopToLeft(int value) {
		ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		ui::UiRect   r0 = one->GetMargin();
		ui::UiRect r(r0.left - value, 0, 0, 0);
		one->SetMargin(r);
	}

	void CPlayImage::ToLeft() {
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
		wsprintf(buf, L"CPlayImage::   i= %d, size = %d", i, m_vector.size());
		OutputDebugString(buf);
		if (i < 0) {
			i = m_vector.size() - 1;
		}

		m_vector.at(i)->SetVisible(true);
	}

	void CPlayImage::ToRight() {
		int i = 0;
		for (auto v : m_vector) {
			i++;
			if (v->IsVisible()) {
				v->SetVisible(false);
				break;
			}

		}

		WCHAR buf[32];
		wsprintf(buf, L"CPlayImage::   i= %d, size = %d", i, m_vector.size());
		OutputDebugString(buf);
		if (i >= m_vector.size()) {
			i = 0;
		}

		m_vector.at(i)->SetVisible(true);
	}
		 
 

	AnimationThread::AnimationThread(enum ThreadId thread_id, const char* name)
		: FrameworkThread(name)
		, thread_id_(thread_id)
	{

	}

	AnimationThread::~AnimationThread(void)
	{

	}

	void AnimationThread::Init()
	{
		nbase::ThreadManager::RegisterThread(thread_id_);
	}

	void AnimationThread::Cleanup()
	{
		nbase::ThreadManager::UnregisterThread();
	}

}

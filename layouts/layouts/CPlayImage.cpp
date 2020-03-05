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
		m_turnNumber = 1;
		m_isAnimationRunning = false;
		m_isFirstCircle = true;
		m_animationThread = new AnimationThread(kThreadAni, "Animation Thread");
	}
	 
	CPlayImage::~CPlayImage() {
		delete m_animationThread;
	}


	/*
	*  timer callback
	*/
	void CALLBACK PlayToLeft(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
	{
		
		if (!g_active)  return;
		g_player->StartMove();
 
	}

	 
	void CPlayImage::StartMove() {

		//if (m_turnNumber >= 4) { //È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»
		//	OutputDebugString(L"È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»");
		//	ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		//	ui::UiRect   r0 = one->GetMargin();
		//	ui::UiRect r(one->GetFixedWidth(), 0, 0, 0);
		//	one->SetMargin(r);
		//	m_turnNumber = 0;
		//}

		if (m_turnNumber >= 4) { //È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»
			OutputDebugString(L"È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(0, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 0;
		}

		m_turnNumber++;
		StartAnimation(0);
	}

	/*
	*  Æô¶¯Í¼Æ¬ÇÐ»» animation
	*  @param  direction     0, left;   1, right
	*/
	void  CPlayImage::StartAnimation(int direction) {
		m_isAnimationRunning = true;
		for (int count = 0; count < m_steps; count++) {			 
			int64_t delay = count * 50;
			nbase::ThreadManager::PostDelayedTask(kThreadAni, ToWeakCallback([=]() {

				if (!g_active)  return;
				nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this, count,direction]() {
					if (!g_active)  return;
					int v = m_stepLength;
					if (count == 9)  v += m_leftvalue;
					switch (direction) {
					case 0:
						MoveOneStepToLeft(v,count);
						break;
					case 1:
						MoveOneStepToRight(v,count);
						break;
					}
 
				}));
			}), nbase::TimeDelta::FromMilliseconds(delay));
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

	void CPlayImage::MoveOneStepToLeft(int value,int currentstep) {
		//ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		//ui::UiRect   r0 = one->GetMargin();
		//ui::UiRect r(r0.left - value, 0, 0, 0);
		//one->SetMargin(r);
		ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
		ui::UiRect   r0 = one->GetMargin();
		ui::UiRect r(r0.left - value, 0, 0, 0);
		one->SetMargin(r);

		if (currentstep >= m_steps - 1) {
			OutputDebugString(L"Animation is finished.");
			m_isAnimationRunning = false;
		}
		OutputDebugString(L"CPlayImage::MoveOneStopToLef --->");
	}

	void CPlayImage::MoveOneStepToRight(int value, int currentstep) {
		ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
		ui::UiRect   r0 = one->GetMargin();
		ui::UiRect r(r0.left + value, 0, 0, 0);
		one->SetMargin(r);
		if (currentstep >= m_steps - 1) {
			OutputDebugString(L"Animation is finished.");
			m_isAnimationRunning = false;
		}
		OutputDebugString(L"CPlayImage::MoveOneStepToLeft --->");
	}

	void CPlayImage::ToLeft() {
		if (m_isAnimationRunning)  return;
		if (m_turnNumber >= 4) { //È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»
			OutputDebugString(L"È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(0, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 0;
		}
		m_turnNumber++;
		StartAnimation(0);
	
	}

	void CPlayImage::ToRight() {
		if (m_isAnimationRunning)  return;
		if (m_turnNumber <=1 ) { //È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»
			OutputDebugString(L"È«²¿Í¼Æ¬²¥Íê£¬Í¼Æ¬¸´Î»");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(-one->GetFixedWidth()*5, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 5;
		}
		m_turnNumber--;
		StartAnimation(1);
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

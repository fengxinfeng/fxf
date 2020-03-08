/*
 *date: 2020.03.05
 *author: fengxinfeng
*/
#include "stdafx.h"
#include "CPlayImage.h"  
#include "duilib/Animation/AnimationPlayer.h"
#include <math.h>

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

		//if (m_turnNumber >= 4) { //全部图片播完，图片复位
		//	OutputDebugString(L"全部图片播完，图片复位");
		//	ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		//	ui::UiRect   r0 = one->GetMargin();
		//	ui::UiRect r(one->GetFixedWidth(), 0, 0, 0);
		//	one->SetMargin(r);
		//	m_turnNumber = 0;
		//}
		m_isAnimationRunning = true;
		if (m_turnNumber >= 4) { //全部图片播完，图片复位
			OutputDebugString(L"全部图片播完，图片复位");
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
	*  启动图片切换 animation
	*  @param  direction     0, left;   1, right
	*/
	void  CPlayImage::StartAnimation(int direction) {
		m_isAnimationRunning = true;
		SwitchTaps(m_turnNumber );
		for (int count = 0; count < m_steps; count++) {			 
			int64_t delay = count * 50;
			nbase::ThreadManager::PostDelayedTask(kThreadAni, ToWeakCallback([=]() {

				if (!g_active)  return;
				nbase::ThreadManager::PostTask(kThreadUI, ToWeakCallback([this, count,direction]() {
					if (!g_active)  return;
					int v = m_stepLength;
					if (count == m_steps -1)  v += m_leftvalue;
					switch (direction) {
					case 0:
						MoveOneStepToLeft(v,count);
						break;
					case 1:
						MoveOneStepToRight(v,count);
						break;
					}
					if (count >= m_steps - 1) {
						OutputDebugString(L"Animation is finished.");
						m_isAnimationRunning = false;
					}
 
				}));
			}), nbase::TimeDelta::FromMilliseconds(delay));
		}

	}


	/*
*  快速切换 animation
*  @param  direction     0, left;   1, right
*  @param steps   移动步数
*  @ steplength   移动步长
*/
	void  CPlayImage::FastAnimation(int direction, int steps, int steplength, int leftlength) {
		m_isAnimationRunning = true;
		SwitchTaps(m_turnNumber);
		for (int count = 0; count < steps; count++) {
			int64_t delay = count * 50;
 
				if (!g_active)  return;
				nbase::ThreadManager::PostDelayedTask(kThreadUI, ToWeakCallback([this, count, steps, steplength, leftlength, direction]() {
					if (!g_active)  return;
					int v = steplength;
					if (count == steps -1)  v += leftlength;
					switch (direction) {
					case 0:
						MoveOneStepToLeft(v, count);
						break;
					case 1:
						MoveOneStepToRight(v, count);
						break;
					}
					if (count >= steps - 1) {
						OutputDebugString(L"Animation is finished.");
						m_isAnimationRunning = false;
					}

				}), nbase::TimeDelta::FromMilliseconds(delay));
		}

	}


	void CPlayImage::Construct() {
 
		m_hwnd = this->GetWindow()->GetHWND();

 
		//ui::HBox *container = dynamic_cast<ui::HBox*>(FindSubControl(L"image_container"));
		//int boxwidth = container->GetFixedWidth();

		ui::Label *image = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		if (image) {
			//image->SetFixedWidth(boxwidth);
			m_vector.push_back(image);
		}
		image = dynamic_cast<ui::Label*>(FindSubControl(L"two"));
		if (image) {
			//image->SetFixedWidth(boxwidth);
			m_vector.push_back(image);
		}
		image = dynamic_cast<ui::Label*>(FindSubControl(L"three"));
		if (image) {
			//image->SetFixedWidth(boxwidth);
			m_vector.push_back(image);
		}
		image = dynamic_cast<ui::Label*>(FindSubControl(L"four"));
		if (image) {
			//image->SetFixedWidth(boxwidth);
			m_vector.push_back(image);
		}
 
		int width = image->GetFixedWidth();
		m_stepLength = width / m_steps;
		m_leftvalue = width % 10;


		WCHAR buf[256];
		wsprintf(buf, L"CPlayImage: handler =  %lx   image width=%d,  stepLegth=%d,  leftvalue=%d", m_hwnd,  width,m_stepLength,m_leftvalue);
		OutputDebugString(buf);

		ui::Button * leftbtn = dynamic_cast<ui::Button*>(FindSubControl(L"left"));
		ui::Button * rightbtn = dynamic_cast<ui::Button*>(FindSubControl(L"right"));	
		ui::Button * firstbtn = dynamic_cast<ui::Button*>(FindSubControl(L"first"));

		firstbtn->SetAttribute(L"normaltextcolor", L"green");
		ui::Button * secondbtn = dynamic_cast<ui::Button*>(FindSubControl(L"second"));
		ui::Button * thirdbtn = dynamic_cast<ui::Button*>(FindSubControl(L"third"));
		ui::Button * forthbtn = dynamic_cast<ui::Button*>(FindSubControl(L"forth"));
		m_taps.push_back(firstbtn);
		m_taps.push_back(secondbtn);
		m_taps.push_back(thirdbtn);
		m_taps.push_back(forthbtn);
 

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

		if (firstbtn) {
			firstbtn->AttachClick([this](ui::EventArgs* args) {
				if (m_isAnimationRunning) {
					OutputDebugString(L" is animation, return");
					return true;
				}
 
				FastMove(1);
				return true;
			});
		}
		if (secondbtn) {
			secondbtn->AttachClick([this](ui::EventArgs* args) {
				if (m_isAnimationRunning) {
					OutputDebugString(L" is animation, return");
					return true;
				}

				FastMove(2);
				return true;
			});
		}

		if (thirdbtn) {
			thirdbtn->AttachClick([this](ui::EventArgs* args) {
				if (m_isAnimationRunning) {
					OutputDebugString(L" is animation, return");
					return true;
				}

				FastMove(3);
				return true;
			});
		}
		if (forthbtn) {
			forthbtn->AttachClick([this](ui::EventArgs* args) {
				if (m_isAnimationRunning) {
					OutputDebugString(L" is animation, return");
					return true;
				}

				FastMove(4);
				return true;
			});
		}
	}
	    

	void CPlayImage::FastMove(int whichclicked) {
		WCHAR buf[256];

		ui::Label *image = dynamic_cast<ui::Label*>(FindSubControl(L"one"));
		int width = image->GetFixedWidth();
		int steps = 2;
		int steplength = width / steps;
		int leftvalue = width % 10;
		int  times = abs(m_turnNumber - whichclicked);
		wsprintf(buf, L"CPlayImage::: firstbtn clicked   current is %d, steps=%d, length=%d, left=%d, times=%d", m_turnNumber, steps, steplength, leftvalue, times);
		OutputDebugString(buf);

		if (whichclicked < m_turnNumber ) {
			for (int i = 0; i < times; i++) {
				FastToRight(steps, steplength, leftvalue, true);
			}
		}
		else if (whichclicked > m_turnNumber  ) {
			for (int i = 0; i < times; i++) {
				 
				FastToLeft(steps, steplength, leftvalue, true);
			}
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

		//OutputDebugString(L"CPlayImage::MoveOneStopToLef --->");
	}

	void CPlayImage::MoveOneStepToRight(int value, int currentstep) {
		ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
		ui::UiRect   r0 = one->GetMargin();
		ui::UiRect r(r0.left + value, 0, 0, 0);
		one->SetMargin(r);
		OutputDebugString(L"CPlayImage::MoveOneStepToLeft --->");
	}

	void CPlayImage::ToLeft() {
		if (m_isAnimationRunning)  return;
		m_isAnimationRunning = true;
		if (m_turnNumber >= 4) { //全部图片播完，图片复位
			OutputDebugString(L"全部图片播完，图片复位");
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
		m_isAnimationRunning = true;
		if (m_turnNumber <=1 ) { //全部图片播完，图片复位
			OutputDebugString(L"全部图片播完，图片复位");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(-one->GetFixedWidth()*5, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 5;
		}

		m_turnNumber--;
		StartAnimation(1);
		 
	}

	void CPlayImage::FastToLeft(int steps, int steplength, int left, bool atonce ) {
		if (m_isAnimationRunning && !atonce)  return;
		m_isAnimationRunning = true;
		if (m_turnNumber >= 4) { //全部图片播完，图片复位
			OutputDebugString(L"全部图片播完，图片复位");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(0, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 0;
		}

		m_turnNumber++;
		WCHAR buf[32];
		wsprintf(buf, L"FastToLeft   current is %d", m_turnNumber);
		OutputDebugString(buf);
		FastAnimation(0,   steps,   steplength,   left);
		 
	}

	void CPlayImage::FastToRight(int steps, int steplength, int left, bool atonce  ) {
		if (m_isAnimationRunning && !atonce)  return;
		m_isAnimationRunning = true;
		if (m_turnNumber <= 1) { //全部图片播完，图片复位
			OutputDebugString(L"FastToRight 全部图片播完，图片复位");
			ui::Label *one = dynamic_cast<ui::Label*>(FindSubControl(L"beforeone"));
			ui::UiRect   r0 = one->GetMargin();
			ui::UiRect r(-one->GetFixedWidth() * 5, 0, 0, 0);
			one->SetMargin(r);
			m_turnNumber = 5;
		}
		 
		m_turnNumber--;
		WCHAR buf[32];
		wsprintf(buf, L"FastToRight   current is %d", m_turnNumber);
		OutputDebugString(buf);
		FastAnimation(1, steps, steplength, left);
		 
	}
		 
	void CPlayImage::SwitchTaps(int n) {
		WCHAR buf[32];
		wsprintf(buf, L"SwitchTaps:: current number %d", n);
		OutputDebugString(buf);
		for (size_t i = 1; i <= m_taps.size(); i++) {
			if (i == n) {
				m_taps.at(i-1)->SetAttribute(L"normaltextcolor", L"green");
			}
			else {
				m_taps.at(i-1)->SetAttribute(L"normaltextcolor", L"black");
			}
			 
		}
		 
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

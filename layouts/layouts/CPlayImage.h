#pragma once
 
/*
 *date: 2020.03.05
 *author: fengxinfeng
*/
#include "CSubVBox.h"
#include <vector>

namespace  nui {



	class AnimationThread : public nbase::FrameworkThread
	{
	public:
		/**
		* ���캯��
		* @param[in] thread_id �߳�id
		* @param[in] name �߳�����
		*/
		AnimationThread(enum ThreadId thread_id, const char *name);
		~AnimationThread(void);

	private:
		/**
		* �麯������ʼ���߳�
		* @return void	�޷���ֵ
		*/
		virtual void Init() override;

		/**
		* �麯�����߳��˳�ʱ����һЩ������
		* @return void	�޷���ֵ
		*/
		virtual void Cleanup() override;

	private:
		enum ThreadId thread_id_;
	};

	class CPlayImage : public CSubVBox
	{
	public:
		CPlayImage(ui::VBox *p);
		~CPlayImage();
	protected:
		//void HandleMessage(ui::EventArgs &msg);
	public:
		void Start();
		void Stop();
		void StartMove();
	private:
		void ToLeft();
		void ToRight();
		void FastMove(int whichclicked);
		void FastToLeft(int steps, int steplength, int left, bool atonce=false);
		void FastToRight(int steps, int steplength, int left, bool atonce=false);
		void Construct();
		void  MoveOneStepToLeft(int value, int currentstep);
		void  MoveOneStepToRight(int value, int currentstep);
		void  StartAnimation(int direction);
		void  FastAnimation(int direction, int steps, int steplength, int left);
		void  SwitchTaps(int n);
	private:
		bool  m_hasinited;
		int  m_steps;
		int  m_stepLength;
		int  m_leftvalue;
		int  m_turnNumber;;
		bool  m_isAnimationRunning;
		bool  m_isFirstCircle;
		std::vector<ui::Label *> m_vector;
		std::vector<ui::Button *> m_taps;
		HWND m_hwnd;
		AnimationThread * m_animationThread;
		 
	};
}
 


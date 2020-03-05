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
		void ToLeft();
		void ToRight();
		void  MoveOneStopToLeft(int value);
		void StartMove();
	private:
		void Construct();
		bool  m_hasinited;
		int  m_steps;
		int  m_stepLength;
		int  m_leftvalue;
		int  m_turnNumber;;
		bool  m_isAnimationRunning;
		bool  m_isFirstCircle;
		std::vector<ui::Label *> m_vector;
		HWND m_hwnd;
		AnimationThread * m_animationThread;
		 
	};
}
 


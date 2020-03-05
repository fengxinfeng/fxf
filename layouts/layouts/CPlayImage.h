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
		* 构造函数
		* @param[in] thread_id 线程id
		* @param[in] name 线程名字
		*/
		AnimationThread(enum ThreadId thread_id, const char *name);
		~AnimationThread(void);

	private:
		/**
		* 虚函数，初始化线程
		* @return void	无返回值
		*/
		virtual void Init() override;

		/**
		* 虚函数，线程退出时，做一些清理工作
		* @return void	无返回值
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
 


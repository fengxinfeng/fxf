#pragma once

#include "resource.h"

/** @class MainThread
* @brief 主线程（UI线程）类，继承 nbase::FrameworkThread
* @copyright (c) 2015, NetEase Inc. All rights reserved
* @author towik
* @date 2015/1/1

*/
class DownThread : public nbase::FrameworkThread
{
public:
	/**
	* 构造函数
	* @param[in] thread_id 线程id
	* @param[in] name 线程名字
	*/
	DownThread(enum ThreadId thread_id, const char *name);
	~DownThread(void);

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

class MainThread : public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}

private:
	/**
	* 虚函数，初始化主线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，主线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;
	std::unique_ptr<DownThread>	m_DownThread_;
};

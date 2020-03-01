#pragma once

#include <string>
#include "windows.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define CLIENT_LOG_INI			L"setting.ini"
#define MAINLOG_DEF				L"main.log"
#define FOCUS					L"focus.log"

enum {
	LOGDEBUG = 0,
	LOGINFO,
	LOGWARN,
	LOGERROR,
	LOGFATAL,
};

namespace LogHelper
{
	struct stLogSet
	{
		bool bOn; //on or off
		bool bThreadSafe; //thread safe
		wchar_t szBasepath[MAX_PATH]; //if empty, log on exe path
		UINT uLevel;
		UINT uTrancateSize;
	};

	void SetLogSetting(stLogSet & setLog);
	void SetLogSetting(bool bOn, bool tHtread, const wchar_t * path, UINT level, UINT trancatesize);
	void LogToFile(const wchar_t * szFileName, const char * szFileline, DWORD uLevel, const wchar_t* szFormat,   ...);
	void LogToFilePlain(const wchar_t * szFileName, const char * szFileline, DWORD uLevel, const wchar_t* szFormat);
	const char* get_file_name(const char* pathname);

	class SStreamLogBase
	{
	public:
		SStreamLogBase(const wchar_t * wsFileName, DWORD level, const char * pFix);
		~SStreamLogBase();

		wostringstream & GetStream() { return m_strm; }
	protected:
		wostringstream			m_strm;
		wstring					m_wsLogFileName;
		string					m_ssPreffix;
		DWORD					m_nLevel;
	};
	wstring GetLogDirBase();
}


#define _LOGN(filename, level, szFormat, ...)\
	{ char szFileLine[128] = {0};\
	sprintf_s(szFileLine, _countof(szFileLine)-1, "[%s:%d]", LogHelper::get_file_name(__FILE__), __LINE__);\
	LogHelper::LogToFile(filename, szFileLine, level, szFormat, ##__VA_ARGS__);}

#define  LOG _LOGN

#define BASE_TRACE(filename, level, arg)	\
{\
		char szFileLine[128] = { 0 }; \
sprintf_s(szFileLine, _countof(szFileLine) - 1, "[%s:%d]", LogHelper::get_file_name(__FILE__), __LINE__);\
	LogHelper::SStreamLogBase theLog(filename, level, szFileLine);\
	theLog.GetStream() << "\t" << arg << std::endl; \
}

#define LOG_INFO(filename, arg)\
	BASE_TRACE(filename, LOGINFO, arg)

#define LOG_DEBUG(filename, arg)\
	BASE_TRACE(filename, LOGDEBUG, arg)

#define LOG_WARN(filename, arg)\
	BASE_TRACE(filename, LOGWARN, arg)

#define LOG_ERROR(filename, arg)\
	BASE_TRACE(filename, LOGERROR, arg)

#define LOG_FATAL(filename, arg)\
	BASE_TRACE(filename, LOGFATAL, arg)
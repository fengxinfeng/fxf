
#include "LogHelper.h"
#include "StringHelper.h"
#include <fstream> 
#include<chrono>
#include <mutex> 

#ifdef LOG_THREAD_SAFE
std::mutex gLock;
#endif
std::mutex gLock;

using namespace stralgo;
namespace LogHelper
{

	SYSTEMTIME GetCurrentSystemTime()
	{
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);
		return systemTime;
	}

	SYSTEMTIME SysTime2LocalSysTime(SYSTEMTIME st)
	{
		TIME_ZONE_INFORMATION DEFAULT_TIME_ZONE_INFORMATION = { -480 };
		SystemTimeToTzSpecificLocalTime(&DEFAULT_TIME_ZONE_INFORMATION, &st, &st);
		return st;
	}

	SYSTEMTIME GetCurrentLocalSystemTime()
	{
		SYSTEMTIME st = GetCurrentSystemTime();
		return SysTime2LocalSysTime(st);
	}

	wstring	GetModuleDir(HINSTANCE hInstance)
	{
		WCHAR szModulePath[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(hInstance, szModulePath, MAX_PATH);

		wstring	strPath = szModulePath;
		size_t index = strPath.rfind((const WCHAR*)L"\\");
		if (index > 0)
		{
			strPath = strPath.substr(0, index + 1);
		}
		else
		{
			strPath += L"\\";
		}

		//strPath.Replace((const WCHAR*)L"/",(const WCHAR*)L"\\");
		return strPath;
	}
// helper

	static stLogSet gLogSetting = {
		true, false, L"",0,0
	};

//	static CriticalSection gLock;

	wstring GetLogDirBase()
	{ 
		wstring wsBase = gLogSetting.szBasepath;
		if (wsBase.empty())
		{
			wsBase = GetModuleDir(NULL);
			//wsBase.append(L"log\\");
		}
		return wsBase;
	}

	void SetLogSetting(bool bOn, bool bThread, const wchar_t * path, UINT level, UINT trancatesize)
	{
		stLogSet theConfig = { bOn, bThread , L"", level, trancatesize };
		if (path)
		{
			wcsncpy(theConfig.szBasepath, path, _countof(theConfig.szBasepath) - 1);
		}
		SetLogSetting(theConfig);
	}

	void SetLogSetting(stLogSet & setLog)
	{
		gLogSetting = setLog;
//		gLock.Init();

		wstring wstrIniPath = GetLogDirBase();
		wchar_t swzInipath[256] = { 0 };
		_snwprintf(swzInipath, _countof(swzInipath), L"%s%s", wstrIniPath.c_str(), CLIENT_LOG_INI);
		wstrIniPath = swzInipath;
		if (_waccess(wstrIniPath.c_str(), 0) == -1)
		{
			return;
		}
		gLogSetting.bOn = (GetPrivateProfileInt(TEXT("Logging"),TEXT("OpenLogging"),0,wstrIniPath.c_str()) != 0);
		gLogSetting.bThreadSafe = (GetPrivateProfileInt(TEXT("Logging"),TEXT("ThreadSafe"),0,wstrIniPath.c_str()) != 0);
		gLogSetting.uLevel = GetPrivateProfileInt(TEXT("Logging"),TEXT("Level"),0,wstrIniPath.c_str());
		gLogSetting.uTrancateSize = GetPrivateProfileInt(TEXT("Logging"),TEXT("TruncateSize"),0,wstrIniPath.c_str());

	}

	wstring GetLevelStr( DWORD uLevel)
	{
		wchar_t table[][64] = {
			L"[DEBUG]",
			L"[INFO]",
			L"[WARN]",
			L"[ERROR]",
			L"[FATAL]",
		};

		int n = _countof(table);
		if (uLevel < _countof(table))
		{
			return table[uLevel];
		}
		return L"[NONE]";
	}

	void LogToFile(const wchar_t * szFileName,  const char * szFileline, DWORD uLevel, const wchar_t* szFormat,   ...)
	{
		if (!gLogSetting.bOn) return;
		wstring wsLogFileName = GetLogDirBase() + szFileName;
#ifdef LOG_THREAD_SAFE
		if (gLogSetting.bThreadSafe)
		{
			gLock.lock();
		}	
#endif


		FILE * pLogFile(NULL);
		_wfopen_s(&pLogFile, wsLogFileName.c_str(), L"a+,ccs=UTF-8"); //ccs=UNICODE doesn't work
		if (pLogFile == NULL)
		{
#ifdef LOG_THREAD_SAFE
			if (gLogSetting.bThreadSafe)
			{
				gLock.unlock();
			}
#endif
			return;
		}
		const int maxsize = 2*1024;
		wchar_t szBuffer[maxsize + 1] = L" ";
		bool bEmpty(false);
		bool bFileBig(false);
		va_list argList;
		va_start(argList, szFormat);
		char * lpChar = NULL;
		// 获取参数值
		va_arg(argList, DWORD);
		va_arg(argList, DWORD);
		bEmpty = (StringHelper::BeginWith(argList, "[") != FALSE);
		va_end(argList);
		if (!bEmpty)
		{
			va_start(argList, szFormat);
			_vsnwprintf_s(szBuffer, maxsize, szFormat, argList);
			va_end(argList);
		}

		SYSTEMTIME sysTime = GetCurrentLocalSystemTime();
		wchar_t swzTime[256] = { 0 };
		_snwprintf(swzTime, _countof(swzTime)-1, L"[%02d-%02d %02d:%02d:%02d] ", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		wstring strTime = swzTime;
		wstring strMsg  = strTime + GetLevelStr(uLevel) + StringHelper::ANSIToUnicode(szFileline) + (bEmpty?szFormat : szBuffer) ;
		//strMsg.append(L"\r\n");

		fwprintf(pLogFile, L"%s", strMsg.c_str());

		if (gLogSetting.uTrancateSize)
		{
			fseek(pLogFile, 0, SEEK_END); //定位到文件末 
			long nFileLen = ftell(pLogFile);
			if (nFileLen >= gLogSetting.uTrancateSize)
			{
				bFileBig = true;
			}
		}

		fclose(pLogFile);  

		if (bFileBig)
		{
			wstring wsNewName = wsLogFileName;
			wchar_t swzTime[256] = { 0 };
			_snwprintf(swzTime, _countof(swzTime) - 1, L"[%02d_%02d_%02d] ", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
			wstring strDay = swzTime;
			strDay.append(L".log");
			StringHelper::Replace(wsNewName, L".log", strDay.c_str());
			_wrename(wsLogFileName.c_str(), wsNewName.c_str());
		}
#ifdef LOG_THREAD_SAFE
		if (gLogSetting.bThreadSafe)
		{
			gLock.unlock();
		}
#endif

	}

	void LogToFilePlain(const wchar_t * szFileName, const char * szFileline, DWORD uLevel, const wchar_t* szFormat)
	{
		if (!gLogSetting.bOn) return;
		wstring wsLogFileName = GetLogDirBase() + szFileName;
#ifdef LOG_THREAD_SAFE
		if (gLogSetting.bThreadSafe)
		{
			gLock.lock();
		}
#endif


		FILE * pLogFile(NULL);
		_wfopen_s(&pLogFile, wsLogFileName.c_str(), L"a+,ccs=UTF-8"); //ccs=UNICODE doesn't work
		if (pLogFile == NULL)
		{
#ifdef LOG_THREAD_SAFE
			if (gLogSetting.bThreadSafe)
			{
				gLock.unlock();
			}
#endif
			return;
		}
		bool bFileBig(false);
		SYSTEMTIME sysTime = GetCurrentLocalSystemTime();
		wchar_t swzTime[256] = { 0 };
		_snwprintf(swzTime, _countof(swzTime) - 1, L"[%02d-%02d %02d:%02d:%02d] ", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		wstring strTime = swzTime;
		wstring strMsg = strTime + GetLevelStr(uLevel) + StringHelper::ANSIToUnicode(szFileline) + szFormat;
		//strMsg.append(L"\r\n");

		fwprintf(pLogFile, L"%s", strMsg.c_str());

		if (gLogSetting.uTrancateSize)
		{
			fseek(pLogFile, 0, SEEK_END); //定位到文件末 
			long nFileLen = ftell(pLogFile);
			if (nFileLen >= gLogSetting.uTrancateSize)
			{
				bFileBig = true;
			}
		}

		fclose(pLogFile);

		if (bFileBig)
		{
			wstring wsNewName = wsLogFileName;
			wchar_t swzTime[256] = { 0 };
			_snwprintf(swzTime, _countof(swzTime) - 1, L"[%02d_%02d_%02d] ", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
			wstring strDay = swzTime;
			strDay.append(L".log");
			StringHelper::Replace(wsNewName, L".log", strDay.c_str());
			_wrename(wsLogFileName.c_str(), wsNewName.c_str());
		}
#ifdef LOG_THREAD_SAFE
		if (gLogSetting.bThreadSafe)
		{
			gLock.unlock();
		}
#endif
	}


	//void logtest(const wchar_t * filename,DWORD level, const wchar_t* szFormat,   ...)
	//{
	//	wchar_t szFileLine[128] = {0};
	//	wsprintfW (szFileLine,  L"[%s:%s]", __FILE__, __LINE__);
	//	LogHelper::LogToFile(filename, szFileLine, level, szFormat, __VA_ARGS__);
	//}
	//#define LOG(format, ...)     fprintf(stdout, format, ##__VA_ARGS__)
	//#define LOG(format, args...) fprintf(stdout, format, ##args)



	SStreamLogBase::SStreamLogBase(const wchar_t * wsFileName, DWORD level, const char * pFix)
	{
		m_wsLogFileName = wsFileName;
		m_nLevel = level;
		m_ssPreffix = pFix;
	}

	SStreamLogBase::~SStreamLogBase()
	{
		if (m_wsLogFileName.size())
		{
			std::wstring wsContent = m_strm.str();
			LogToFilePlain(m_wsLogFileName.c_str(), m_ssPreffix.c_str(), m_nLevel, wsContent.c_str());
		}

	}

	const char* get_file_name(const char* pathname)
	{
		if (pathname == NULL)
			return pathname;

		int32_t size = strlen(pathname);

		char *pos = (char *)pathname + size;

#ifdef WIN32
		while (*pos != '\\' && pos != pathname)
#else
		while (*pos != '/' && pos != pathname)
#endif
			pos--;

		if (pos == pathname)
			return pathname;
		else
			return pos + 1;
	}

}

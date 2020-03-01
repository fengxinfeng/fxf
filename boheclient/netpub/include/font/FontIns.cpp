#include "FontIns.h"
#include <string>
#include<Shlwapi.h>
#include<wchar.h>

// 安装字体
BOOL WINAPI FontInstall(std::wstring strFontFilePath_)
{
	if (!PathFileExists(strFontFilePath_.c_str()))
	{
		return FALSE;
	}

	// 文件名
	int _nIndex = strFontFilePath_.rfind('\\');
	std::wstring _strFileName = strFontFilePath_.substr(_nIndex+2,strFontFilePath_.size()- _nIndex - 1);

	// 字体名
	_nIndex = _strFileName.rfind('.');\
	std::wstring _strFontName = _strFileName.substr(0,_nIndex);

	// 系统字体路径
	WCHAR _szWinDir[MAX_PATH] = {NULL};
	GetWindowsDirectory(_szWinDir,MAX_PATH); 
	std::wstring _strSysFontPath(_szWinDir);
	_strSysFontPath += L"\\Fonts\\";
	std::wstring  _strSysFontInstall = _strSysFontPath + _strFileName;

	// 拷贝字体到系统字体目录
	if (!CopyFile(strFontFilePath_.c_str(), _strSysFontInstall.c_str(), FALSE))
	{
		//return FALSE;
	}

	// 添加字体到系统字体库
	int _nFontNum = AddFontResource(_strSysFontInstall.c_str());
	if (_nFontNum <= 0)
	{
		//return FALSE;
	}

	// 向注册表注册字体
	HKEY _phkResult;
	DWORD _dwRegResult = 0;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
					0,const_cast<LPWSTR>(L"REG_SZ"), 0, KEY_WRITE, 0, &_phkResult, &_dwRegResult);
	
	int _nLength = _strFileName.size();
	std::wstring _sFileName = _strFileName.c_str();
	BYTE* _byValue = (BYTE*)_sFileName.c_str();
	RegSetValueEx(_phkResult, _strFontName.c_str(),  0, REG_SZ, _byValue, _nLength);

	RegCloseKey(_phkResult);

	return TRUE;
}

// 卸载字体
BOOL WINAPI FontUnInstall(std::wstring strFontName_)
{
	if (strFontName_.empty())
	{
		return FALSE;
	}

	// 系统字体路径
	WCHAR _szWinDir[MAX_PATH] = {NULL};
	GetWindowsDirectory(_szWinDir,MAX_PATH); 
	std::wstring  _strSysFontPath(_szWinDir);
	_strSysFontPath += L"\\Fonts\\";
	std::wstring _strSysFontInstall = _strSysFontPath + strFontName_;

	//删除注册表
	HKEY _regKey;
	LONG _lRlt = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
					L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_ALL_ACCESS, &_regKey);
	if (_lRlt == ERROR_SUCCESS)
	{
		RegDeleteValue (_regKey,strFontName_.c_str());
		RegCloseKey(_regKey);
	}

	//从系统字体表移除字体
	RemoveFontResource(_strSysFontInstall.c_str());
	DeleteFile(_strSysFontInstall.c_str());

	return TRUE;
}
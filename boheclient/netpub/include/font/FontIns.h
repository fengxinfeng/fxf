#pragma once
#include<Windows.h>
#include<string>
// 安装字体
BOOL  WINAPI FontInstall(std::wstring strFontFilePath_);

// 卸载字体
BOOL  WINAPI FontUnInstall(std::wstring strFontName_);

#pragma once
#include<Windows.h>
#include<string>
// ��װ����
BOOL  WINAPI FontInstall(std::wstring strFontFilePath_);

// ж������
BOOL  WINAPI FontUnInstall(std::wstring strFontName_);

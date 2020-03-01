#include<stdafx.h>
#include<commdlg.h>
#include<regex>
#include "CTool.h"


namespace bh_personpub
{

	CTool::CTool()
	{

	}
	CTool::~CTool()
	{

	}

	std::wstring  CTool::SelectPath()
	{
		//     
		OPENFILENAME ofn;      // �����Ի���ṹ��   
		TCHAR szFile[1024]; // �����ȡ�ļ����ƵĻ�������             
		// ��ʼ��ѡ���ļ��Ի���   
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
		// ��ʾ��ѡ���ļ��Ի���   

		if (GetOpenFileName(&ofn))
		{
			return  szFile;
		}
		return L"";
	};

	bool CTool::JudgePhoneNumber(string &PhoneNub)
	{
		bool temp = false;
		regex e("^1(3\\d|47|5([0-3]|[5-9])|8(0|2|[5-9]))\\d{8}$");

		if (regex_match(PhoneNub,e))
		{
			
			return true;
		}
		else
		{
			return false;
		}
	}
}
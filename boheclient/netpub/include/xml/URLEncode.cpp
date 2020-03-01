/*****************************************************************************
Module :     URLEncode.cpp
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: CPP URL Encoder
*****************************************************************************/
///#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#define _CRTDBG_MAP_ALLOC
///#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <algorithm>
#include "URLEncode.h"
#include "StringHelper.h"

using namespace stralgo;
#define MAX_BUFFER_SIZE (4096)
///using namespace Common;

	// HEX Values array
	char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	// UNSAFE String
	std::string CURLEncode::csUnsafeString= "'\"<>%\\^[]`+$,@:;/!#?=&";

	// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
	std::string CURLEncode::Convert(const char &val) 
	{
		std::string csRet;
		csRet += "%";
		csRet += DecToHex(val, 16);	
		return csRet;
	}

	// THIS IS A HELPER FUNCTION.
	// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
	std::string CURLEncode::DecToHex(const char &num, const int &radix)
	{	
		int temp=0;	
		std::string csTmp;
		int num_char;
		num_char = (int) num;

		// ISO-8859-1 
		// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
		// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
		if (num_char < 0)
		{
			num_char = 256 + num_char;
		}

		while (num_char >= radix)
		{
			temp = num_char % radix;
			num_char = (int)floor((float)(num_char / radix));
			csTmp = hexVals[temp];
		}

		csTmp += hexVals[num_char];

		if(csTmp.length() < 2)
		{
			csTmp += '0';
		}

		std::string strdecToHex(csTmp);
		// Reverse the String
		
		std::reverse( strdecToHex.begin(), strdecToHex.end());

		return strdecToHex;
	}

	// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
	// TRUE = UNSAFE, FALSE = SAFE
	bool CURLEncode::IsUnsafe(const char &compareChar)
	{
		bool bcharfound = false;
		char tmpsafeChar = 0;
		size_t m_strLen = 0;

		m_strLen = csUnsafeString.length();
		for(size_t ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
		{
			tmpsafeChar = csUnsafeString.at(ichar_pos); 
			if(tmpsafeChar == compareChar)
			{ 
				bcharfound = true;
				break;
			} 
		}

		int char_ascii_value = 0;
		//char_ascii_value = __toascii(compareChar);
		char_ascii_value = (int) compareChar;

		if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
		{
			return false;
		}
		// found no unsafe chars, return false		
		else
		{
			return true;
		}

		return true;
	}
	// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
	// TO URL ENCODE FORM.
	std::string CURLEncode::URLEncode(const std::string& pcsEncode)
	{
#if 1
		size_t ichar_pos;
		std::string csEncode;
		std::string csEncoded;	
		size_t m_length;

		csEncode = pcsEncode;
		//m_length = csEncode.length();
		m_length = csEncode.size();

		for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
		{
			char ch = csEncode.at(ichar_pos);
			if (ch < ' ') 
			{
				ch = ch;
			}		
			if(!IsUnsafe(ch))
			{
				// Safe Character
				char strTmp[2] = {0}; 
				///sprintf_s(strTmp, sizeof(strTmp) - 1, "%c", ch);
				sprintf_s(strTmp, sizeof(strTmp), "%c", ch);
				strTmp[1] = '\0';

				csEncoded += std::string(strTmp);
			}
			else
			{
				// get Hex Value of the Character
				csEncoded += Convert(ch);
			}
		}

		return csEncoded;
#else
		std::string strResult = URLEncodeSZStr(pcsEncode.c_str());
		return strResult;
#endif
	}

	std::wstring CURLEncode::URLEncode(const std::wstring& vData)
	{
		std::string inner_str = StringHelper::UnicodeToUtf8( vData);

		return StringHelper::Utf8ToUnicode( URLEncode( inner_str));
	}

	std::wstring CURLEncode::URLEncodeLocalPath(const std::wstring& vData)
	{
		std::wstring str = vData.c_str();

		WCHAR szTemp[][2][6] = 
		{
			L"&", L"&amp;"
			//L"%", L"%25",
			//L"#", L"%23"
		};

		for(int i = 0; i < ARRAYSIZE(szTemp); ++i)
		{
			StringHelper::Replace(str, szTemp[i][0], szTemp[i][1]);
		}	

		return str;
	}

	std::string CURLEncode::URLDecode(const std::string& vData)
	{
		std::string strResult = "";
		size_t inlen = vData.length();
		for(size_t i=0;i<inlen;i++)
		{
			if(vData.at(i)=='%')
			{
				i++;
				char c = FromHex1(vData.at(i++));
				c = c << 4;
				c += FromHex1(vData.at(i));
				strResult += c;
			}
			else if(vData.at(i)=='+')
			{
				strResult += ' ';
			}
			else
			{
				strResult += vData.at(i);
			}
		}
		return strResult;
	}

	std::wstring CURLEncode::URLDecode(const std::wstring& vData)
	{
		std::wstring str = vData.c_str();

		wchar_t szTemp[40][2][4] = {		
			L" ",   L"%20",
			L"'",   L"%27",
			L"\"", L"%22",
			L"<", L"%3C",
			L">", L"%3E",
			L"\\", L"%5C",
			L"^", L"%5E",
			L"[",  L"%5B",
			L"]",  L"%5D",
			L"`",  L"%60",
			L"+", L"%2B",
			L"$", L"%24",
			L",", L"%2C",
			L"@", L"%40",
			L":", L"%3A",
			L";", L"%3B",
			L"/", L"%2F",
			L"!", L"%21",
			L"#", L"%23",
			L"?", L"%3F",
			L"=", L"%3D",
			L"&", L"%26",
			L"<", L"%3c",
			L">", L"%3e",
			L"\\", L"%5c",
			L"^", L"%5e",
			L"[",  L"%5b",
			L"]",  L"%5d",
			L"+", L"%2b",
			L",", L"%2c",
			L":", L"%3a",
			L";", L"%3b",
			L"/", L"%2f",
			L"?", L"%3f",
			L"=", L"%3d",
			L"%", L"%25",
			L"{", L"%7b",
			L"}", L"%7d",
			L"{", L"%7B",
			L"}", L"%7D",
		};

		for(int i = 0; i < 40; ++i)
		{
			StringHelper::Replace(str, szTemp[i][1], szTemp[i][0]);
		}

		return str;

		//"'\"<>%\\^[]`+$,@:;/!#?=&{}"
		//"%27%22%3C%3E%25%5C%5E%5B%5D%60%2B%24%2C%40%3A%3B%2F%21%23%3F%3D%26%7b%7d"
	}

	std::string CURLEncode::char2hex( char dec )
	{
		char dig1 = (dec&0xF0)>>4;
		char dig2 = (dec&0x0F);
		if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48 in ascii
		if (10<= dig1 && dig1<=15) dig1+=65-10; //A,65 in ascii
		if ( 0<= dig2 && dig2<= 9) dig2+=48;
		if (10<= dig2 && dig2<=15) dig2+=65-10;

		std::string r;
		r.append( &dig1, 1);
		r.append( &dig2, 1);
		return r;
	}

	std::string CURLEncode::urlencode( const std::string &c )
	{

		std::string escaped;
		int max = c.length();
		for(int i=0; i<max; i++)
		{
			if ( (48 <= c[i] && c[i] <= 57) ||//0-9
				(65 <= c[i] && c[i] <= 90) ||//ABC...XYZ
				(97 <= c[i] && c[i] <= 122) || //abc...xyz
				(c[i]=='~' || c[i]=='-' || c[i]=='_' || c[i]=='.')
				)
			{
				escaped.append( &c[i], 1);
			}
			else
			{
				escaped.append("%");
				escaped.append( char2hex(c[i]) );//converts char 255 to string "FF"
			}
		}
		return escaped;
	}

	uint8_t CURLEncode::toHex(const uint8_t &x)
	{
		return x > 9 ? x + 55 : x + 48;
	}

	char * CURLEncode::URLEncodeSZStr(const char * pszIn)
	{
		const int nLen = strlen(pszIn)*3 + 1;

		char * pszOut = new char[nLen];
		memset(pszOut, 0, nLen);

		register LPBYTE pOutTmp = NULL;
		LPBYTE pOutBuf = NULL;
		register LPBYTE pInTmp = NULL;
		LPBYTE pInBuf =(LPBYTE)pszIn;
		LPBYTE b = 0;

		//alloc out buffer
		pOutBuf = (LPBYTE)pszOut;//new BYTE [nLen  * 3];
		if(pOutBuf)
		{
			pInTmp   = pInBuf;
			pOutTmp = pOutBuf;

			// do encoding
			while (*pInTmp)
			{
				if(isalnum(*pInTmp))
				{
					*pOutTmp++ = *pInTmp;
				}
				else
				{				
					if(isspace(*pInTmp))
					{					
						*pOutTmp++ = '+';
					}
					else
					{
						*pOutTmp++ = '%';
						*pOutTmp++ = toHex(*pInTmp>>4);
						*pOutTmp++ = toHex(*pInTmp%16);
					}
				}
				pInTmp++;
			}
		}

		return pszOut;
	}

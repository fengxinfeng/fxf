#pragma once

#include <set>
#include <algorithm>
#include <vector>
#include <string>
//#include <tchar.h>

using namespace  std;

// 注意： 不可将tstring作为非Exe的导出函数返回值

// #ifdef _WINDLL
// #pragma  message("-------- tstring不能作为DLL中导出函数的返回值 -------- ")
// #endif // _WINDLL

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

//#if !defined(_WIN64)
//#define unsigned __int32  size_t
//#endif

typedef std::vector<wstring> StringVector;

namespace stralgo
{

class StringHelper
{
public:

    // Convert
    static wstring  ANSIToUnicode(const char* pString);
	static wstring  ANSIToUnicode(const std::string pString);
    static wstring  ANSIToUnicode(const char* pString, DWORD dwLen);
    static string   UnicodeToANSI(const wchar_t* pString);
	static string   UnicodeToANSI(const std::wstring pString);
    static string   UnicodeToANSI(const wchar_t* pString, DWORD dwLen);

    static wstring  Utf8ToUnicode(const char* pString);
    static wstring  Utf8ToUnicode(const std::string pString);
    static wstring  Utf8ToUnicode(const char* pString, DWORD dwLen);
    static string   UnicodeToUtf8(const wchar_t* pString);
    static string   UnicodeToUtf8(const std::wstring pString);
    static string   UnicodeToUtf8(const wchar_t* pString, DWORD dwLen);

	static string   Utf8ToANSI(const char* pString);
	static string   Utf8ToANSI(const std::string pString);
	static string   ANSIToUft8(const char* pString);
	static string   ANSIToUft8(const std::string pString);
	static int      EncodeQuoted(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
	static string   DecodeQuoted(const std::string& v_strSrc);

public:

    // Format
    static   tstring     FormatT(const   TCHAR*   szFormat,   ...);
    static   string       FormatA(const   char*   szFormat,   ...);
    static   wstring    FormatW(const wchar_t*   szFormat,   ...);

    // Convert From Int
    static wstring      ConvertFromIntW(INT64 value);
    static wstring      ConvertFromIntW(UINT64 value);
    static wstring      ConvertFromIntW(int value);
    static wstring      ConvertFromIntW(size_t value, bool x64);
    static wstring      ConvertFromIntW(ULONG value);
    static wstring      ConvertFromIntW(double value);
    static wstring      ConvertFromIntW(bool value);

    static string         ConvertFromIntA(INT64 value);
    static string         ConvertFromIntA(UINT64 value);
    static string         ConvertFromIntA(int value);
    static string         ConvertFromIntA(size_t value, bool x64);
    static string         ConvertFromIntA(ULONG value);
    static string         ConvertFromIntA(double value);
    static string         ConvertFromIntA(bool value);

    // Convert To Int
    static INT64 ConvertToInt64(wstring value);
    static int ConvertToInt(wstring value);
    static double ConvertToFloat(wstring value);
    static bool ConvertToBool(wstring value);

    static INT64 ConvertToInt64(string value);
    static int ConvertToInt(string value);
    static double ConvertToFloat(string value);
    static bool ConvertToBool(string value);

public:

    static string	MakeLower( string strToLowered);
    static wstring	MakeLower( wstring strToLowered);

	static char	 MakeLower(char chToLowered);
	static wchar_t	 MakeLower(wchar_t wchToLowered);

    static string	MakeUpper( string strToUppered );
    static wstring	MakeUpper( wstring strToUppered );

	static char	 MakeUpper(char chToUppered);
	static wchar_t	 MakeUpper(wchar_t wchToUppered);

    static string	Reverse(string strToReversed);
    static wstring	Reverse(wstring strToReversed);

    static bool	IsNumberic(const string strNum); 
    static bool	IsNumberic(const wstring strNum); 

	static bool	IsAlpha(const char * strNum); 
	static bool	IsAlpha(const wchar_t * strNum); 

    static string	TrimAll( string strToTrimed);
    static wstring	TrimAll( wstring strToTrimed);

    static string  Left(string strValue, size_t nNum);
    static wstring  Left(wstring strValue, size_t nNum);

    static string  Right(string strValue, size_t nNum);
    static wstring  Right(wstring strValue, size_t nNum);

public:

    static size_t SplitString(const wstring& strText, StringVector& vctToken, LPCTSTR szToken, 
        BOOL bCaseSensitive = FALSE, BOOL bIncludeEmpty = FALSE);

    static BOOL SplitString(const wstring& strString, const StringVector& vctTokens, 
        StringVector& vctResult, BOOL bCaseSensitive = FALSE, BOOL bIncludeEmpty = FALSE);
	
    static void SplitString(std::string strToSplit, char chSplit, std::vector<std::string>& vSplittedString);
    static void SplitString(std::wstring strToSplit, wchar_t chSplit, std::vector<std::wstring>& vSplittedString);


    // srcStr为源字符串，将对它里面的子串进行替换
    // replacedStr为替换字符串
    // replacingStr为将要替换的字符串
    // 函数返回替换后的字符串
    // 比如，如果需要将abcedef中的e替换为m，则用法如下
    // Replace("abcedef", "e", "m");
    static string	Replace( std::string strSrc, const std::string & from, const std::string &to);
	static void Replace(std::wstring& str, const wchar_t* strSrc, const wchar_t* strDst);

	static size_t   ByteLength(LPCTSTR value);
	static size_t   ByteLength(wstring value);
	//pszSrc 长串，pszSub 短串，-1 ：比较长度为短串的全长
	static BOOL BeginWith(const wchar_t *pszSrc, const  wchar_t *pszSub, int length = -1, BOOL bSensitive=FALSE);
	static BOOL EndWith(const wchar_t *pszSrc, const  wchar_t *pszSub, int length = -1, BOOL bSensitive=FALSE);
	static BOOL BeginWith(const char *pszSrc, const  char *pszSub, int length = -1, BOOL bSensitive=FALSE);
	static BOOL EndWith(const char *pszSrc, const  char *pszSub, int length = -1, BOOL bSensitive=FALSE);
private:
    static std::wstring  MultiByteToUnicode(const char* pString, UINT uCodePage);
    static std::string   UnicodeToMultiByte(const wchar_t* pString, UINT uCodePage);
    static std::wstring  MultiByteToUnicode(const char* pString, DWORD dwLen, UINT uCodePage);
    static std::string   UnicodeToMultiByte(const wchar_t* pString, DWORD dwLen, UINT uCodePage);
};

};


#define W2AS stralgo::StringHelper::UnicodeToANSI
#define W2U stralgo::StringHelper::UnicodeToUtf8
#define U2W stralgo::StringHelper::Utf8ToUnicode
#define AS2W stralgo::StringHelper::ANSIToUnicode
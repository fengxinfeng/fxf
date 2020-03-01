//#include "strsafe.h"
#include "windows.h"
#include <tchar.h>
#include <strsafe.h>

#include "StringHelper.h"
using namespace std;

#include <ctime>

namespace stralgo
{

template<class StringType>
void DoReplaceSubstringsAfterOffset(StringType* str,
									typename StringType::size_type start_offset,
									const StringType& find_this,
									const StringType& replace_with,
									bool replace_all) 
{
	if ((start_offset == StringType::npos) || (start_offset >= str->length()))
		return;

	//ASSERT(!find_this.empty());
	if (find_this.empty())
	{
		return;
	}

	for (typename StringType::size_type offs(str->find(find_this, start_offset));
		offs != StringType::npos; 
		offs = str->find(find_this, offs)) 
	{
		str->replace(offs, find_this.length(), replace_with);
		offs += replace_with.length();

		if (!replace_all)
		{
			break;
		}
	}
}

static int g_NoCaseTable[65536] = {0};
int InitNoCaseTable()
{
	for (UINT i = 0; i < 65536; i++)
	{
		g_NoCaseTable[i]=_totlower(i);
	}

	return TRUE;
}

int g_NoCaseTableInit = InitNoCaseTable();

#ifdef _NEED_DEF_UNICODE_STRING_

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;

} UNICODE_STRING;

typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

#else   // _NEED_DEF_UNICODE_STRING_

#endif  // _NEED_DEF_UNICODE_STRING_

//===============================================
// Convert
wstring  StringHelper::MultiByteToUnicode(const char* pString, UINT uCodePage)
{
    if(pString == NULL)
        return wstring();
    return MultiByteToUnicode(pString, strlen(pString), uCodePage);
}

string   StringHelper::UnicodeToMultiByte(const wchar_t* pString, UINT uCodePage)
{
    if(pString == NULL)
        return string();
    return UnicodeToMultiByte(pString, _tcslen(pString), uCodePage);
}

std::wstring StringHelper::MultiByteToUnicode(const char* pString, DWORD dwLen, UINT uCodePage)
{
    if(pString == NULL)
        return wstring();
    wstring wstrResult;
    if(pString == NULL)
        return wstrResult;

    int nBufferSize = ::MultiByteToWideChar(uCodePage, 0, pString, dwLen, NULL, 0);
    if(nBufferSize > 0)
    {
        const size_t nStackBufferSize = 4096;
        wchar_t szStackBuffer[nStackBufferSize];
        wchar_t* pBuffer = (nBufferSize >= nStackBufferSize) ? new wchar_t[nBufferSize + 1] : szStackBuffer;
        nBufferSize = ::MultiByteToWideChar(uCodePage, 0, pString, dwLen, pBuffer, nBufferSize);
        if(nBufferSize > 0)
        {
            pBuffer[nBufferSize] = 0;
            wstrResult.assign(pBuffer, nBufferSize);
        }
        if(pBuffer != szStackBuffer)
            delete[] pBuffer;
    }
    return wstrResult;
}

std::string StringHelper::UnicodeToMultiByte(const wchar_t* pString, DWORD dwLen, UINT uCodePage)
{
    if(pString == NULL)
        return string();
    string strResult;
    if(pString == NULL)
        return strResult;

    int nBufferSize = ::WideCharToMultiByte(uCodePage, 0, pString, dwLen, 0, 0, 0, 0);
    if(nBufferSize > 0)
    {
        const size_t nStackBufferSize = 4096;
        char szStackBuffer[nStackBufferSize];
        char* pBuffer = (nBufferSize >= nStackBufferSize) ? new char[nBufferSize + 1] : szStackBuffer;
        nBufferSize = ::WideCharToMultiByte(uCodePage, 0, pString, dwLen, pBuffer, nBufferSize, 0, 0);
        if(nBufferSize > 0)
        {
            pBuffer[nBufferSize] = 0;
            strResult.assign(pBuffer, nBufferSize);
        }
        if(pBuffer != szStackBuffer)
            delete[] pBuffer;
    }

    return strResult;
}

wstring  StringHelper::ANSIToUnicode(const char* pString)
{
    return MultiByteToUnicode(pString, 936).c_str();
}

wstring  StringHelper::ANSIToUnicode(const string pString)
{
	return ANSIToUnicode(pString.c_str());
}

string   StringHelper::UnicodeToANSI(const wchar_t* pString)
{
    return UnicodeToMultiByte(pString, 936).c_str();
}

string   StringHelper::UnicodeToANSI(const wstring pString)
{
	return UnicodeToANSI(pString.c_str());
}

wstring  StringHelper::Utf8ToUnicode(const char* pString)
{
    return MultiByteToUnicode(pString, CP_UTF8).c_str();
}

wstring  StringHelper::Utf8ToUnicode(const string pString)
{
	return Utf8ToUnicode(pString.c_str());
}

string  StringHelper::UnicodeToUtf8(const wchar_t* pString)
{
    return UnicodeToMultiByte(pString, CP_UTF8).c_str();
}

string  StringHelper::UnicodeToUtf8(const wstring pString)
{
	return UnicodeToUtf8(pString.c_str());
}

wstring StringHelper::ANSIToUnicode(const char* pString, DWORD dwLen)
{
    return MultiByteToUnicode(pString, dwLen, 936).c_str();
}

string StringHelper::UnicodeToANSI(const wchar_t* pString, DWORD dwLen)
{
    return UnicodeToMultiByte(pString, dwLen, 936).c_str();
}

wstring StringHelper::Utf8ToUnicode(const char* pString, DWORD dwLen)
{
    return MultiByteToUnicode(pString, dwLen, CP_UTF8).c_str();
}

string StringHelper::UnicodeToUtf8(const wchar_t* pString, DWORD dwLen)
{
    return UnicodeToMultiByte(pString, dwLen, CP_UTF8).c_str();
}

string  StringHelper::Utf8ToANSI(const char* pString)
{
    return UnicodeToANSI(Utf8ToUnicode(pString));
}

string  StringHelper::Utf8ToANSI(const string pString)
{
    return UnicodeToANSI(Utf8ToUnicode(pString));
}

string   StringHelper::ANSIToUft8(const char* pString)
{
    return UnicodeToUtf8(ANSIToUnicode(pString));
}

string   StringHelper::ANSIToUft8(const string pString)
{
   return UnicodeToUtf8(ANSIToUnicode(pString));
}

// Format
tstring StringHelper::FormatT(const   TCHAR*   szFormat,   ...)
{
    const int maxsize = 63*1024;
    TCHAR   szBuffer[ maxsize  +   1]   =   _T( " ");
    va_list   argList;
    va_start(   argList,   szFormat   );
#ifdef _UNICODE
    _vsnwprintf_s(szBuffer,   maxsize,   szFormat,   argList);
#else
    _vsnprintf_s(szBuffer,   maxsize,   szFormat,   argList);
#endif
    va_end(   argList   );

	return   szBuffer;
} 

string StringHelper::FormatA(const   char*   szFormat,   ...)
{
    const int maxsize = 63*1024;
    char   szBuffer[ maxsize  +   1]   =   " ";
    va_list   argList;
    va_start(   argList,   szFormat   );
    _vsnprintf_s(szBuffer,   maxsize,   szFormat,   argList);
    va_end(   argList   );
    return   szBuffer;
} 

wstring StringHelper::FormatW(const wchar_t*   szFormat,   ...)
{
    const int maxsize = 63*1024;
    wchar_t   szBuffer[ maxsize  +   1]   =  L" ";
    va_list   argList;
    va_start(   argList,   szFormat   );
    _vsnwprintf_s(szBuffer,   maxsize,   szFormat,   argList);
    va_end(   argList   );
    return   szBuffer;
} 

// Convert From Int

wstring StringHelper::ConvertFromIntW(INT64 value)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%I64d", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(UINT64 value)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%I64u", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(int value)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%d", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(size_t value, bool x64)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%u", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(ULONG value)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%u", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(double value)
{
	wchar_t szBuffer[256];
	StringCchPrintfW(szBuffer, 256, L"%f", value);
	return szBuffer;
}

wstring StringHelper::ConvertFromIntW(bool value)
{
    return value?_T("true"):_T("false");
}

string StringHelper::ConvertFromIntA(INT64 value)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%I64d", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(UINT64 value)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%I64u", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(int value)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%d", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(size_t value, bool x64)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%u", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(ULONG value)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%u", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(double value)
{
	char szBuffer[256];
	StringCchPrintfA(szBuffer, 256, "%f", value);
	return szBuffer;
}

string StringHelper::ConvertFromIntA(bool value)
{
    return value?"true":"false";
}

//
INT64 StringHelper::ConvertToInt64(wstring value)
{
    return _ttoi64(value.c_str());
}

int StringHelper::ConvertToInt(wstring value)
{
    return _ttoi(value.c_str());
}

double StringHelper::ConvertToFloat(wstring value)
{
    return _tcstod(value.c_str(), 0);
}

bool StringHelper::ConvertToBool(wstring value)
{
    return _T("true") == value;
}

INT64 StringHelper::ConvertToInt64(string value)
{
    return _atoi64(value.c_str());
}

int StringHelper::ConvertToInt(string value)
{
    return atoi(value.c_str());
}

double StringHelper::ConvertToFloat(string value)
{
    return strtod(value.c_str(), 0);
}

bool StringHelper::ConvertToBool(string value)
{
    return "true" == value;
}

// Upper Or Lower
string	StringHelper::MakeLower( string strToLowered)
{
    string strResult = strToLowered;
    std::transform( strResult.begin(), strResult.end(), strResult.begin(),tolower);

    return strResult.c_str();
}

wstring 	StringHelper::MakeLower( wstring strToLowered)
{
    wstring strResult = strToLowered;
    std::transform( strResult.begin(), strResult.end(), strResult.begin(),tolower);
    return strResult.c_str();
}

char	 StringHelper::MakeLower(char chToLowered)
{
	if( chToLowered >= 'A' && chToLowered <='Z')
	{
		return chToLowered + 'a' - 'A';
	}

	return chToLowered;
}

wchar_t	 StringHelper::MakeLower(wchar_t wchToLowered)
{
	if( wchToLowered >= L'A' && wchToLowered <= L'Z')
	{
		return wchToLowered + L'a' - L'A';
	}

	return wchToLowered;
}

string	StringHelper::MakeUpper( string strToUppered )
{
    string strResult = strToUppered;
    std::transform( strResult.begin(), strResult.end(), strResult.begin(),toupper);
    return strResult.c_str();
}

wstring	    StringHelper::MakeUpper( wstring strToUppered )
{
    wstring strResult = strToUppered;
    std::transform( strResult.begin(), strResult.end(), strResult.begin(),toupper);
    return strResult.c_str();
}

char	 StringHelper::MakeUpper(char chToUppered)
{
	if( chToUppered >= 'a' && chToUppered <='z')
	{
		return chToUppered - ('a' - 'A');
	}

	return chToUppered;
}

wchar_t	 StringHelper::MakeUpper(wchar_t wchToUppered)
{
	if( wchToUppered >= L'a' && wchToUppered <=L'z')
	{
		return wchToUppered - (L'a' - L'A');
	}

	return wchToUppered;
}

size_t StringHelper::ByteLength(LPCTSTR value)
{
   return lstrlen(value)*sizeof(TCHAR);
}

size_t StringHelper::ByteLength(wstring value)
{
   return value.size()*sizeof(TCHAR);
}

string	StringHelper::Reverse(string strToReversed)
{
    string strResult = strToReversed;
    std::reverse( strResult.begin(), strResult.end());
    return strResult.c_str();
}

wstring	StringHelper::Reverse(wstring strToReversed)
{
    wstring strResult = strToReversed;
    std::reverse( strResult.begin(), strResult.end());
    return strResult.c_str();
}

bool StringHelper::IsNumberic(const string strNum) 
{ 
    return strNum.find_first_not_of("0123456789.") == string::npos;
} 

bool	StringHelper::IsNumberic(const wstring strNum)
{
    return strNum.find_first_not_of(L"0123456789.") == wstring::npos;
}

bool	StringHelper::IsAlpha(const char * strNum)
{
	while (*strNum)
	{
		if (!isalpha(*strNum) && !isdigit(*strNum))
		{
			return false;
		}
		strNum ++;
	}

	return true;
}

bool	StringHelper::IsAlpha(const wchar_t * strNum)
{
	while (*strNum)
	{
		wchar_t wc = *strNum;
		if (wc > 255)
		{
			return false;
		}
		strNum ++;
	}

	return true;
}

string StringHelper::TrimAll( string strToTrimed )
{
    if (strToTrimed.empty())
    {
        return strToTrimed.c_str();
    }

    strToTrimed.erase(0,strToTrimed.find_first_not_of(" \r\n\t"));
    strToTrimed.erase(strToTrimed.find_last_not_of(" \r\n\t") + 1);
    return strToTrimed.c_str();
}

wstring	StringHelper::TrimAll( wstring strToTrimed)
{
    if (strToTrimed.empty())
    {
        return strToTrimed.c_str();
    }

    strToTrimed.erase(0,strToTrimed.find_first_not_of(L" \r\n\t"));
    strToTrimed.erase(strToTrimed.find_last_not_of(L" \r\n\t") + 1);
    return strToTrimed.c_str();
}

string  StringHelper::Left(string strValue, size_t nNum)
{
    if( nNum > strValue.length() || nNum < 0)
        return "";

    int nBeginIndex = 0 ;
    string strResult = strValue.substr(0, nNum);
    return strResult.c_str();
}

wstring  StringHelper::Left(wstring strValue, size_t nNum)
{
    if( nNum > strValue.length() || nNum < 0)
        return L"";

    int nBeginIndex = 0 ;
    wstring strResult = strValue.substr(0, nNum);
    return strResult.c_str();
}

string  StringHelper::Right(string strValue, size_t nNum)
{
    if( nNum > strValue.length() || nNum < 0)
        return "";

    size_t nBeginIndex = strValue.length() - nNum ;
    string strResult = strValue.substr(nBeginIndex, nNum);
    return strResult.c_str();
}

wstring  StringHelper::Right(wstring strValue, size_t nNum)
{
    if( nNum > strValue.length() || nNum < 0)
        return L"";

    size_t nBeginIndex = strValue.length() - nNum ;
    wstring strResult = strValue.substr(nBeginIndex, nNum);
    return strResult.c_str();
}

void StringHelper::Replace(
						   std::wstring& str, 
						   const wchar_t* strSrc, 
						   const wchar_t* strDst
						   )
{
	std::wstring wcsSrc(strSrc);
	std::wstring wcsDst(strDst);
	return DoReplaceSubstringsAfterOffset(&str, 0, wcsSrc, wcsDst, true);
}

string StringHelper::Replace( string strSrc, const string &strReplaced, const string &strReplacing )
{
    string strResult = strSrc; 

    string::size_type pos = 0;
    string::size_type srclen = strReplaced.size();
    string::size_type dstlen = strReplacing.size();

    while( (pos =strSrc.find(strReplaced, pos)) != string::npos )
    {
        strResult.replace( pos, srclen, strReplacing );
        pos += dstlen;
    }

    return strResult.c_str();
}

size_t StringHelper::SplitString(const wstring& strText, StringVector& vctToken, 
                                 LPCTSTR szToken, BOOL bCaseSensitive /* = FALSE */, BOOL bIncludeEmpty /* = FALSE */)
{
    vctToken.clear();

    wstring strTarget = strText;
    wstring strToken(szToken);
    if(!bCaseSensitive)
        std::transform(strTarget.begin(), strTarget.end(), strTarget.begin(), ::_totlower);
    if(!bCaseSensitive)
        std::transform(strToken.begin(), strToken.end(), strToken.begin(), ::_totlower);

    size_t nTokenLenght = strToken.size();

    size_t pos = strTarget.find(strToken);
    if(pos == wstring::npos)
    {
        if(!strText.empty() || bIncludeEmpty)
            vctToken.push_back(strText);
    }
    else
    {
        size_t posLast = 0 - nTokenLenght;
        size_t pos = strTarget.find(strToken, 0);
        wstring strTemp;
        do 
        {
            strTemp = strText.substr(posLast + nTokenLenght, pos - posLast - nTokenLenght);
            if(!strTemp.empty() || bIncludeEmpty)
                vctToken.push_back(strTemp);
            posLast = pos;
            pos = strTarget.find(strToken, pos + 1);
        } while (pos != wstring::npos);

        if(posLast != strText.size() - nTokenLenght)
        {
            strTemp = strText.substr(posLast + nTokenLenght);
            if(!strTemp.empty() || bIncludeEmpty)
                vctToken.push_back(strTemp);
        }
    }

    return vctToken.size();
}

BOOL StringHelper::SplitString(const wstring& strString, const StringVector& vctTokens, 
                               StringVector& vctResult, BOOL bCaseSensitive, BOOL bIncludeEmpty)
{
    vctResult.clear();

    if(strString.empty())
    {
        if(bIncludeEmpty)
            vctResult.push_back(strString);
        return TRUE;
    }

    if(vctTokens.size() == 0)
    {
        if(bIncludeEmpty || !strString.empty())
            vctResult.push_back(strString);
        return TRUE;
    }

    size_t nPos = 0, nLastPos = 0;

    wstring strTarget = strString;
    StringVector vctTargetTokens(vctTokens);

    if(!bCaseSensitive)
    {
        std::transform(strTarget.begin(), strTarget.end(), strTarget.begin(), ::_totlower);
        StringVector::iterator IteToken = vctTargetTokens.begin();
        for(; IteToken != vctTargetTokens.end(); ++ IteToken)
        {
            wstring& strToken = *IteToken;
            std::transform(strToken.begin(), strToken.end(), strToken.begin(), ::_totlower);
        }
    }

    wstring strToken;
    size_t nTokenIndex = 0;
    strToken = vctTokens[nTokenIndex];
    nPos = strTarget.find(strToken);
    if(nPos == wstring::npos)
    {
        if(bIncludeEmpty || !strString.empty())
            vctResult.push_back(strString);
        return TRUE;
    }

    BOOL bResult = FALSE;
    wstring strTemp;
    size_t nTokenCount = vctTokens.size();

    for(; nPos != wstring::npos; nPos = strTarget.find(strToken, nPos))
    {
        strTemp = strString.substr(nLastPos, nPos - nLastPos);
        if(bIncludeEmpty || !strTemp.empty())
            vctResult.push_back(strTemp);
        nPos += strToken.size();
        nLastPos = nPos;
        nTokenIndex = (nTokenIndex + 1) % nTokenCount;
        strToken = vctTokens[nTokenIndex];
    }
    nTokenIndex = (nTokenIndex == 0) ? (nTokenCount - 1) : (nTokenIndex - 1);
    strToken = vctTokens[nTokenIndex];
    strTemp = strString.substr(nLastPos);
    if(strTemp != strToken)
    {
        if(bIncludeEmpty || !strTemp.empty())
            vctResult.push_back(strTemp);
    }

    return bResult;
}

void StringHelper::SplitString(std::string strToSplit, char chSplit, std::vector<std::string>& vSplittedString)
{
    size_t i = 0;
    size_t nLastPos = 0;
    std::string strSub;

    i = strToSplit.find(chSplit);

    while (i != -1)
    {
        strSub = strToSplit.substr(nLastPos, i - nLastPos);
        nLastPos = i + 1;
        i = strToSplit.find(chSplit, nLastPos);

		if (strSub.size())
			vSplittedString.push_back(strSub);
    }

    if (nLastPos != 0)
    {
        strSub = strToSplit.substr(nLastPos, strToSplit.length() - nLastPos);
		if (strSub.size())
			vSplittedString.push_back(strSub);
		
   
    }
}

void StringHelper::SplitString(std::wstring strToSplit, wchar_t chSplit, std::vector<std::wstring>& vSplittedString)
{
    size_t nLastPos = 0;
    std::wstring strSub;

    size_t nCommaIndex = strToSplit.find(chSplit);

    while (nCommaIndex != -1)
    {
        strSub = strToSplit.substr(nLastPos, nCommaIndex - nLastPos);
        nLastPos = nCommaIndex + 1;
        nCommaIndex = strToSplit.find(chSplit, nLastPos);

        vSplittedString.push_back(strSub);
    }

    if (nLastPos != 0)
    {
        strSub = strToSplit.substr(nLastPos, strToSplit.length() - nLastPos);
        vSplittedString.push_back(strSub);
    }
    else
    {
        vSplittedString.push_back(strToSplit);
    }

}

BOOL StringHelper::BeginWith(const wchar_t *pszSrc, const  wchar_t *pszSub, int length /*= -1*/, BOOL bSensitive/*=FALSE*/)
{
	if (!pszSrc || !pszSub) return FALSE;
	const wchar_t * pLong = pszSrc;
	const wchar_t * pShort = pszSub;
	if (wcslen(pszSrc) < wcslen(pszSub))
	{
		pLong = pszSub;
		pShort = pszSrc;
	}

	if (length == -1)
	{
		length = wcslen(pShort);
	}

	return bSensitive ? wcsncmp(pLong, pShort, length) == 0 : _wcsnicmp(pLong, pShort, length) == 0;

}

BOOL StringHelper::EndWith(const wchar_t *pszSrc, const  wchar_t *pszSub, int length /*= -1*/, BOOL bSensitive/*=FALSE*/)
{
	if (!pszSrc || !pszSub) return FALSE;
	const wchar_t * pLong = pszSrc;
	const wchar_t * pShort = pszSub;
	if (wcslen(pszSrc) < wcslen(pszSub))
	{
		pLong = pszSub;
		pShort = pszSrc;
	}

	if (length == -1)
	{
		length = wcslen(pShort);
	}

	const wchar_t * pLongStart = pszSrc+(wcslen(pLong)-length);
	const wchar_t * pShortStart = pszSub+(wcslen(pShort)-length);

	return bSensitive ?  wcsncmp(pLongStart, pShortStart, length) == 0 : _wcsnicmp(pLongStart, pShortStart, length) == 0;
}

BOOL StringHelper::BeginWith(const char *pszSrc, const  char *pszSub, int length /*= -1*/, BOOL bSensitive/*=FALSE*/)
{
	if (!pszSrc || !pszSub) return FALSE;
	const char * pLong = pszSrc;
	const char * pShort = pszSub;
	if (strlen(pszSrc) < strlen(pszSub))
	{
		pLong = pszSub;
		pShort = pszSrc;
	}

	if (length == -1)
	{
		length = strlen(pShort);
	}

	return bSensitive ? strncmp(pLong, pShort, length) == 0 : _strnicmp(pLong, pShort, length) == 0;

}

BOOL StringHelper::EndWith(const char *pszSrc, const  char *pszSub, int length /*= -1*/, BOOL bSensitive/*=FALSE*/)
{
	if (!pszSrc || !pszSub) return FALSE;
	const char * pLong = pszSrc;
	const char * pShort = pszSub;
	if (strlen(pszSrc) < strlen(pszSub))
	{
		pLong = pszSub;
		pShort = pszSrc;
	}

	if (length == -1)
	{
		length = strlen(pShort);
	}

	const char * pLongStart = pszSrc+(strlen(pLong)-length);
	const char * pShortStart = pszSub+(strlen(pShort)-length);

	return bSensitive ?  strncmp(pLongStart, pShortStart, length) == 0 : _strnicmp(pLongStart, pShortStart, length) == 0;
}

int StringHelper::EncodeQuoted(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen) 
{ 
	int nDstLen = 0;
	int nLineLen = 0;
	for (int i = 0; i < nSrcLen; i++, pSrc++) 
	{ 
		// ASCII 33-60, 62-126原样输出，其余的需编码 
		if ((*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '=')) 
		{ 
			*pDst++ = (char)*pSrc; 
			nDstLen++; 
			nLineLen++; 
		} 
		else 
		{ 
			sprintf(pDst, "=%02X", *pSrc); 
			pDst += 3; 
			nDstLen += 3; 
			nLineLen += 3; 
		} 
		// 输出换行？ 
		if (nLineLen >= nMaxLineLen - 3) 
		{ 
			sprintf(pDst, "=\r\n"); 
			pDst += 3; 
			nDstLen += 3; 
			nLineLen = 0; 
		} 
	} 
	// 输出加个结束符 
	*pDst = '\0'; 

	return nDstLen; 
}

string StringHelper::DecodeQuoted(const std::string& v_strSrc) 
{  
	std::string::size_type i = 0;  
	char pDst[sizeof(int)+1] = {0};
	std::string v_strDst;
	const char* pSrc = v_strSrc.c_str();  

	while (i < v_strSrc.size()) {  
		if (strncmp(pSrc+i, "=\r\n", 3) == 0){ // 软回车，跳过  
			i += 3;  
		} else {  
			if (pSrc[i] == '='){ // 是编码字节  
				sscanf(pSrc+i, "=%02X", (unsigned int*)pDst);  
				v_strDst.append(pDst);  
				i += 3;  
			} else{ // 非编码字节  
				v_strDst.push_back(pSrc[i]);  
				i++;  
			}  
		}  
	}
	return v_strDst;
}  

};
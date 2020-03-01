/*****************************************************************************
Module :     URLEncode.h
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
			URL Encode/Decode 类CURLEncode的声明头文件                                                    
			进行URL Encode 编码如空格将转换为 %20和URL Decode解码 如将%20转换为空格 
			支持std::string和std::wstring两种类型的参数.
*****************************************************************************/
#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

#include <string>
#include "stdint.h"
#pragma pack(push,_CRT_PACKING)

	/*
	* URL Encode/Decode 类CURLEncode
	* 进行URL Encode 编码如空格将转换为 %20和URL Decode解码 如将%20转换为空格
	* 支持std::string和std::wstring两种类型的参数.
	*/
	class  CURLEncode
	{
	public:		
		/*
		* 对传入vData进行URL Encode 编码如空格将转换为 %20
		* 和IE上输入的URL转换类似
		* @param [in] const std::string& vData 待转换的字符串
		* @return std::string 转换后的字符串
		*/
		static std::string URLEncode(const std::string& vData);

		/*
		* 对传入vData进行URL Encode 编码如空格将转换为 %20
		* 和IE上输入的URL转换类似
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring 转换后的字符串
		*/
		static std::wstring URLEncode(const std::wstring& vData);
		
		/*
		* 对传入vData进行URL Decode 解码如%20将转换为 空格
		* 和IE上输入的URL转换类似
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring 转换后的字符串
		*/
		static std::wstring URLDecode(const std::wstring& vData);
		
		/*
		* 对传入vData进行URL Decode 解码如%20将转换为 空格
		* 和IE上输入的URL转换类似
		* @param [in] const std::string& vData 待转换的字符串
		* @return std::string 转换后的字符串
		*/
		static std::string URLDecode(const std::string& vData);
		
		/** 对本地路径进行URLEncode,主要用于聊天框中的加载本地html用
		* 这里不需要进行所有字符的Encode只需要进行少数字符的Encode
		* 如:%,#等
		* @param [in] const std::wstring& vData 待转换的字符串
		* @return std::wstring.
		*/
		static std::wstring URLEncodeLocalPath(const std::wstring& vData);

		static std::string char2hex( char dec );
		static std::string urlencode( const std::string &c );

		static uint8_t toHex(const uint8_t &x);
		static char *URLEncodeSZStr(const char * pszIn);
	protected:

		CURLEncode() { };
		virtual ~CURLEncode() { };

	private:
		static std::string csUnsafeString;
		static std::string DecToHex(const char &num, const int &radix);
		static bool IsUnsafe(const char &compareChar);
		static std::string Convert(const char &val);

		static inline uint8_t ToHex1(const uint8_t &x){ return x > 9 ? x + 55: x + 48; }
		static inline uint8_t FromHex1(const uint8_t &x){ return x > 64 ? x - 55 : x - 48; }
	};

///为方便，直接using namespace Common，若有和其它数据结构冲突时可以将下面语句注释掉

#pragma pack(pop)

#endif //__CURLENCODE_H_
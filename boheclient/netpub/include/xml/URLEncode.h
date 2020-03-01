/*****************************************************************************
Module :     URLEncode.h
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
			URL Encode/Decode ��CURLEncode������ͷ�ļ�                                                    
			����URL Encode ������ո�ת��Ϊ %20��URL Decode���� �罫%20ת��Ϊ�ո� 
			֧��std::string��std::wstring�������͵Ĳ���.
*****************************************************************************/
#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

#include <string>
#include "stdint.h"
#pragma pack(push,_CRT_PACKING)

	/*
	* URL Encode/Decode ��CURLEncode
	* ����URL Encode ������ո�ת��Ϊ %20��URL Decode���� �罫%20ת��Ϊ�ո�
	* ֧��std::string��std::wstring�������͵Ĳ���.
	*/
	class  CURLEncode
	{
	public:		
		/*
		* �Դ���vData����URL Encode ������ո�ת��Ϊ %20
		* ��IE�������URLת������
		* @param [in] const std::string& vData ��ת�����ַ���
		* @return std::string ת������ַ���
		*/
		static std::string URLEncode(const std::string& vData);

		/*
		* �Դ���vData����URL Encode ������ո�ת��Ϊ %20
		* ��IE�������URLת������
		* @param [in] const std::wstring& vData ��ת�����ַ���
		* @return std::wstring ת������ַ���
		*/
		static std::wstring URLEncode(const std::wstring& vData);
		
		/*
		* �Դ���vData����URL Decode ������%20��ת��Ϊ �ո�
		* ��IE�������URLת������
		* @param [in] const std::wstring& vData ��ת�����ַ���
		* @return std::wstring ת������ַ���
		*/
		static std::wstring URLDecode(const std::wstring& vData);
		
		/*
		* �Դ���vData����URL Decode ������%20��ת��Ϊ �ո�
		* ��IE�������URLת������
		* @param [in] const std::string& vData ��ת�����ַ���
		* @return std::string ת������ַ���
		*/
		static std::string URLDecode(const std::string& vData);
		
		/** �Ա���·������URLEncode,��Ҫ����������еļ��ر���html��
		* ���ﲻ��Ҫ���������ַ���Encodeֻ��Ҫ���������ַ���Encode
		* ��:%,#��
		* @param [in] const std::wstring& vData ��ת�����ַ���
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

///Ϊ���㣬ֱ��using namespace Common�����к��������ݽṹ��ͻʱ���Խ��������ע�͵�

#pragma pack(pop)

#endif //__CURLENCODE_H_
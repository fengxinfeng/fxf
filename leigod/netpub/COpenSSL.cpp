#include "stdafx.h"
#include "COpenSSL.h"
#include "openssl/des.h"
#include<openssl/ssl.h>
#include<openssl/err.h>
#include "engine_define.h"
#include "zlib/zlib.h"

#define ZLIB_BUFGSIZE  10*1024

int WINAPI ModeDecode(char* in, char* out, int outlen, char* key);
int WINAPI ModeEncode(char* in, char* out, int outlen, char* key);

typedef int(__stdcall	* pModeDecode)(char* in, char* out, int outlen, char* key);
typedef int(__stdcall	* pModeEncode)(char* in, char* out, int outlen, char* key);
typedef int(__stdcall	* pModeHardID)(char* out, int outlen);

namespace leigod_netpub
{
	HINSTANCE gCrypoHandle = NULL;
	COpenSSL::COpenSSL()
	{
	}


	COpenSSL::~COpenSSL()
	{
	}

	bool COpenSSL::DesEncode(string & src, string & dst)
	{
		DES_cblock key;
		DES_key_schedule key_schedule;
		DES_string_to_key(DES_KEY, &key);

		if (DES_set_key_checked(&key, &key_schedule) != 0) 
		{
			return false;
		}

		size_t len = ((src.length()) + 7) / 8 * 8;
		dst.assign(len, 0);
		DES_cblock ivec;

		//DES_ncbc_encrypt((const unsigned char*)src.c_str(), (unsigned char*)dst.data(), src.length(), &key_schedule, &ivec, DES_ENCRYPT);
		DES_ecb_encrypt((const_DES_cblock *)src.c_str(), (DES_cblock *)dst.data(), &key_schedule, DES_ENCRYPT);
		return true;
	}

	bool COpenSSL::DesEncode1(string & src, string & dst)
	{
		DES_cblock key;
		DES_key_schedule key_schedule;
		DES_string_to_key(DES_KEY, &key);

		if (DES_set_key_checked(&key, &key_schedule) != 0)
		{
			return false;
		}

		int nPos = 0;
		while (nPos < src.length() - 8)
		{
			DES_cblock input;
			DES_cblock output;
			memcpy(input, src.data() + nPos, 8);
			DES_ecb_encrypt(&input, &output, &key_schedule, DES_ENCRYPT);
			string ss_temp;
			ss_temp.assign((char*)output, 8);
			dst += ss_temp;
			nPos += 8;
		}

		if (nPos < src.length())
		{
			DES_cblock input;
			memset(input, 0, sizeof(DES_cblock));
			DES_cblock output;
			memcpy(input, src.data() + nPos, src.length() - nPos);
			DES_ecb_encrypt(&input, &output, &key_schedule, DES_ENCRYPT);
			string ss_temp;
			ss_temp.assign((char*)output, 8);
			dst += ss_temp;
		}


		return true;
	}

	bool COpenSSL::DesDecode1(string & src, string & dst)
	{
		DES_cblock key;
		DES_key_schedule key_schedule;
		DES_string_to_key(DES_KEY, &key);

		if (DES_set_key_checked(&key, &key_schedule) != 0)
		{
			return false;
		}

		int nPos = 0;
		while (nPos < src.length()-8)
		{
			DES_cblock input;
			DES_cblock output;
			memcpy(input, src.data() + nPos, 8);
			DES_ecb_encrypt(&input, &output, &key_schedule, DES_DECRYPT);
			string ss_temp;
			ss_temp.assign((char*)output, 8);
			dst += ss_temp;
			nPos += 8;
		}

		if (nPos < src.length())
		{
			DES_cblock input;
			memset(input, 0, sizeof(DES_cblock));
			DES_cblock output;
			memcpy(input, src.data() + nPos, src.length() - nPos);
			DES_ecb_encrypt(&input, &output, &key_schedule, DES_DECRYPT);
			string ss_temp;
			ss_temp.assign((char*)output, 8);
			dst += ss_temp;
		}


		return true;
	}

	bool COpenSSL::DesDecode(string & src, string & dst_ret)
	{
		DES_cblock key;
		DES_key_schedule key_schedule;
		DES_string_to_key(DES_KEY, &key);

		if (DES_set_key_checked(&key, &key_schedule) != 0) 
		{
			return false;
		}

		std::string dst;
		size_t len = ((src.length()) + 7) / 8 * 8;
		dst.assign(len , 0);

		DES_cblock ivec;
		//DES_ncbc_encrypt((const unsigned char*)src.c_str(), (unsigned char*)dst.data(), src.length(), &key_schedule, &ivec, DES_DECRYPT);
		DES_ecb_encrypt((const_DES_cblock *)src.c_str(), (DES_cblock *)dst.data(), &key_schedule, DES_DECRYPT);
		dst_ret = dst;
		//free(buffer);

		return true;
	}

	bool COpenSSL::Base64Encode(string & src, string & dst, bool newLine)
	//char * COpenSSL::Base64Encode(const char *buffer, int length, bool newLine)
	{
		BIO *bmem = NULL;
		BIO *b64 = NULL;
		BUF_MEM *bptr;

		b64 = BIO_new(BIO_f_base64());
		if (!newLine) {
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bmem = BIO_new(BIO_s_mem());
		b64 = BIO_push(b64, bmem);
		BIO_write(b64, src.data(), src.length());
		BIO_flush(b64);
		BIO_get_mem_ptr(b64, &bptr);
		BIO_set_close(b64, BIO_NOCLOSE);

		dst.assign(bptr->length + 1, 0);
		char *buff = (char *)malloc(bptr->length + 1);
		if (buff)
		{
			memcpy(buff, bptr->data, bptr->length);
			buff[bptr->length] = 0;
			dst = buff;
			free(buff);
		}

		BIO_free_all(b64);

		return true;
	}

	bool COpenSSL::Base64Decode(string & src, string & dst, bool newLine)
	//char * COpenSSL::Base64Decode(const char *input, int length, bool newLine)
	{
		BIO *b64 = NULL;
		BIO *bmem = NULL;
		char *buffer = (char *)malloc(src.length());
		if (buffer)
		{
			memset(buffer, 0, src.length());
			b64 = BIO_new(BIO_f_base64());
			if (!newLine) {
				BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
			}
			bmem = BIO_new_mem_buf(src.data(), src.length());
			bmem = BIO_push(b64, bmem);
			BIO_read(bmem, buffer, src.length());
			dst = buffer;
			free(buffer);

			BIO_free_all(bmem);
		}


		return true;
	}

	bool COpenSSL::ZipContent(string & src, string & dst)
	{
		uLong uDest = compressBound(src.size());
		if (uDest)
		{
			char * out_buffer = new char[uDest];
			if (out_buffer)
			{
				int nErr = compress((Bytef *)out_buffer, &uDest, (const Bytef *)src.data(), src.length());
				if (Z_OK == nErr)
				{
					dst.assign(out_buffer, uDest);
					delete out_buffer;
				}

				return (Z_OK == nErr);
			}
		}
		
		return false;
	}

	bool COpenSSL::UnZipContent(string & src, string & dst)
	{

		uLong uReDest = ZLIB_BUFGSIZE;// compressBound(src.size());
		if (uReDest)
		{
			char * renew_buffer = new char[uReDest];
			memset(renew_buffer, 0, uReDest);
			int nErrRenew = uncompress((Bytef *)renew_buffer, &uReDest, (const Bytef *)src.data(), src.length());
			if (Z_OK == nErrRenew)
			{
				dst.assign(renew_buffer, uReDest);
				delete renew_buffer;
				return (Z_OK == nErrRenew);
			}

		}
	
		return false;
	}

	bool COpenSSL::PacketParam(string & src, string & dst)
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsCrypo = wsExepath + L"EncodeTool.dll";
		if (!gCrypoHandle)
			gCrypoHandle = LoadLibraryW(wsCrypo.c_str());
		if (gCrypoHandle)
		{
			pModeEncode pfun_encode = (pModeEncode)GetProcAddress(gCrypoHandle, "ModeEncode");
			if (pfun_encode)
			{
				char * pbuffer = new char[src.size() * 2];
				memset(pbuffer, 0, src.size() * 2);
				pfun_encode((char*)src.data(), pbuffer, src.size() * 2, (char*)DES_KEY);
				dst = pbuffer;
				delete pbuffer;
			}
		}

		return true;
#if 0
		string zip_src;
		if (ZipContent(src, zip_src))
		{
			string des_src;
			if (DesEncode1(zip_src, des_src))
			{
				string base_src;
				if (Base64Encode(des_src, base_src, false))
				{
					dst = base_src;
					return true;
				}
			}
			;
		}

		return false;
#endif

	}

	string COpenSSL::GetHardHash()
	{
		string ssRet;
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsCrypo = wsExepath + L"EncodeTool.dll";
		if (!gCrypoHandle)
			gCrypoHandle = LoadLibraryW(wsCrypo.c_str());
		if (gCrypoHandle)
		{
			pModeHardID pfun_hash = (pModeHardID)GetProcAddress(gCrypoHandle, "ModeHardID");
			if (pfun_hash)
			{
				char buffer[MAX_PATH] = { 0 };
				pfun_hash(buffer, MAX_PATH);
				ssRet = buffer;
			}
		}

		return ssRet;
	}

	void COpenSSL::Release()
	{
		if (gCrypoHandle)
		{
			FreeLibrary(gCrypoHandle);
			gCrypoHandle = NULL;
		}
	}

	bool COpenSSL::UnPacketParam(string & src, string & dst)
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsCrypo = wsExepath + L"EncodeTool.dll";
		if (!gCrypoHandle)
			gCrypoHandle = LoadLibraryW(wsCrypo.c_str());
		if (gCrypoHandle)
		{
			pModeEncode pfun_decode = (pModeEncode)GetProcAddress(gCrypoHandle, "ModeDecode");
			if (pfun_decode)
			{

				int nlength = pfun_decode((char*)src.data(), NULL, 0, (char*)DES_KEY);
				if (nlength)
				{
					char * pbuffer = new char[nlength * 1.2];
					memset(pbuffer, 0, nlength * 1.2);
					pfun_decode((char*)src.data(), pbuffer, nlength, (char*)DES_KEY);
					dst = pbuffer;
					delete pbuffer;
				}

			}
		}

		return true;
#if 0
		string base_dst;
		if (Base64Decode(src, base_dst, false))
		{
			string des_dst;
			if (DesDecode1(base_dst, des_dst))
			{
				string raw_dst;
				rtrim(des_dst);
				if (UnZipContent(des_dst, raw_dst))
				{
					dst = raw_dst;
					return true;
				}
			}
		}
		return false;
#endif

	}

	void rtrim(string& str)
	{
		string::reverse_iterator p = find_if(str.rbegin(), str.rend(), [](char ch) {return ch != 0; });
		str.erase(p.base(), str.end());
	}
	void rtrim_xml(string& str)
	{
		string::reverse_iterator p = find_if(str.rbegin(), str.rend(), [](char ch) {return ch == '>'; });
		str.erase(p.base(), str.end());
		while (str.size())
		{
			if (str[str.length()-1] == '>' && str[str.length() - 2] == '>')
			{
				str.pop_back();
			}
			else
			{
				break;
			}
		}
	}
}
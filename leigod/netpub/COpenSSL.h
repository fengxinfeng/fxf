#pragma once
#include <string>

using namespace std;
namespace leigod_netpub
{

	class COpenSSL
	{
	public:
		COpenSSL();
		~COpenSSL();
		static void Release();
		static bool DesEncode1(string & src, string & dst);
		static bool DesDecode1(string & src, string & dst);
		static bool DesEncode(string & src, string & dst);
		static bool DesDecode(string & src, string & dst);
		static bool Base64Encode(string & src, string & dst, bool newLine);
		static bool Base64Decode(string & src, string & dst, bool newLine);
		static bool ZipContent(string & src, string & dst);
		static bool UnZipContent(string & src, string & dst);
		static bool PacketParam(string & src, string & dst);
		static bool UnPacketParam(string & src, string & dst);
		static string GetHardHash();
	};

	void rtrim(string& str);
	void rtrim_xml(string& str);
}
#include "stdafx.h"
#include "CProtocalHelper.h"
#include "curl/curl.h"
#include "json/json.h"
#include "StringHelper.h"
#include "COpenSSL.h"
#include "duilib/Core/Markup.h"
#include "CAccLogicCenter.h"
#include "atlimage.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include "xml/URLEncode.h"
#include <vector>
#include <math.h>

using namespace std;
using namespace stralgo;

#define PARAM_H "<?xml version=\"1.0\" encoding=\"UTF-8\"?><methodCall><methodName>Execute</methodName><params>"
#define PARAM_T "</params></methodCall>"
int get_rand_plat();

namespace leigod_netpub {


	CURLcode curl_body_json_req(const string &url, string params, string &response);
	CURLcode curl_get_req_ex(const std::string &url, std::string &response, int timeout);
	CURLcode curl_post_req(const string &url, std::vector<std::pair<std::string, std::string> >& parms, string &response);
	CURLcode curl_postfile_req(const string &url, std::vector<std::pair<std::string, std::string> >& parms, string file_path, string &response);

	time_t StringToDatetime(string str);

	string GetFileNameOnPath(const string & path);
	wstring GetFileNameOnPath(const wstring & path);
	wstring GetDefaultLogoPath();
	wstring GetDefaultBGPath();

	CProtocalHelper::CProtocalHelper()
	{
		//test
		//uint32_t uAvg = GET_AVG_DELAY("47.100.86.2");
		//string ssHash = getHardHashCode();
		//int nHour(0); int nMin(0);core://game/subuser_success:425131:
//		wstring  wsCntent(L"core://game/subuser_success:425131:49%e5%b0%8f%e6%97%b642%e5%88%86%e9%92%9f");
//		wstring ws = CURLEncode::URLDecode(wsCntent);
		//wstring was = AS2W(ws);
		//wstring wus = U2W(ws);
		//GET_H_M(wsCntent, nHour, nMin);
	}


	CProtocalHelper::~CProtocalHelper()
	{
	}


	bool CProtocalHelper::GetGeoIP()
	{
		bool ret(false);
		for (int i = 0; i < GEO_TRY_TIMES; i++)
		{
			string ssReq = get_geo_dyn();
			string ssResponse;
			CURLcode err = curl_get_req_ex(ssReq, ssResponse, 6);
			if (err == CURLE_OK && ssResponse.size())
			{
				STGeoIP stGeoIP;
				if (PraseBase(ssResponse, stGeoIP))
				{
					 LOGIC_CENTER()->GetGeoIP() = stGeoIP;
					 ret = true;
					break;
				}
				
			}
		}

		return ret;
	}

	bool CProtocalHelper::LoginPin(STParamLoginPin & params)
	{
		LOG_INFO(MAINLOG_DEF, L"start login pin:" << U2W(params.ssPin));
		bool bRet(false);
		params.ssHardID = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParamRenew;
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			for (int i = 0; i < PRO_TRY_TIMES; i++)
			{
				CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
				if (err == CURLE_OK && ssResponse.size())
				{
					string ss_res_raw;
					if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
					{
						leigod_netpub::rtrim_xml(ss_res_raw);
						STParamLoginPinResponse &  rspData = LOGIC_CENTER()->GetPinRes();
						if (UnPraseBase(ss_res_raw, rspData))
						{
							bRet = true;
						}
						if (LOG_XML)
						{
							wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\logpin.xml";
							LOG_FILE(wsDir, ss_res_raw);
						}
						break;
					}
					else
					{
				
						assert(0);
					}
					;
			}

			}
		}
		else
		{
			assert(0);
		}
		;

		STParamLoginPinResponse &  rspData = LOGIC_CENTER()->GetPinRes();
		if (rspData.IsValid())
		{
			LOG_INFO(MAINLOG_DEF, L"start login Pin success:" << U2W(rspData.wsUserTitle));
		}
		else
		{
			LOG_INFO(MAINLOG_DEF, L"start login Pin res:" << rspData.wsErrMsg);
		}
		return bRet;
	}

	bool CProtocalHelper::LoginKey(STParamLoginKey & params)
	{
		LOG_INFO(MAINLOG_DEF, L"start login key:" << U2W(params.ssKey));
		bool bRet(false);
		params.ssHardID = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParamRenew;
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			for (int i = 0; i < PRO_TRY_TIMES; i++)
			{
				CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
				if (err == CURLE_OK && ssResponse.size())
				{
					string ss_res_raw;
					if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
					{
						leigod_netpub::rtrim_xml(ss_res_raw);
						STParamLoginPinResponse &  rspData = LOGIC_CENTER()->GetPinRes();
						if (UnPraseBase(ss_res_raw, rspData))
						{
							bRet = true;
						}
						if (LOG_XML)
						{
							wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\logkey.xml";
							LOG_FILE(wsDir, ss_res_raw);
						}
						break;
					}
					else
					{
						assert(0);
					}
					;
				}
			}

		}
		else
		{
			assert(0);
		}

		STParamLoginPinResponse &  rspData = LOGIC_CENTER()->GetPinRes();
		if (rspData.IsValid())
		{
			LOG_INFO(MAINLOG_DEF, L"start login Pin success:" << U2W(rspData.wsUserTitle));
		}
		else
		{
			LOG_INFO(MAINLOG_DEF, L"start login Pin res:" << rspData.wsErrMsg);
		}
		return bRet;
	}

	bool CProtocalHelper::GetSvrSet(STParamGetSvrSetting & params)
	{
		string ssParaRaw = PacketParam(params);
		string ssParamRenew;
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			for (int i = 0; i < PRO_TRY_TIMES; i++)
			{
				CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
				if (err == CURLE_OK && ssResponse.size())
				{
					string ss_res_raw;
					if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
					{
						leigod_netpub::rtrim_xml(ss_res_raw);
						if (LOG_XML)
						{
							wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\svrsetting.xml";
							LOG_FILE(wsDir, ss_res_raw);
						}
						STSvrSet & rfData = LOGIC_CENTER()->GetSvrSetInfo();
						UnPraseBase(ss_res_raw, rfData);
						break;
					}

				}
			}

		}
		return false;
	}

	bool CProtocalHelper::GetLineGroup(STParamLineGroup & params, STLineGroup & rfData)
	{
		bool bRet(false);
		string ssParaRaw = PacketParam(params);
		string ssParamRenew;
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				string ss_res_raw;
				if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
				{
					leigod_netpub::rtrim_xml(ss_res_raw);
					rfData.vctGroups.clear();
					rfData.nGroupCount = 0;
					bRet = UnPraseBase(ss_res_raw, rfData, params.wsProvName);
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\linegroup.xml";
						LOG_FILE(wsDir, ss_res_raw);
					}

				}
			}
		}
		return bRet;
	}

	bool CProtocalHelper::GetLineList(STParamLineList & params)
	{
		string ssParaRaw = PacketParam(params);
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				string ss_res_raw;
				if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
				{
					leigod_netpub::rtrim_xml(ss_res_raw);
					STLineList rfData;
					UnPraseBase(ss_res_raw, rfData);
				}
			}
		}
		return false;
	}

	bool CProtocalHelper::GetStartAccInfo(STParamStartAccInfo & params)
	{
		bool bRet(false);
		params.ssHardSeries = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				string ss_res_raw;
				if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
				{
					leigod_netpub::rtrim_xml(ss_res_raw);
					STACCInfo & rfData = LOGIC_CENTER()->GetAccInfo();
					rfData.nErrCode = 0;
					bRet = UnPraseBase(ss_res_raw, rfData);
					if (rfData.ssTokenPwd.empty() || rfData.ssTokenPwd.empty())
					{
						bRet = false;
					}
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\accinfoo.xml";
						LOG_FILE(wsDir, ss_res_raw);
					}
				}
			}
		}
		return bRet;
	}

	bool CProtocalHelper::UpdateToken(STUpdateToken & params)
	{
		bool bRet(false);
		params.ssHardSeries = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				string ss_res_raw;
				if (COpenSSL::UnPacketParam(ssResponse, ss_res_raw))
				{
					leigod_netpub::rtrim_xml(ss_res_raw);
	
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\updatetoken.xml";
						LOG_FILE(wsDir, ss_res_raw);
					}
					STUpdateTokenResponse & rfToken = LOGIC_CENTER()->GetUpdateToken();
					if (UnPraseBase(ss_res_raw, rfToken))
					{
						bRet = true;
					}
					
				}
			}
		}
		return bRet;
	}

	bool CProtocalHelper::StartAccLog(STParamLogStart & params)
	{
		bool bRet(false);
		params.ssHardSeries = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				bRet = true;
			}
		}

		return bRet;
	}

	bool CProtocalHelper::EndAccLog(STParamLogEnd & params)
	{
		bool bRet(false);
		params.ssHardSeries = getHardHashCode();
		string ssParaRaw = PacketParam(params);
		string ssParaSecret;
		if (COpenSSL::PacketParam(ssParaRaw, ssParaSecret))
		{
			string ssResponse;
			CURLcode err = curl_body_json_req(PROTO_URL, ssParaSecret, ssResponse);
			if (err == CURLE_OK && ssResponse.size())
			{
				bRet = true;
			}
		}

		return bRet;
	}

	static size_t img_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
	{
		size_t return_size = fwrite(buffer, size, nmemb, (FILE*)stream);
		return return_size;
	}

	bool CProtocalHelper::DownLoadFile(string url_, wstring path)
	{
		string url = stralgo::StringHelper::Utf8ToANSI(url_);
		FILE * pHandFile = NULL;
		_wfopen_s(&pHandFile, path.c_str(), L"wb+");
		if (!pHandFile)
		{
			return false;
		}

		CURL *curl;
		CURLcode res;
		curl_global_init(CURL_GLOBAL_DEFAULT);

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//下载指定的文件
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, img_fwrite);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)pHandFile);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);

			if (CURLE_OK != res) {

				fprintf(stderr, "curl told us %d\n", res);
			}
		}

		fclose(pHandFile); /* 关闭本地文件 */
		curl_global_cleanup();

		return true;
	}


	bool CProtocalHelper::GetGameListChn()
	{
		string ssResponse;

		auto err = curl_get_req_ex(GAMELIST_CHN, ssResponse, 6);
		if (CURLE_OK == err)
		{
			STGameListConfigration & rfLoaclIni = LOGIC_CENTER()->GetCinfigIni();
			if (UnPraseBaseEx(ssResponse, LOGIC_CENTER()->GetGameListChn(), rfLoaclIni.ssLayoutVersion0))
			{
				wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
				wstring wsDir = wsExepath + L"gamelib\\gamesave.xml";
				FILE * pLogFile(NULL);
				pLogFile = _wfopen(wsDir.c_str(), L"w");
				if (pLogFile)
				{
					fwrite(ssResponse.data(), ssResponse.size(), 1, pLogFile);
					fclose(pLogFile);
				}
			}
			
		}

		return false;
	}

	bool CProtocalHelper::GetPlatList()
	{
		bool bRet(false);
		string ssResponse;

		auto err = curl_get_req_ex(GET_PLAT_LIST, ssResponse, 6);
		if (CURLE_OK == err)
		{
			STGamePlat stPlatPlat;
			if (UnPraseBase(ssResponse, stPlatPlat))
			{
				if (LOG_XML)
				{
					wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\platplat.xml";
					LOG_FILE(wsDir, ssResponse);
				}
				if (stPlatPlat.vctPlats.size() > 0)
				{
					STGamePlatItem & platPlat = stPlatPlat.vctPlats[0];
					platPlat.wsPlatName = L"平台";
					platPlat.nPlatID = 0;
					STGamePlat & rfData = LOGIC_CENTER()->GetGamePlat();;
					rfData.vctPlats.insert(rfData.vctPlats.begin(), platPlat);
				}
				bRet = true;
			}
		}

		return bRet;
	}

	bool CProtocalHelper::GetGamePlatList()
	{
		bool bRet(false);
		string ssResponse;

		auto err = curl_get_req_ex(GET_PLAT_LIST, ssResponse, 6);
		if (CURLE_OK == err)
		{
			if (LOGIC_CENTER()->GetCinfigIni().nTestData == 0)
			{
				STGamePlat & rfData = LOGIC_CENTER()->GetGamePlat();;
				if (UnPraseBase(ssResponse, rfData))
				{
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\gameplat.xml";
						LOG_FILE(wsDir, ssResponse);
					}
					bRet = true;
					STGamePlatItem  platPlat;
					platPlat.wsPlatName = L"平台";
					platPlat.nPlatID = GA_PLAT;

					rfData.vctPlats.insert(rfData.vctPlats.begin(), platPlat);
				}
			}
			else if (LOGIC_CENTER()->GetCinfigIni().nTestData == 1)
			{
				STGamePlat & rfData = LOGIC_CENTER()->GetGamePlat();;
				bRet = true;
				STGamePlatItem  platPlat;
				platPlat.wsPlatName = L"平台";
				platPlat.nPlatID = GA_PLAT;

				rfData.vctPlats.insert(rfData.vctPlats.begin(), platPlat);
			}


		}

		return bRet;
	}

	bool CProtocalHelper::GetGameList()
	{
		bool bInChn = LOGIC_CENTER()->GetGeoIP().nRegionCode == 1;
		
		/*国服*/
		{
			bInChn = true;
			string ssGameListUrl = bInChn ? GAMELIST_CHN : GAMELIST_FOR;
			string ssResponse;
			auto err = curl_get_req_ex(ssGameListUrl, ssResponse, 6);
			if (CURLE_OK == err)
			{
				STGameListConfigration & rfLoaclIni = LOGIC_CENTER()->GetCinfigIni();
				STGameList & rfGameLst = LOGIC_CENTER()->GetGameListChn();
				if (UnPraseBaseEx(ssResponse, rfGameLst, rfLoaclIni.ssLayoutVersion0))
				{
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + (bInChn ? L"gamelib\\gamelist1.xml" : L"gamelib\\gamelist0.xml");
						LOG_FILE(wsDir, ssResponse);
					}
					SAVE_INI("lib", "layoutversion1", rfGameLst.ssLastEdit);
					// try downloading ....
					int nCount = min(SAFE_BOUND, rfGameLst.vctGames.size());
					for (int i = 0; i < nCount; i ++)
					{
						STGame & item = rfGameLst.vctGames[i];
						if (item.ssLogoUrl.size())
						{
							item.wsLogoPath = ConvertGamePath(item.ssLogoUrl, bInChn);
							if (!ImageValid(item.wsLogoPath))
							{
								DownLoadFile(item.ssLogoUrl, item.wsLogoPath);
								if (!ImageValid(item.wsLogoPath))
								{
									item.wsLogoPath = GetDefaultLogoPath();
								}
							}
							for (STGameArea & itra : rfGameLst.vctGames[i].vctAreas)
							{
								itra.wsLogoPath = ConvertGamePath(itra.ssLogoUrl, bInChn);
								if (!ImageValid(itra.wsLogoPath))
								{
									DownLoadFile(itra.ssLogoUrl, itra.wsLogoPath);
								}
							}
						}
						else
						{
							item.wsLogoPath = GetDefaultLogoPath();
						}

						if (item.ssBgUrl.size())
						{
							item.wsBgPath = ConvertGamePath(item.ssBgUrl, bInChn);
							if (!ImageValid(item.wsBgPath))
							{
								DownLoadFile(item.ssBgUrl, item.wsBgPath);
								if (!ImageValid(item.wsBgPath))
								{
									item.wsBgPath = GetDefaultBGPath();
								}
							}
						}
						else
						{
							item.wsBgPath = GetDefaultBGPath();
						}

					}
					for (STGame & rfGame : rfGameLst.vctGames)
					{
						rfGame.nPlatID = GA_CHN;
					}

				}
				else  //read from localfile
				{
					wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + (bInChn ? L"gamelib\\gamelist1.xml" : L"gamelib\\gamelist0.xml");
					LoadGameListLocal(wsDir, rfGameLst);
					int nCount = min(SAFE_BOUND, rfGameLst.vctGames.size());
					for (int i = 0; i < nCount; i++)
					{
						STGame & item = rfGameLst.vctGames[i];
						item.wsLogoPath = ConvertGamePath(item.ssLogoUrl, bInChn);
						if (!ImageValid(item.wsLogoPath))
						{
							item.wsLogoPath = GetDefaultLogoPath();
						}
						item.wsBgPath = ConvertGamePath(item.ssBgUrl, bInChn);
						if (!ImageValid(item.wsBgPath))
						{
							item.wsBgPath = GetDefaultBGPath();
						}

						for (STGameArea & itra : rfGameLst.vctGames[i].vctAreas)
						{
							itra.wsLogoPath = ConvertGamePath(itra.ssLogoUrl, bInChn);
							if (!ImageValid(itra.wsLogoPath))
							{
								DownLoadFile(itra.ssLogoUrl, itra.wsLogoPath);
							}
						}
					}
				}

				for (STGame & rfGame : rfGameLst.vctGames)
				{
					rfGame.nPlatID = GA_CHN;
				}
			}
		}


		/*外服*/
		{
			bInChn = false;
			string ssGameListUrl = bInChn ? GAMELIST_CHN : GAMELIST_FOR;
			string ssResponse;
			auto err = curl_get_req_ex(ssGameListUrl, ssResponse, 6);
			if (CURLE_OK == err)
			{
				STGameListConfigration & rfLoaclIni = LOGIC_CENTER()->GetCinfigIni();
				STGameList & rfGameLst = LOGIC_CENTER()->GetGameListFor();
				if (UnPraseBaseEx(ssResponse, rfGameLst, rfLoaclIni.ssLayoutVersion0))
				{
					if (LOG_XML)
					{
						wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + (bInChn ? L"gamelib\\gamelist1.xml" : L"gamelib\\gamelist0.xml");
						LOG_FILE(wsDir, ssResponse);
					}
					SAVE_INI("lib", "layoutversion0", rfGameLst.ssLastEdit);

					// try downloading ....
					int nCount = min(SAFE_BOUND, rfGameLst.vctGames.size());
					for (int i = 0; i < nCount; i++)
					{
						STGame & item = rfGameLst.vctGames[i];
						item.nPlatID = get_rand_plat();
						if (item.ssLogoUrl.size())
						{
							item.wsLogoPath = ConvertGamePath(item.ssLogoUrl, bInChn);
							if (!ImageValid(item.wsLogoPath))
							{
								DownLoadFile(item.ssLogoUrl, item.wsLogoPath);
								if (!ImageValid(item.wsLogoPath))
								{
									item.wsLogoPath = GetDefaultLogoPath();
								}
							}

							for (STGameArea & itra : rfGameLst.vctGames[i].vctAreas)
							{
								itra.wsLogoPath = ConvertGamePath(itra.ssLogoUrl, bInChn);
								if (!ImageValid(itra.wsLogoPath))
								{
									DownLoadFile(itra.ssLogoUrl, itra.wsLogoPath);
								}
							}
						}
						else
						{
							item.wsLogoPath = GetDefaultLogoPath();
						}

						if (item.ssBgUrl.size())
						{
							item.wsBgPath = ConvertGamePath(item.ssBgUrl, bInChn);

							if (!ImageValid(item.wsBgPath))
							{
								DownLoadFile(item.ssBgUrl, item.wsBgPath);
								if (!ImageValid(item.wsBgPath))
								{
									item.wsBgPath = GetDefaultBGPath();
								}
							}
						}
						else
						{
							item.wsBgPath = GetDefaultBGPath();
						}

					}


				}
				else  //read from localfile
				{
					wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + (bInChn ? L"gamelib\\gamelist1.xml" : L"gamelib\\gamelist0.xml");
					LoadGameListLocal(wsDir, rfGameLst);
					int nCount = min(SAFE_BOUND, rfGameLst.vctGames.size());
					for (int i = 0; i < nCount; i++)
					{
						STGame & item = rfGameLst.vctGames[i];
						item.wsLogoPath = ConvertGamePath(item.ssLogoUrl, bInChn);
						if (!ImageValid(item.wsLogoPath))
						{
							item.wsLogoPath = GetDefaultLogoPath();
						}
						item.wsBgPath = ConvertGamePath(item.ssBgUrl, bInChn);
						if (!ImageValid(item.wsBgPath))
						{
							item.wsBgPath = GetDefaultBGPath();
						}

						for (STGameArea & itra : rfGameLst.vctGames[i].vctAreas)
						{
							itra.wsLogoPath = ConvertGamePath(itra.ssLogoUrl, bInChn);
							if (!ImageValid(itra.wsLogoPath))
							{
								DownLoadFile(itra.ssLogoUrl, itra.wsLogoPath);
							}
						}
					}
				}
			}
		}

		return true;
	}

	bool CProtocalHelper::IsGamePicExist(string ssUrl, bool bChn)
	{
		wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\game_pic\\chn\\";
		if (!bChn)
		{
			wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\game_pic\\for\\";
		}
		string ssFileName = GetFileNameOnPath(ssUrl);
		wstring wsDownPath = wsDir + U2W(ssFileName);

		return ImageValid(wsDownPath);
	}

	wstring CProtocalHelper::ConvertGamePath(string ssUrl, bool bChn)
	{
		wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\game_pic\\chn\\";
		if (!bChn)
		{
			wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\game_pic\\for\\";
		}
		string ssFileName = GetFileNameOnPath(ssUrl);
		wstring wsDownPath = wsDir + U2W(ssFileName);

		return (wsDownPath);
	}

	wstring CProtocalHelper::ConvertTailPath(string ssUrl)
	{
		wstring wsDir = ui::PathUtil::GetCurrentModuleDir() + L"gamelib\\game_pic\\";

		string ssFileName = GetFileNameOnPath(ssUrl);
		wstring wsDownPath = wsDir + U2W(ssFileName);

		return (wsDownPath);
	}

	string CProtocalHelper::getHardHashCode()
	{
		return leigod_netpub::COpenSSL::GetHardHash();
	}

	bool ImageValid(wstring wsPath)
	{
		bool bSucc(false);
		{
			ATL::CImage image;
			bSucc = SUCCEEDED(image.Load(wsPath.c_str())) ? true : false;
		}

		return bSucc;
	}

	bool ImageValid(string ssPath)
	{
		bool bSucc(false);
		if (ssPath.empty())
		{
			return false;
		}
		wstring wsPath = U2W(ssPath);
		{
			ATL::CImage image;
			bSucc = SUCCEEDED(image.Load(wsPath.c_str())) ? true : false;
		}

		return bSucc;
	}

	string GetFileNameOnPath(const string & path)
	{
		char sFileName[MAX_PATH] = { 0 };
		char sExt[MAX_PATH] = { 0 };
		_splitpath(path.c_str(), NULL, NULL, sFileName, sExt);
		std::vector<std::string> vSplittedString;
		string ssName = sFileName;
		StringHelper::SplitString(ssName, ';', vSplittedString);
		if (vSplittedString.size() > 1)
		{
			return StringHelper::FormatA("%s%s", vSplittedString[0].c_str(), sExt);
		}
		return StringHelper::FormatA("%s%s", sFileName, sExt);
	}

	wstring GetFileNameOnPath(const wstring & path)
	{
		string ssPath = StringHelper::UnicodeToANSI(path);
		return StringHelper::ANSIToUnicode(GetFileNameOnPath(ssPath));
	}

	//---------------------------   curl   --------------------------
	size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		string *str = (string*)stream;
		(*str).append((char*)ptr, size*nmemb);
		return size * nmemb;
	}
	//set timeout
	CURLcode curl_get_req_ex(const std::string &url, std::string &response, int timeout)
	{
		// init curl  
		CURL *curl = curl_easy_init();
		// res code  
		CURLcode res;
		if (curl)
		{
			// set params  
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 0);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout); // set transport and time out time in second
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
			// start req  
			res = curl_easy_perform(curl);
		}
		// release curl  
		curl_easy_cleanup(curl);
		return res;
	}

	CURLcode curl_post_req(const string &url, std::vector<std::pair<std::string, std::string> >& parms, string &response)
	{
		// init curl  
		CURL *curl = curl_easy_init();
		// res code  
		CURLcode res;
		if (curl)
		{
			curl_httppost *post = NULL;
			curl_httppost *last = NULL;
			// set params  
			curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
															  //		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  
			for (auto itr : parms)
			{
				curl_formadd(&post, &last, CURLFORM_COPYNAME, itr.first.c_str(), CURLFORM_COPYCONTENTS, itr.second.c_str(), CURLFORM_END);
			}

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 0);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
			// start req  
			res = curl_easy_perform(curl);
			curl_formfree(last);

		}
		// release curl  
		curl_easy_cleanup(curl);
		return res;
	}

	string get_filename_onpath(const string & path)
	{
		char sFileName[MAX_PATH] = { 0 };
		char sExt[MAX_PATH] = { 0 };
		_splitpath(path.c_str(), NULL, NULL, sFileName, sExt);

		return StringHelper::FormatA("%s%s", sFileName, sExt);
	}

	wstring get_filename_onpath(const wstring & path)
	{
		return U2W(get_filename_onpath(W2U(path)));
	}

	CURLcode curl_postfile_req(const string &url, std::vector<std::pair<std::string, std::string> >& parms, string file_path, string &response)
	{
		// init curl  
		CURL *curl = curl_easy_init();
		// res code  
		CURLcode res;
		if (curl)
		{
			string ssFileName = get_filename_onpath(file_path);
			curl_httppost *post = NULL;
			curl_httppost *last = NULL;
			// set params  
			curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
															  // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  
			for (auto itr : parms)
			{
				curl_formadd(&post, &last, CURLFORM_COPYNAME, itr.first.c_str(), CURLFORM_COPYCONTENTS, itr.second.c_str(), CURLFORM_END);
			}
			curl_formadd(&post, &last,
				CURLFORM_COPYNAME, "file",
				CURLFORM_FILE, file_path.c_str(),//imagepath
				CURLFORM_CONTENTTYPE, "application/octet-stream",
				CURLFORM_END);
			//	curl_formadd(&post, &last, CURLFORM_PTRNAME, "path", CURLFORM_PTRCONTENTS, "D:", CURLFORM_END);//form-data key(path) 和 value(device_cover)
			curl_formadd(&post, &last,
				CURLFORM_COPYNAME, "filename",
				CURLFORM_COPYCONTENTS, ssFileName.c_str(),
				CURLFORM_END);

			curl_formadd(&post, &last,
				CURLFORM_COPYNAME, "submit",
				CURLFORM_COPYCONTENTS, "OK",
				CURLFORM_END);



			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 0);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
			// start req  
			res = curl_easy_perform(curl);
			curl_formfree(last);

		}
		// release curl  
		curl_easy_cleanup(curl);
		return res;
	}

	CURLcode curl_body_json_req(const string &url, string params, string &response)
	{
		CURL *curl = curl_easy_init();
		// res code  
		CURLcode res;
		if (curl)
		{
			struct curl_slist* http_header = NULL;
			http_header = curl_slist_append(http_header, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/534.3 (KHTML, like Gecko) Chrome/6.0.472.63 Safari/534.3");
			http_header = curl_slist_append(http_header, "Charset: UTF-8");
			http_header = curl_slist_append(http_header, "Connection: keep-alive");//保持长连接
			http_header = curl_slist_append(http_header, "Content-Type:application/json;charset=UTF-8");

			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);//修改协议头
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);//设置接收回调
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);//设置设置参数
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2);//设置连接时的超时时间为5秒
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//指定URL
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
			res = curl_easy_perform(curl);//执行		ret = (CURLE_OK == res) ? TRUE : FALSE;

			curl_slist_free_all(http_header);
			curl_easy_cleanup(curl);
		}

		return res;
	}



	//--------------------------------------------------------   proto base  ------------------------------------------------------------------------
	int GetIntValue(Json::Value & value)
	{
		if (value.isInt())
		{
			return value.asInt();
		}
		else if (value.isString())
		{
			return atoi(value.asCString());
		}
		else if (value.isDouble())
		{
			return (int)value.asDouble();
		}

		return 0;
	}

	int64_t GetIntValue64(Json::Value & value, const char * raw)
	{
		Json::FastWriter writer;
		uint64_t uRet(0);
		if (value.isInt())
		{
			uRet = value.asInt();
		}
		else if (value.isString())
		{
			uRet = _atoi64(value.asCString());
		}
		else if (value.isDouble())
		{
			uRet = (int64_t)value.asDouble();
		}

		string ssRet = StringHelper::ConvertFromIntA(uRet);
		if (strstr(raw, ssRet.c_str()) == NULL)
		{
			int nLenght(12);
			while (nLenght > 8)
			{
				string ssNum = ssRet.substr(0, nLenght);
				const char * pPos = strstr(raw, ssNum.c_str());
				if (pPos == NULL)
				{
					nLenght--;
					continue;
				}

				return  _atoi64(pPos);
			}

		}
		return uRet;
	}


	string GetStrValue(Json::Value & value)
	{
		if (value.isString())
		{
			return StringHelper::TrimAll(value.asString());
		}

		return "";
	}

	double GetDbValue(Json::Value & value)
	{
		if (value.isDouble())
		{
			return value.asDouble();
		}

		return 0.0;
	}

	string PackLine(const char * str)
	{
		return "<param><value>" + string(str) + "</value></param>";
	}
	
	string PackLine(int value)
	{
		return "<param><value><i4>" + std::to_string(value) + "</i4></value></param>";
	}

	int get_isp_type(string ssIsp)
	{
		ssIsp = stralgo::StringHelper::TrimAll(ssIsp);
		int nIsp(0);
		if (StrStrIA(ssIsp.c_str(), "telecom") || ssIsp == "telecom")
		{
			nIsp = ISP_TELCOM;
		}
		else if (StrStrIA(ssIsp.c_str(), "mobile") || ssIsp == "mobile")
		{
			nIsp = ISP_MOBILE;
		}
		else if (StrStrIA(ssIsp.c_str(), "unicom") || ssIsp == "unicom")
		{
			nIsp = ISP_UNICOM;
		}

		return nIsp;
	}

	//............................................................................................................................................//
	bool PraseBase(string & ssContent, STProtoBase & rfData)
	{
		if (ssContent.empty())
		{
			return false;
		}

		Json::Features features;
		Json::Reader reader(features);
		Json::Value root;

		bool bInnerPacket(false);
		bool parsingSuccessful = reader.parse(ssContent.c_str(), root);
		if (parsingSuccessful)
		{
			rfData.nCode = GetIntValue(root["code"]);
			rfData.wsMsg = U2W(GetStrValue(root["msg"]));
		}

		return parsingSuccessful;
	}


	bool UnPraseBase(string & ssContent, STCountryList & rfData)
	{
		if (ssContent.empty())
		{
			return false;
		}

		Json::Features features;
		Json::Reader reader(features);
		Json::Value root;
		bool parsingSuccessful = reader.parse(ssContent.c_str(), root);
		if (parsingSuccessful)
		{
			Json::Value data = root["data"];
			Json::Value topCountry = data["top_country"];
			if (topCountry.isArray())
			{
				int nArrSize = topCountry.size();
				for (int i = 0; i < nArrSize; i++)
				{
					Json::Value item = topCountry[i];
					if (item.isObject())
					{
						STCountry stCountry;
						stCountry.nCode = GetIntValue(item["code"]);
						stCountry.wsName = U2W(GetStrValue(item["name"]));
						stCountry.ssGroup = GetStrValue(item["group"]);
						stCountry.ssIcoUrl = GetStrValue(item["ico"]);
						stCountry.ssIsoCode = GetStrValue(item["iso_code"]);
						rfData.vctTopCountry.push_back(stCountry);
					}
				}
			}

			Json::Value listCountry = data["list_country"];
			if (listCountry.isArray())
			{
				int nArrSize = listCountry.size();
				for (int i = 0; i < nArrSize; i++)
				{
					Json::Value item = listCountry[i];
					if (item.isObject())
					{
						STCountry stCountry;
						stCountry.nCode = GetIntValue(item["code"]);
						stCountry.wsName = U2W(GetStrValue(item["name"]));
						stCountry.ssGroup = GetStrValue(item["group"]);
						stCountry.ssIcoUrl = GetStrValue(item["ico"]);
						stCountry.ssIsoCode = GetStrValue(item["iso_code"]);
						rfData.vctListCountry.push_back(stCountry);
					}
				}
			}
		}

		return parsingSuccessful;
	}

	bool PraseBase(string & ssContent, STGeoIP & rfData)
	{
		if (ssContent.empty())
		{
			return false;
		}

		Json::Features features;
		Json::Reader reader(features);
		Json::Value root;

		bool bInnerPacket(false);
		bool parsingSuccessful = reader.parse(ssContent.c_str(), root);
		if (parsingSuccessful)
		{
			rfData.nCode = GetIntValue(root["code"]);
			rfData.wsMsg = U2W(GetStrValue(root["msg"]));

			Json::Value valData = root["data"];
			if (valData.isObject())
			{
				rfData.nRegionCode = GetIntValue(valData["region_code"]);
				rfData.nMobileCode = GetIntValue(valData["mobile_code"]);
				rfData.ssIP = GetStrValue(valData["ip_address"]);
				rfData.ssIsp = GetStrValue(valData["isp"]);
				rfData.nIspType = get_isp_type(rfData.ssIsp);
				rfData.ssProviceCode = stralgo::StringHelper::MakeLower(GetStrValue(valData["province_code"])) ;
				rfData.dbLatitude = GetDbValue(valData["latitude"]);
				rfData.dbLongitude = GetDbValue(valData["longitude"]);
			}
		}

		return parsingSuccessful;

	}


	string PacketParam(STParamLoginPin & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(LOGIN_PIN));
		ss_ret.append(PackLine(params.ssPin.c_str()));
		ss_ret.append(PackLine(params.ssOem.c_str()));
		ss_ret.append(PackLine(params.ssHardID.c_str()));
		ss_ret.append(PackLine(params.ssCoreVer.c_str()));
		ss_ret.append(PackLine(params.ssFileVer.c_str()));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamLoginKey & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(LOGIN_KEY));
		ss_ret.append(PackLine(params.ssKey.c_str()));
		ss_ret.append(PackLine(params.ssOem.c_str()));
		ss_ret.append(PackLine(params.ssHardID.c_str()));
		ss_ret.append(PackLine(params.ssCoreVer.c_str()));
		ss_ret.append(PackLine(params.ssFileVer.c_str()));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamGetSvrSetting & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H); 
		ss_ret.append(PackLine(SVR_SET));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamLineGroup & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(GROUP_LINE));
		ss_ret.append(PackLine(params.nUserID));
		ss_ret.append(PackLine(params.nGroupID));
		ss_ret.append(PackLine(params.nGameID));
		ss_ret.append(PackLine(params.nAreaID));

		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamLineList & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(LIST_LINE));
		ss_ret.append(PackLine(params.nUserID));
		ss_ret.append(PackLine(params.nGroupID));
		ss_ret.append(PackLine(params.nGameID));
		ss_ret.append(PackLine(params.nAreaID));

		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamStartAccInfo & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(ACC_INFO));
		ss_ret.append(PackLine(params.ssUser.c_str()));
		ss_ret.append(PackLine(params.ssPassword.c_str()));
		ss_ret.append(PackLine(params.nSvrID));
		ss_ret.append(PackLine(params.ssHardSeries.c_str()));
		ss_ret.append(PackLine(params.ssCoreVersion.c_str()));
		ss_ret.append(PackLine(params.ssFileVersion.c_str()));
		ss_ret.append(PackLine(params.nGameID));
		ss_ret.append(PackLine(params.ssWallPaper.c_str()));
		ss_ret.append(PackLine(params.nSubUserID));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STUpdateToken & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(UPDATE_TKN));
		ss_ret.append(PackLine(params.nUserID));
		ss_ret.append(PackLine(params.nSvrID));
		ss_ret.append(PackLine(params.nConnType));
		ss_ret.append(PackLine(params.ssHardSeries.c_str()));
		ss_ret.append(PackLine(params.nSendBY));
		ss_ret.append(PackLine(params.nRecvBY));
		ss_ret.append(PackLine(params.ssLocalIP.c_str()));
		ss_ret.append(PackLine(params.ssSvrName.c_str()));
		ss_ret.append(PackLine(params.nGameID));
		ss_ret.append(PackLine(params.nAreaID));
		ss_ret.append(PackLine(params.nSubUserID));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamLogStart & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(LOG_START));
		ss_ret.append(PackLine(params.nUserID));
		ss_ret.append(PackLine(params.nSvrID));
		ss_ret.append(PackLine(params.nConnType));
		ss_ret.append(PackLine(params.ssHardSeries.c_str()));
		ss_ret.append(PackLine(params.ssLocalIP.c_str()));
		ss_ret.append(PackLine(params.ssSvrName.c_str()));
		ss_ret.append(PackLine(params.nGameID));
		ss_ret.append(PackLine(params.nAreaID));
		ss_ret.append(PackLine(params.nSubUserID));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

	string PacketParam(STParamLogEnd & params)
	{
		string ss_ret;
		ss_ret.append(PARAM_H);
		ss_ret.append(PackLine(LOG_END));
		ss_ret.append(PackLine(params.nUserID));
		ss_ret.append(PackLine(params.nSvrID));
		ss_ret.append(PackLine(params.nConnType));
		ss_ret.append(PackLine(params.ssHardSeries.c_str()));
		ss_ret.append(PackLine(params.ssLocalIP.c_str()));
		ss_ret.append(PackLine(params.nSessionID));
		ss_ret.append(PackLine(params.ssNodeIP.c_str()));
		ss_ret.append(PackLine(params.nSubUserID));
		ss_ret.append(PARAM_T);

		return ss_ret;
	}

#define GET_XML_STR(st, key) \
else if (wsName == key)\
{st = W2U(wsValue);}
	
#define GET_XML_WCS(st, key) \
else if (wsName == key)\
{st = (wsValue);}

#define GET_XML_INT(st, key) \
else if (wsName == key)\
{\
		CMarkupNode nodeI4 = value_.GetChild();\
		if (nodeI4.IsValid())\
		{\
				st = _wtoi(nodeI4.GetValue());\
		}\
}

#define GET_XML_INT2(st, key) \
else if (wsName == key)\
{\
		st = _wtoi(wsValue.c_str());\
}

	bool UnPraseBase(string & sContent, STParamLoginPinResponse & rfData)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			wstring wsRootName = root.GetName();
			if (root.HasChildren())
			{
				CMarkupNode params = root.GetChild();
				wstring wsParams = params.GetName();
				if (params.HasChildren())
				{
					CMarkupNode param = params.GetChild();
					if (param.HasChildren())
					{
						bRet = true;
						CMarkupNode value = param.GetChild();
						CMarkupNode nodeStruct = value.GetChild(L"struct");
						CMarkupNode nodeMember = nodeStruct.GetChild();
						while (nodeMember.IsValid())
						{
							CMarkupNode name_ = nodeMember.GetChild(L"name");
							CMarkupNode value_ = nodeMember.GetChild(L"value");
							wstring wsName = name_.GetValue();
							wstring wsValue = value_.GetValue();
							if (wsName == L""){}
							GET_XML_STR(rfData.ssAuthKey, L"authkey")
							GET_XML_INT(rfData.nAuthType, L"authtype")
							GET_XML_STR(rfData.ssUserName, L"username")
							GET_XML_INT2(rfData.nUserID, L"id")
							GET_XML_WCS(rfData.wsGroup, L"group")
							GET_XML_WCS(rfData.wsNetPubTitle, L"usertitle")
							GET_XML_INT2(rfData.nGroupID, L"groupid")
							GET_XML_INT2(rfData.nMinuteLeft, L"expireminutes")
							GET_XML_WCS(rfData.wsExpireData, L"expiredate")
							GET_XML_WCS(rfData.wsExpireTime, L"expiretime")

							else if (wsName == L"error")
							{
								rfData.wsErrMsg = wsValue;
								bRet = false;
							}
							nodeMember = nodeMember.GetSibling();

						}


					}
				}
			}

		}
		
		return bRet;
	}

	bool UnPraseBase(string & sContent, STLineGroup & rfData, wstring & wsProv)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			wstring wsRootName = root.GetName();
			if (root.HasChildren())
			{
				CMarkupNode params = root.GetChild();
				wstring wsParams = params.GetName();
				if (params.HasChildren())
				{
					CMarkupNode param = params.GetChild();
					if (param.HasChildren())
					{
						bRet = true;
						bool bFirstChild = false;
						CMarkupNode nodeMember = param.GetChild();
						while (nodeMember.IsValid())
						{
							CMarkupNode name_ = nodeMember.GetChild(L"name");
							CMarkupNode value_ = nodeMember.GetChild(L"value");\
							if (name_.IsValid() && value_.IsValid())
							{
								wstring wsName = name_.GetValue();
								wstring wsValue = value_.GetValue();
								if (wsName == L"") {}
								GET_XML_WCS(rfData.wsResult, L"result")
								GET_XML_INT2(rfData.nGroupCount, L"gpcount")
								else
								{
									if (stralgo::StringHelper::BeginWith(wsName.c_str(), G_H_TAG))
									{
										STGroupItem stGItem;
										stGItem.wsGroupName = wsName;
										stGItem.wsGroupValue = wsValue ;
										wstring wsProvince;
										vector<wstring> vctSplit;
										stralgo::StringHelper::SplitString(wsValue, L'|', vctSplit);
										if (vctSplit.size() > 3)
										{
											stGItem.nGroupID = _wtoi(vctSplit[0].c_str());
											stGItem.wsProvName = wsProvince = vctSplit[2];
										}
										//if (wsProvince.size() && wsProvince == wsProv)
										{
											rfData.vctGroups.push_back(stGItem);
										}
										
									}
									else if (stralgo::StringHelper::BeginWith(wsName.c_str(), L_H_TAG))
									{
										if (!bFirstChild)
										{
											std::sort(rfData.vctGroups.begin(), rfData.vctGroups.end(), [&](STGroupItem & left, STGroupItem & right) {
												double db_x = LOGIC_CENTER()->GetGeoIP().dbLongitude;
												double db_y = LOGIC_CENTER()->GetGeoIP().dbLatitude;
												vector<wstring> vctLeft;
												stralgo::StringHelper::SplitString(left.wsGroupValue, L'|', vctLeft);
												vector<wstring> vctRight;
												stralgo::StringHelper::SplitString(right.wsGroupValue, L'|', vctRight);
												if (vctLeft.size() > 5 && vctRight.size() > 5)
												{
													double db_x_l = _wtof(vctLeft[3].c_str());
													double db_y_l = _wtof(vctLeft[4].c_str());
													double db_x_r = _wtof(vctRight[3].c_str());
													double db_y_r = _wtof(vctRight[4].c_str());

													return (pow(db_x_l - db_x, 2) + pow(db_y_l - db_y, 2)) < (pow(db_x_r - db_x, 2) + pow(db_y_r - db_y, 2));
												}

												return true;
											});
											bFirstChild = true;
										}

										
										//=================== sep =====================
										STLineItem stLItem(wsName, wsValue);
										vector<wstring> vctSplit;
										stralgo::StringHelper::SplitString(stLItem.wsValue, L'|', vctSplit);
										if (vctSplit.size() > 15)
										{
											stLItem.nLineID = _wtoi(vctSplit[1].c_str());
											stLItem.nProtoType = _wtoi(vctSplit[6].c_str());
											stLItem.ssSvr = W2U(vctSplit[3]);
											int nIspCode = _wtoi(vctSplit[vctSplit.size() - 2].c_str());
											stLItem.nIspType = nIspCode == 0 ?  rand() % 4 : nIspCode;
											stLItem.wsLineName = vctSplit[2];
											int nGID = _wtoi(vctSplit[5].c_str());
											for (int i = 0; i < rfData.vctGroups.size(); i++)
											{
												if (nGID == rfData.vctGroups[i].nGroupID)
												{
													if (rfData.vctGroups[i].wsProvName == wsProv)
													{
														stLItem.nDelay = GET_AVG_DELAY(stLItem.ssSvr);//
													}
													else
													{
														stLItem.nDelay = 16 + (rand() % 30);//GET_AVG_DELAY(stLItem.ssSvr);//
													}
													
													bool bInsert(false);
													//for (int j = 0; j < rfData.vctGroups[i].vctGroupLines.size(); j++)
													//{
													//	if (wcscmp(stLItem.wsLineName.c_str(), rfData.vctGroups[i].vctGroupLines[j].wsLineName.c_str()) < 0)
													//	{
													//		rfData.vctGroups[i].vctGroupLines.insert(rfData.vctGroups[i].vctGroupLines.begin() + j, stLItem);
													//		bInsert = true;
													//		break;
													//	}
													//}
													if (!bInsert)
													{
														rfData.vctGroups[i].vctGroupLines.push_back(stLItem);
													}
													
													break;
												}
											}
										}
									}

								}
							}


							nodeMember = nodeMember.GetSibling();

						}


					}
				}
			}

		}



		return bRet;
	}

	bool UnPraseBase(string & sContent, STLineList & rfData)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			wstring wsRootName = root.GetName();
			if (root.HasChildren())
			{
				CMarkupNode params = root.GetChild();
				wstring wsParams = params.GetName();
				if (params.HasChildren())
				{
					CMarkupNode param = params.GetChild();
					if (param.HasChildren())
					{
						bRet = true;
						CMarkupNode nodeMember = param.GetChild();
						while (nodeMember.IsValid())
						{
							CMarkupNode name_ = nodeMember.GetChild(L"name");
							CMarkupNode value_ = nodeMember.GetChild(L"value");
							wstring wsName = name_.GetValue();
							wstring wsValue = value_.GetValue();
							if (wsName == L"") {}
							GET_XML_WCS(rfData.wsResult, L"result")
							GET_XML_INT2(rfData.nGroupCount, L"gpcount")
							else
							{
								rfData.vctMember.push_back(STLineItem(wsName , wsValue));
							}
							nodeMember = nodeMember.GetSibling();

						}


					}
				}
			}

		}

		return bRet;
	}

	bool UnPraseBase(string & sContent, STACCInfo & rfData)
	{
		//sContent = "<methodResponse><params><param><value><struct><member><name>error</name><value>您的账号已经过期，请续费.</value></member><member><name>errorcode</name><value><i4>20001</i4></value></member><member><name>message</name><value>您的账号已经过期，请续费.</value></member><member><name>result</name><value>Error</value></member></struct></value></param></params>";

		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			wstring wsRootName = root.GetName();
			if (root.HasChildren())
			{
				CMarkupNode params = root.GetChild();
				wstring wsParams = params.GetName();
				if (params.HasChildren())
				{
					CMarkupNode param = params.GetChild();
					if (param.HasChildren())
					{
						bRet = true;
						CMarkupNode value = param.GetChild();
						CMarkupNode nodeStruct = value.GetChild(L"struct");
						CMarkupNode nodeMember = nodeStruct.GetChild();
						while (nodeMember.IsValid())
						{
							CMarkupNode name_ = nodeMember.GetChild(L"name");
							CMarkupNode value_ = nodeMember.GetChild(L"value");
							wstring wsName = name_.GetValue();
							wstring wsValue = value_.GetValue();
							if (wsName == L"") {}
								GET_XML_WCS(rfData.wsResult, L"result")
								GET_XML_WCS(rfData.wsUserName, L"username")
								GET_XML_INT(rfData.nID, L"id")
								GET_XML_INT(rfData.nUserID, L"iuidd")
								GET_XML_INT(rfData.nFeePerTime, L"fee_per_times")
								GET_XML_WCS(rfData.wsPassWord, L"password")
								GET_XML_STR(rfData.ssRules, L"rules")
								GET_XML_STR(rfData.ssSvr, L"server")
								GET_XML_INT(rfData.nPort, L"port")
								GET_XML_STR(rfData.ssVpnSvr, L"vpnserver")
								GET_XML_STR(rfData.ssTokenUser, L"tokenuser")
								GET_XML_STR(rfData.ssTokenPwd, L"tokenpass")
								GET_XML_STR(rfData.ssAddSvr, L"add_server")
								GET_XML_STR(rfData.ssAddSvrArea, L"add_server_area")
								GET_XML_INT2(rfData.nAddSvrPort, L"add_server_port")
								GET_XML_STR(rfData.ssAddSvrEx, L"add_serverex")
								GET_XML_STR(rfData.ssAddSvrAreaEx, L"add_server_areaex")
								GET_XML_STR(rfData.ssBandWidth, L"ROSRateLimit")
								GET_XML_STR(rfData.ssAddSvrPortEx, L"add_server_portex")
								GET_XML_STR(rfData.ssProcessRule, L"process_rules")
								GET_XML_INT(rfData.nErrCode, L"errorcode")
								{
									vector<string> vctSplit;
									stralgo::StringHelper::SplitString(rfData.ssBandWidth, ' ', vctSplit);
									if (vctSplit.size())
									{
										rfData.ssBandWidth = vctSplit[0];
									}
								}
								rfData.ssRules = stralgo::StringHelper::TrimAll(rfData.ssRules);
							nodeMember = nodeMember.GetSibling();

						}


					}
				}
			}

		}

		return bRet;
	}


	bool UnPraseBase(string & sContent, STSvrSet & rfData)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			wstring wsRootName = root.GetName();
			if (root.HasChildren())
			{
				CMarkupNode params = root.GetChild();
				wstring wsParams = params.GetName();
				if (params.HasChildren())
				{
					CMarkupNode param = params.GetChild();
					if (param.HasChildren())
					{
						bRet = true;
						CMarkupNode nodeMember = param.GetChild();
						while (nodeMember.IsValid())
						{
							CMarkupNode name_ = nodeMember.GetChild(L"name");
							CMarkupNode value_ = nodeMember.GetChild(L"value");
							wstring wsName = name_.GetValue();
							wstring wsValue = value_.GetValue();
							if (wsName == L"") {}
							GET_XML_WCS(rfData.wsResult, L"result")
							GET_XML_STR(rfData.ssZantingUrl, L"member_zanting_url")
							GET_XML_STR(rfData.ssZantingUrl2, L"member_zanting_url2")
							GET_XML_STR(rfData.ssBaseUrl, L"baseurl")
							GET_XML_STR(rfData.ssUrl, L"member_url")
							GET_XML_STR(rfData.ssUrl2, L"member_url2")
							GET_XML_STR(rfData.ssKeFuUrl, L"member_kefu_url")
							GET_XML_STR(rfData.ssHelpUrl, L"member_help_url")
							GET_XML_STR(rfData.ssNewUrl, L"news_url")
							GET_XML_STR(rfData.ssXufeiUrl, L"member_xufei_url")
							GET_XML_STR(rfData.ssADUrl, L"client_ad_url")
							GET_XML_WCS(rfData.wsADTitle, L"client_ad_title")
							GET_XML_STR(rfData.ssClientProto, L"client_protocol")
							GET_XML_STR(rfData.ssPngUrlSM[0], L"ad_qr_tailbar_1_png")
							GET_XML_STR(rfData.ssPngUrlSM[1], L"ad_qr_tailbar_2_png")
							GET_XML_STR(rfData.ssTargetUrlSM[0], L"ad_qr_tailbar_1_url")
							GET_XML_STR(rfData.ssTargetUrlSM[1], L"ad_qr_tailbar_2_url")

							GET_XML_STR(rfData.ssPngUrlBY[0], L"ad_m_tailbar_1_png")
							GET_XML_STR(rfData.ssPngUrlBY[1], L"ad_m_tailbar_2_png")
							GET_XML_STR(rfData.ssTargetUrlBY[0], L"ad_m_tailbar_1_url")
							GET_XML_STR(rfData.ssTargetUrlBY[1], L"ad_m_tailbar_2_url")

							nodeMember = nodeMember.GetSibling();

						}


					}
				}
			}

		}

		return bRet;
	}

	bool UnPraseBase(string & sContent, STGamePlat & rfData)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			if (!root.IsValid() || !root.HasChildren()) return false;
			CMarkupNode prarms = root.GetChild();
			if (!prarms.IsValid() || !prarms.HasChildren()) return false;
			CMarkupNode nodeMember = prarms.GetChild();
			if (!nodeMember.IsValid()) return false;
			while (nodeMember.IsValid())
			{
				if (nodeMember.HasChildren())
				{
					STGamePlatItem item_;
					CMarkupNode name_ = nodeMember.GetChild(L"szname");
					CMarkupNode id_ = nodeMember.GetChild(L"id");
					CMarkupNode tip_ = nodeMember.GetChild(L"tipmessage");
					CMarkupNode rules_ = nodeMember.GetChild(L"process_rules");

					item_.wsPlatName = name_.GetValue();
					item_.nPlatID = _wtoi(id_.GetValue());
					item_.wsTipMsg = tip_.GetValue();
					item_.ssRule = W2U(rules_.GetValue());
					rfData.vctPlats.push_back(item_);
					bRet = true;
				}
				nodeMember = nodeMember.GetSibling();
			}
		}

		return bRet;
	}

	bool UnPraseBase(string & sContent, STUpdateTokenResponse & rfData)
	{
		bool bRet(false);
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)sContent.data(), sContent.length()))
		{
			CMarkupNode root = xml.GetRoot();
			CMarkupNode params = root.GetChild();
			CMarkupNode param = params.GetChild();
			CMarkupNode val = param.GetChild();
			CMarkupNode struc = val.GetChild();
			CMarkupNode nodeMember = struc.GetChild();
			while (nodeMember.IsValid())
			{
				CMarkupNode name_ = nodeMember.GetChild(L"name");
				CMarkupNode value_ = nodeMember.GetChild(L"value");
				wstring wsName = name_.GetValue();
				wstring wsValue = value_.GetValue();
				if (wsName == L"") {}
				GET_XML_STR(rfData.ssResult, L"result")
				GET_XML_INT(rfData.nExpireed, L"isexpired")
				GET_XML_STR(rfData.ssTokenUser, L"tokenuser")
				GET_XML_STR(rfData.ssTokenPass, L"tokenpass")
				nodeMember = nodeMember.GetSibling();
				bRet = true;
			}
		}

		return bRet;
	}

	bool UnPraseBase(string &, STAreaList &)
	{
		int i;
		int pos = 0;
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsDir = wsExepath + L"gamelib\\gamelib.ini";

		wchar_t chSectionNames[MAX_ALLSECTIONS] = { 0 };//总的提出来的字符串
		wchar_t chSection[MAX_SECTION] = { 0 };//存放一个小节名
		wchar_t chKey[MAX_SECTION] = { 0 };


		GetPrivateProfileSectionNames(chSectionNames, MAX_ALLSECTIONS, wsDir.c_str());
		for (i = 0; i < MAX_ALLSECTIONS - 1; i++)
		{
			if (chSectionNames[i] == 0)//得到前面段明
			{
				for (int m = pos; m <= i; m++)
				{
					chSection[m - pos] = chSectionNames[m];//获取小节名
				}
				GetPrivateProfileSectionW(chSection, chKey, MAX_SECTION, wsDir.c_str());
				if (chSectionNames[i + 1] != 0)
				{
					pos = i + 1;
				}
				else
				{
					break;
				}



			}
		}

		return false;
	}

	bool UnPraseUrl(wstring & sContent, int & nSubID, int & nHour, int & nMinute, int & nTotalMins, int & nWXID)
	{
		bool bRet(false);
		vector<wstring> vctSplit;
		stralgo::StringHelper::SplitString(sContent, L':', vctSplit);
		if (vctSplit.size() > 3)
		{
			nSubID = _wtoi(vctSplit[2].c_str());
			string ssTime = CURLEncode::URLDecode(W2U(vctSplit[3]) );
			wstring wsTime = U2W(ssTime);
			if (auto pMatch = StrStrI(wsTime.c_str(), L"时"))
			{
				nHour = _wtoi(vctSplit[3].c_str());
				nMinute = _wtoi(pMatch + 1);
			}
			else
			{
				nMinute = _wtoi(vctSplit[3].c_str());
			}

			if (vctSplit.size() > 4)
			{
				nTotalMins = _wtoi(vctSplit[4].c_str());
			}
			if (vctSplit.size() > 5)
			{
				nWXID = _wtoi(vctSplit[5].c_str());
			}

			bRet = true;
		}

		return bRet;
	}

	bool UnPraseCDK(wstring & sContent, wstring & wsCDK)
	{
		bool bRet(false);
		vector<wstring> vctSplit;
		stralgo::StringHelper::SplitString(sContent, L':', vctSplit);
		for (int i = 0; i < vctSplit.size(); i++)
		{
			if (StrStrIW(vctSplit[i].c_str(), REWARD_CDK))
			{
				if (i + 1 < vctSplit.size())
				{
					wsCDK = vctSplit[i + 1];
					bRet = true;
				}
				break;
			}
		}

		return bRet;
	}

	void GET_H_M(wstring & wsContent, int & nHour, int & nMin, int & nTotalMins, wstring & nWXID)
	{
		bool bRet(false);
		vector<wstring> vctSplit;
		stralgo::StringHelper::SplitString(wsContent, L':', vctSplit);
		if (vctSplit.size() >= 4)
		{
			for (int i = 2; i < vctSplit.size(); i++)
			{
				string ssTime = CURLEncode::URLDecode(W2U(vctSplit[i]));
				wstring wsTime = U2W(ssTime);
				if (StrStrIW(wsTime.c_str(), L"时") || StrStrIW(wsTime.c_str(), L"分"))
				{
					if (auto pMatch = StrStrI(wsTime.c_str(), L"时"))
					{
						nHour = _wtoi(wsTime.c_str());
						nMin = _wtoi(pMatch + 1);
					}
					else
					{
						nMin = _wtoi(wsTime.c_str());
					}
					break;
				}

			}

			if (vctSplit.size() > 4)
			{
				nTotalMins = _wtoi(vctSplit[4].c_str());	
			}
			if (vctSplit.size() > 5)
			{
				nWXID = (vctSplit[5]);
			}

		}
	}

	void SAVE_INI(string folder, string key, string value)
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsDir = wsExepath + L"gamelib\\gamelib.ini";
		string ssDir = W2AS(wsDir);
		WritePrivateProfileStringA(folder.c_str(), key.c_str(), value.c_str(), ssDir.c_str());
	}

	void SAVE_INI(string folder, string key, int value)
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		wstring wsDir = wsExepath + L"gamelib\\gamelib.ini";
		string ssDir = W2AS(wsDir);
		WritePrivateProfileStringA(folder.c_str(), key.c_str(), std::to_string(value).c_str(), ssDir.c_str());
	}

	wstring GetNodeStringW(ui::CMarkupNode * node)
	{
		if (node->IsValid())
		{
			return node->GetValue();
		}

		return L"";
	}

	string GetNodeStringA(ui::CMarkupNode *node)
	{
		if (node->IsValid())
		{
			return W2U(node->GetValue());
		}

		return "";
	}

	int GetNodeInt(ui::CMarkupNode * node)
	{
		if (node->IsValid())
		{
			return _wtoi(node->GetValue());
		}

		return 0;
	}

	bool UnPraseBase(string & content, STGameList & rfData)
	{
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)content.data(), content.size()))
		{
			CMarkupNode root = xml.GetRoot();
			rfData.ssLastEdit = GetNodeStringA(&root.GetChild(L"lastedit"));
			rfData.nCount = GetNodeInt(&root.GetChild(L"itemcount"));

			CMarkupNode nodeGames = root.GetChild(L"games");
			if (nodeGames.IsValid())
			{
				CMarkupNode nodeGame = nodeGames.GetChild();
				while (nodeGame.IsValid())
				{
					STGame gameItem;
					gameItem.wsGameTitle = GetNodeStringW(&nodeGame.GetChild(L"title"));
					gameItem.wsSubTitle = GetNodeStringW(&nodeGame.GetChild(L"subtitle"));
					gameItem.nGameID = GetNodeInt(&nodeGame.GetChild(L"id"));
					gameItem.nIsFree = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.nHotGame = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.ssRules = GetNodeStringA(&nodeGame.GetChild(L"rules"));
					gameItem.wsAlias = GetNodeStringW(&nodeGame.GetChild(L"alias"));
					//areas
					{
						
						CMarkupNode nodeAreas = nodeGame.GetChild(L"areas");
						if (nodeAreas.IsValid())
						{
							CMarkupNode nodeArea = nodeAreas.GetChild();
							while (nodeArea.IsValid())
							{
								STGameArea gameArea;
								gameArea.nID = GetNodeInt(&nodeArea.GetChild(L"id"));
								gameArea.wsTitle = GetNodeStringW(&nodeArea.GetChild(L"title"));
								gameArea.ssRules = GetNodeStringA(&nodeArea.GetChild(L"rules"));
								string ssArea = GetNodeStringA(&nodeArea.GetChild(L"servers"));
								stralgo::StringHelper::SplitString(ssArea, ',', gameArea.vctSvr);
								gameItem.vctAreas.push_back(gameArea);
								nodeArea = nodeArea.GetSibling();

							}
						}
					}

					rfData.vctGames.push_back(gameItem);
					nodeGame = nodeGame.GetSibling();
				}
			}
		}

		return false;
	}

	bool LoadGameListLocal(wstring & path, STGameList & rfData)
	{
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromFile(path.c_str()))
		{
			CMarkupNode root = xml.GetRoot();
			rfData.ssLastEdit = GetNodeStringA(&root.GetChild(L"lastedit"));
			rfData.nCount = GetNodeInt(&root.GetChild(L"itemcount"));

			CMarkupNode nodeGames = root.GetChild(L"games");
			if (nodeGames.IsValid())
			{
				CMarkupNode nodeGame = nodeGames.GetChild();
				while (nodeGame.IsValid())
				{
					STGame gameItem;
					gameItem.wsGameTitle = GetNodeStringW(&nodeGame.GetChild(L"title"));
					gameItem.wsSubTitle = GetNodeStringW(&nodeGame.GetChild(L"subtitle"));
					gameItem.nGameID = GetNodeInt(&nodeGame.GetChild(L"id"));
					gameItem.nIsFree = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.nHotGame = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.ssRules = GetNodeStringA(&nodeGame.GetChild(L"rules"));
					gameItem.wsAlias = GetNodeStringW(&nodeGame.GetChild(L"alias"));
					gameItem.ssLogoUrl = GetNodeStringA(&nodeGame.GetChild(L"logo"));
					gameItem.ssBgUrl = GetNodeStringA(&nodeGame.GetChild(L"bgimage"));
					int nTestMode = LOGIC_CENTER()->GetCinfigIni().nTestData;
					if (0 == nTestMode)
					{
						gameItem.nPlatID = GetNodeInt(&nodeGame.GetChild(L"platformid"));
					}
					else if (1 == nTestMode)
					{
						gameItem.nPlatID = get_rand_plat();
					}
					
					
					
					//areas
					{

						CMarkupNode nodeAreas = nodeGame.GetChild(L"areas");
						if (nodeAreas.IsValid())
						{
							CMarkupNode nodeArea = nodeAreas.GetChild();
							while (nodeArea.IsValid())
							{
								STGameArea gameArea;
								gameArea.nID = GetNodeInt(&nodeArea.GetChild(L"id"));
								gameArea.wsTitle = GetNodeStringW(&nodeArea.GetChild(L"title"));
								gameArea.ssRules = GetNodeStringA(&nodeArea.GetChild(L"rules"));
								string ssArea = GetNodeStringA(&nodeArea.GetChild(L"servers"));
								gameArea.ssLogoUrl = GetNodeStringA(&nodeArea.GetChild(L"logo"));
								gameArea.wsSubTitle = GetNodeStringW(&nodeArea.GetChild(L"subtitle"));
								stralgo::StringHelper::SplitString(ssArea, ',', gameArea.vctSvr);
								gameItem.vctAreas.push_back(gameArea);
								nodeArea = nodeArea.GetSibling();

							}
						}
					}

					rfData.vctGames.push_back(gameItem);
					nodeGame = nodeGame.GetSibling();
				}
			}
		}
		auto sz = rfData.vctGames.size();
		return false;
	}

	bool LoadGameListLocalPool(wstring & path, STGameList & rfData, std::set<int> & setPool)
	{
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromFile(path.c_str()))
		{
			CMarkupNode root = xml.GetRoot();
			rfData.ssLastEdit = GetNodeStringA(&root.GetChild(L"lastedit"));
			rfData.nCount = GetNodeInt(&root.GetChild(L"itemcount"));

			CMarkupNode nodeGames = root.GetChild(L"games");
			if (nodeGames.IsValid())
			{
				CMarkupNode nodeGame = nodeGames.GetChild();
				while (nodeGame.IsValid())
				{
					STGame gameItem;
					gameItem.wsGameTitle = GetNodeStringW(&nodeGame.GetChild(L"title"));
					gameItem.wsSubTitle = GetNodeStringW(&nodeGame.GetChild(L"subtitle"));
					gameItem.nGameID = GetNodeInt(&nodeGame.GetChild(L"id"));
					gameItem.nIsFree = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.nHotGame = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.ssRules = GetNodeStringA(&nodeGame.GetChild(L"rules"));
					gameItem.wsAlias = GetNodeStringW(&nodeGame.GetChild(L"alias"));
					gameItem.ssLogoUrl = GetNodeStringA(&nodeGame.GetChild(L"logo"));
					gameItem.ssBgUrl = GetNodeStringA(&nodeGame.GetChild(L"bgimage"));
					int nTestMode = LOGIC_CENTER()->GetCinfigIni().nTestData;
					if (0 == nTestMode)
					{
						gameItem.nPlatID = GetNodeInt(&nodeGame.GetChild(L"platformid"));
					}
					else if (1 == nTestMode)
					{
						gameItem.nPlatID = get_rand_plat();
					}



					//areas
					{

						CMarkupNode nodeAreas = nodeGame.GetChild(L"areas");
						if (nodeAreas.IsValid())
						{
							CMarkupNode nodeArea = nodeAreas.GetChild();
							while (nodeArea.IsValid())
							{
								STGameArea gameArea;
								gameArea.nID = GetNodeInt(&nodeArea.GetChild(L"id"));
								gameArea.wsTitle = GetNodeStringW(&nodeArea.GetChild(L"title"));
								gameArea.ssRules = GetNodeStringA(&nodeArea.GetChild(L"rules"));
								string ssArea = GetNodeStringA(&nodeArea.GetChild(L"servers"));
								gameArea.ssLogoUrl = GetNodeStringA(&nodeArea.GetChild(L"logo"));
								gameArea.wsSubTitle = GetNodeStringW(&nodeArea.GetChild(L"subtitle"));
								stralgo::StringHelper::SplitString(ssArea, ',', gameArea.vctSvr);
								gameItem.vctAreas.push_back(gameArea);
								nodeArea = nodeArea.GetSibling();

							}
						}
					}

					auto itrf = setPool.find(gameItem.nGameID);
					if (itrf != setPool.end())
					{
						rfData.vctGames.push_back(gameItem);
					}

					
					nodeGame = nodeGame.GetSibling();
				}
			}
		}
		auto sz = rfData.vctGames.size();
		return false;
	}

	bool UnPraseBaseEx(string & content, STGameList & rfData, string ssLastEdit)
	{
		using namespace ui;
		std::wstring wsRet;
		CMarkup xml;
		if (xml.LoadFromMem((BYTE*)content.data(), content.size()))
		{
			CMarkupNode root = xml.GetRoot();
			string ssLastEditOnline = GetNodeStringA(&root.GetChild(L"lastedit"));
			
			time_t t_online = StringToDatetime(ssLastEditOnline);
			time_t t_local = StringToDatetime(ssLastEdit);
			if (t_local >= t_online)
			{
				return false;
			}

			rfData.ssLastEdit = ssLastEditOnline;
			rfData.nCount = GetNodeInt(&root.GetChild(L"itemcount"));

			CMarkupNode nodeGames = root.GetChild(L"games");
			if (nodeGames.IsValid())
			{
				CMarkupNode nodeGame = nodeGames.GetChild();
				while (nodeGame.IsValid())
				{
					STGame gameItem;
					gameItem.wsGameTitle = GetNodeStringW(&nodeGame.GetChild(L"title"));
					gameItem.wsSubTitle = GetNodeStringW(&nodeGame.GetChild(L"subtitle"));
					gameItem.nGameID = GetNodeInt(&nodeGame.GetChild(L"id"));
					gameItem.nIsFree = GetNodeInt(&nodeGame.GetChild(L"isfree"));
					gameItem.nHotGame = GetNodeInt(&nodeGame.GetChild(L"hotgame"));
					gameItem.ssRules = GetNodeStringA(&nodeGame.GetChild(L"rules"));
					gameItem.wsAlias = GetNodeStringW(&nodeGame.GetChild(L"alias"));
					gameItem.ssLogoUrl = GetNodeStringA(&nodeGame.GetChild(L"logo"));
					gameItem.ssBgUrl = GetNodeStringA(&nodeGame.GetChild(L"bgimage"));
					int nTestMode = LOGIC_CENTER()->GetCinfigIni().nTestData;
					if (0 == nTestMode)
					{
						gameItem.nPlatID = GetNodeInt(&nodeGame.GetChild(L"platformid"));
					}
					else if (1 == nTestMode)
					{
						gameItem.nPlatID = get_rand_plat();
					}
					
					//areas
					{

						CMarkupNode nodeAreas = nodeGame.GetChild(L"areas");
						if (nodeAreas.IsValid())
						{
							CMarkupNode nodeArea = nodeAreas.GetChild();
							while (nodeArea.IsValid())
							{
								STGameArea gameArea;
								gameArea.nID = GetNodeInt(&nodeArea.GetChild(L"id"));
								gameArea.wsTitle = GetNodeStringW(&nodeArea.GetChild(L"title"));
								gameArea.ssRules = GetNodeStringA(&nodeArea.GetChild(L"rules"));
								gameArea.wsSubTitle = GetNodeStringW(&nodeArea.GetChild(L"subtitle"));
								gameArea.ssLogoUrl = GetNodeStringA(&nodeArea.GetChild(L"logo"));
								string ssArea = GetNodeStringA(&nodeArea.GetChild(L"servers"));
								stralgo::StringHelper::SplitString(ssArea, ',', gameArea.vctSvr);
								gameItem.vctAreas.push_back(gameArea);
								nodeArea = nodeArea.GetSibling();

							}
						}
					}

					rfData.vctGames.push_back(gameItem);
					nodeGame = nodeGame.GetSibling();
				}
			}
		}

		return true;
	}


	time_t StringToDatetime(string str)
	{
		char *cha = (char*)str.data();             // 将string转换成char*。
		tm tm_;                                    // 定义tm结构体。
		int year, month, day, hour(0), minute(0), second(0);// 定义时间的各个int临时变量。
		sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
		tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
		tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
		tm_.tm_mday = day;                         // 日。
		tm_.tm_hour = hour;                        // 时。
		tm_.tm_min = minute;                       // 分。
		tm_.tm_sec = second;                       // 秒。
		tm_.tm_isdst = 0;                          // 非夏令时。
		time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
		return t_;                                 // 返回值。 
	}


	void LOG_FILE(wstring & path, string & content)
	{
		FILE * pLogFile(NULL);
		pLogFile = _wfopen(path.c_str(), L"w");
		if (pLogFile)
		{
			fwrite(content.data(), content.size(), 1, pLogFile);
			fclose(pLogFile);
		}
	}

	string READ_FILE(wstring  path)
	{
		string ssRet;
		FILE * pLogFile(NULL);
		pLogFile = _wfopen(path.c_str(), L"r");
		if (pLogFile)
		{
			fseek(pLogFile, 0, SEEK_END); //定位到文件末 
			long nFileLen = ftell(pLogFile);
			fseek(pLogFile, 0, SEEK_SET);
			if (nFileLen)
			{
				ssRet.assign(nFileLen, 0);
				fread((void*)ssRet.data(), nFileLen, 1, pLogFile);
			}
			
			fclose(pLogFile);
		}

		return ssRet;
	}

	void * READ_FILE(wstring  path, int & len)
	{
		void * pRet = NULL;
		FILE * pLogFile(NULL);
		pLogFile = _wfopen(path.c_str(), L"rb");
		if (pLogFile)
		{
			fseek(pLogFile, 0, SEEK_END); //定位到文件末 
			len = ftell(pLogFile);
			fseek(pLogFile, 0, SEEK_SET);
			if (len)
			{
				pRet = new BYTE[len];
				fread((void*)pRet, len, 1, pLogFile);
			}

			fclose(pLogFile);
		}

		return pRet;
	}

	wstring GetDefaultLogoPath()
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		return wsExepath + L"gamelib\\game_pic\\logo.png";
	}

	wstring GetDefaultBGPath()
	{
		wstring wsExepath = ui::PathUtil::GetCurrentModuleDir();
		return wsExepath + L"gamelib\\game_pic\\bg.png";
	}

	int GET_SUBID(wstring & wsResponse, wstring * pErr)
	{
		int nRet(0);
		vector<wstring> vctSplit;
		int i = 0;
		stralgo::StringHelper::SplitString(wsResponse, L':', vctSplit);
		for(;i<vctSplit.size();i++)
		{
			if(vctSplit[i].find(L"subuser_success")!=wstring::npos)
				nRet =  _wtoi(vctSplit[i+1].c_str());
			if (vctSplit[i].find(L"subuser_fee") != wstring::npos)
				nRet =  _wtoi(vctSplit[i + 1].c_str()); 
			if (vctSplit[i].find(L"subuser_fee_fail") != wstring::npos)
					nRet = _wtoi(vctSplit[i + 1].c_str());
			/*	if (pErr && vctSplit.size() > 2)
				{
					*pErr = vctSplit[2];
				}*/
		}

		if (nRet == 0 && vctSplit.size() > 2)
		{
			nRet = _wtoi(vctSplit[2].c_str());
		}

		return nRet;
	}
	//--------------------------------------------- ICMP ----------------------------------------
	typedef struct
	{
		unsigned char hdr_len : 4;         //4位头部长度
		unsigned char version : 4;         //4位版本号
		unsigned char tos;               //8位服务类型
		unsigned short total_len;        //16位总长度
		unsigned short identifier;       //16位标识符
		unsigned short frag_and_flags;   //3位标志加13位片偏移
		unsigned char ttl;               //8位生存时间
		unsigned char protocol;          //8位上层协议号
		unsigned short checksum;         //16位效验和
		unsigned long sourceIP;          //32位源IP地址
		unsigned long destIP;            //32位目的IP地址
	}IP_HEADER;

	//ICMP报头
	typedef struct
	{
		BYTE type;     //8位类型字段
		BYTE code;     //8位代码字段
		USHORT cksum;  //16位效验和
		USHORT id;     //16位标识符
		USHORT seq;    //16位序列号
	}ICMP_HEADER;


	//报文解码结构
	typedef struct
	{
		USHORT usSeqNo;          //序列号
		DWORD dwRoundTripTime;   //返回时间
		in_addr dwIPaddr;        //返回报文的IP地址
	}DECODE_RESULT;

	//计算网际效验和函数
	USHORT checksum(USHORT *pBuf, int iSize)
	{
		unsigned long cksum = 0;
		while (iSize > 1)
		{
			cksum += *pBuf++;
			iSize -= sizeof(USHORT);
		}
		if (iSize)
		{
			cksum += *(USHORT*)pBuf;
		}
		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >> 16);
		return(USHORT)(~cksum);
	}

	//对数据包进行解码
	BOOL DecodeIcmpResponse(char *pBuf, int iPacketSize, DECODE_RESULT &DecodeResult, BYTE ICMP_ECHO_REPLY, BYTE ICMP_TIMEOUT)
	{
		//检查数据报大小的合法性
		IP_HEADER *pIpHdr = (IP_HEADER*)pBuf;
		int iIpHdrLen = pIpHdr->hdr_len * 4;
		if (iPacketSize < (int)(iIpHdrLen + sizeof(ICMP_HEADER)))
			return FALSE;
		//根据ICMP报文类型提取ID字段和序列号字段
		ICMP_HEADER *pIcmpHdr = (ICMP_HEADER*)(pBuf + iIpHdrLen);
		USHORT usID, usSquNo;
		if (pIcmpHdr->type == ICMP_ECHO_REPLY)    //ICMP回显应答报文
		{
			usID = pIcmpHdr->id;   //报文ID
			usSquNo = pIcmpHdr->seq;  //报文序列号
		}
		else if (pIcmpHdr->type == ICMP_TIMEOUT)   //ICMP超时差错报文
		{
			char *pInnerIpHdr = pBuf + iIpHdrLen + sizeof(ICMP_HEADER);  //载荷中的IP头
			int iInnerIPHdrLen = ((IP_HEADER*)pInnerIpHdr)->hdr_len * 4; //载荷中的IP头长
			ICMP_HEADER *pInnerIcmpHdr = (ICMP_HEADER*)(pInnerIpHdr + iInnerIPHdrLen);//载荷中的ICMP头
			usID = pInnerIcmpHdr->id;  //报文ID
			usSquNo = pInnerIcmpHdr->seq;  //序列号
		}
		else {
			return false;
		}
		//检查ID和序列号以确定收到期待数据报
		if (usID != (USHORT)GetCurrentProcessId() || usSquNo != DecodeResult.usSeqNo)
		{
			return false;
		}
		//记录IP地址并计算往返时间
		DecodeResult.dwIPaddr.s_addr = pIpHdr->sourceIP;
		DecodeResult.dwRoundTripTime = GetTickCount() - DecodeResult.dwRoundTripTime;

		//处理正确收到的ICMP数据报
		if (pIcmpHdr->type == ICMP_ECHO_REPLY || pIcmpHdr->type == ICMP_TIMEOUT)
		{
			return true;
		}
		else {
			return false;
		}
		return true;
	}

	uint32_t GET_AVG_DELAY(string url, int nTime, int * nLoss)
	{
		uint32_t uAvgDelay(1);
		int a = 1;
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
		u_long ulDestIP = inet_addr(url.c_str());
		//转换不成功时按域名解析
		if (ulDestIP == INADDR_NONE)
		{
			hostent *pHostent = gethostbyname(url.c_str());
			if (pHostent)
			{
				ulDestIP = (*(in_addr*)pHostent->h_addr).s_addr;
			}
			else
			{
				return 0;
			}
		}
	
		//填充目的端socket地址
		sockaddr_in destSockAddr;
		ZeroMemory(&destSockAddr, sizeof(sockaddr_in));
		destSockAddr.sin_family = AF_INET;
		destSockAddr.sin_addr.s_addr = ulDestIP;
		//创建原始套接字
		SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
		//超时时间
		int iTimeout = 3000;
		//接收超时
		setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout));
		//发送超时
		setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(iTimeout));


		//构造ICMP回显请求消息，并以TTL递增的顺序发送报文
		//ICMP类型字段
		const BYTE ICMP_ECHO_REQUEST = 8;   //请求回显
		const BYTE ICMP_ECHO_REPLY = 0;     //回显应答
		const BYTE ICMP_TIMEOUT = 11;       //传输超时

											//其他常量定义
		const int DEF_ICMP_DATA_SIZE = 32;    //ICMP报文默认数据字段长度
		const int MAX_ICMP_PACKET_SIZE = 1024; //ICMP报文最大长度（包括报头）
		const DWORD DEF_ICMP_TIMEOUT = 3000;   //回显应答超时时间
		const int DEF_MAX_HOP = 4;            //最大跳站数

											  //填充ICMP报文中每次发送时不变的字段
		char IcmpSendBuf[sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE]; //发送缓冲区
		memset(IcmpSendBuf, 0, sizeof(IcmpSendBuf));     //初始化发送缓冲区
		char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE];        //接收缓冲区
		memset(IcmpRecvBuf, 0, sizeof(IcmpRecvBuf));     //初始化接收缓冲区

		ICMP_HEADER *pIcmpHeader = (ICMP_HEADER*)IcmpSendBuf;
		pIcmpHeader->type = ICMP_ECHO_REQUEST;             //类型为请求回显
		pIcmpHeader->code = 0;                             //代码字段为0
		pIcmpHeader->id = (USHORT)GetCurrentProcessId();   //ID字段为当前进程号
		memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);   //数据字段
		USHORT usSeqNo = 0;                //ICMP报文序列号
		int iTTL = 64;                      //TTL初始值为1
		int r = 0, o = 0;
		BOOL bReachDestHost = FALSE;       //循环退出标志
		int iMaxHot = DEF_MAX_HOP;         //循环的最大次数
		DECODE_RESULT DecodeResult;      //传递给报文解码函数的结构化参数
		std::vector<DWORD> vctLayout;
		while (!bReachDestHost&&iMaxHot--)
		{
			//设置IP报头的TTL字段
			setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char*)&iTTL, sizeof(iTTL));
			//填充ICMP报文中每次发送变化的字段
			((ICMP_HEADER*)IcmpSendBuf)->cksum = 0;                 //效验和先置为0
			((ICMP_HEADER*)IcmpSendBuf)->seq = htons(usSeqNo++);    //填充序列号
			((ICMP_HEADER*)IcmpSendBuf)->cksum = checksum((USHORT*)IcmpSendBuf, sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE);  //计算效验和

																															//记录序列号和当前时间
			DecodeResult.usSeqNo = ((ICMP_HEADER*)IcmpSendBuf)->seq;    //当前序号
			DecodeResult.dwRoundTripTime = GetTickCount();              //当前时间
																		//发送TCP回显请求信息
			sendto(sockRaw, IcmpSendBuf, sizeof(IcmpSendBuf), 0, (sockaddr*)&destSockAddr, sizeof(destSockAddr));
			//接收ICMP差错报文并进行解析处理
			sockaddr_in from;              //对端socket地址
			int iFromLen = sizeof(from);     //地址结构大小
			int iReadDataLen;              //接收数据长度
			while (1)
			{
				//接收数据
				iReadDataLen = recvfrom(sockRaw, IcmpRecvBuf, MAX_ICMP_PACKET_SIZE, 0, (sockaddr*)&from, &iFromLen);
				if (iReadDataLen != SOCKET_ERROR)//有数据达到
				{
					//对数据包进行解码
					if (DecodeIcmpResponse(IcmpRecvBuf, iReadDataLen, DecodeResult, ICMP_ECHO_REPLY, ICMP_TIMEOUT))
					{
						//到达目的地，退出循环
						if (DecodeResult.dwIPaddr.s_addr == destSockAddr.sin_addr.s_addr) {
							//输出IP地址
							if (DecodeResult.dwRoundTripTime)
							{
								vctLayout.push_back(DecodeResult.dwRoundTripTime);
							}
							else
							{
								;
							}

							r++;
							break;
						}

					}
				}
				else if (WSAGetLastError() == WSAETIMEDOUT) //接收超时，输出星号
				{
					uAvgDelay = 200;
					
					o++;
					if (nTime == 1)
						return uAvgDelay;
					break;
				}
				else {
					break;
				}
			}
		}

		if (nLoss)
		{
			*nLoss = o;
		}
		if (vctLayout.size() > 3)
		{
			uint64_t uTotal(0);
			for (auto itr : vctLayout)
			{
				uTotal += itr;
			}

			uAvgDelay = (uint32_t)(uTotal / vctLayout.size());
		}


		return uAvgDelay;
	}
}


string get_pro_dyn()
{
#ifdef _DEBUG
	return PRO_BASE2;
#endif
	static string pro_tab[] = { PRO_BASE1 ,PRO_BASE2 ,PRO_BASE3 };
	srand(GetTickCount());
	return  pro_tab[rand() % 3];
}

string get_geo_dyn()
{
	static string pro_tab[] = { GEO_IP1 ,GEO_IP2 ,GEO_IP3 };
	srand(GetTickCount());
	return pro_tab[rand() % 3];
}

int get_rand_plat()
{
	srand(GetTickCount());
	return rand() % 4+1;
}
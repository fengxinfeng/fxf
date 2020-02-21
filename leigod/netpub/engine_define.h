#pragma once
#include <string>
#include <set>

using namespace std;
#define GEO_IP1 "https://geoip.leigod.com/geoip2/index.php"
#define GEO_IP2 "https://www.leigod.com/geoip2/index.php"
#define GEO_IP3 "https://www2.leigod.com/geoip2/index.php"
#define GEO_IP0	"http://ip-api.com/json"				//有经纬度

string get_geo_dyn();

#define GEO_TRY_TIMES  5
#define DES_KEY	"vpn@%091server"

#define PRO_BASE1	"http://api-netbar-1.leigod.com:8517/"
#define PRO_BASE2	"http://api-netbar-2.leigod.com:8517/"
#define PRO_BASE3	"http://api-netbar-3.leigod.com:8517/"   //5542

string get_pro_dyn();
//#ifdef _DEBUG
//#define PROTO_URL	"http://47.100.86.2:8443/"

//#else
#define PROTO_URL	get_pro_dyn()
//#endif


//#ifdef REAL_ENV
#define PROTO_URL	get_pro_dyn()
#define PAY_URL		L"http://netbar.leigod.com/pc_new/scanRecharge.html"//L"http://sandboxie-netbar.leigod.com/pc_new/scanRecharge.html" //
#define BIND_URL	L"http://netbar.leigod.com/pc_new/wechatLogin.html" //L"http://sandboxie-netbar.leigod.com/pc_new/wechatLogin.html" //
#define REWARD_URL	 L"http://netbar.leigod.com/pc_new/springActivities.html"// L"http://sandboxie-netbar.leigod.com/pc_new/springActivities.html"//
#define GAMELIST_CHN	"http://netbar-api.leigod.com/app/get_custom_games_list?area=china"//"http://sandboxie-user.leigod.com/app/get_custom_games_list?area=china"
#define GAMELIST_FOR	"http://netbar-api.leigod.com/app/get_custom_games_list?area=foreign"//"http://sandboxie-user.leigod.com/app/get_custom_games_list?area=foreign"
//#else
//#define PAY_URL		L"http://sandboxie-netbar.leigod.com/pc_new/scanRecharge.html" //
//#define BIND_URL	L"http://sandboxie-netbar.leigod.com/pc_new/wechatLogin.html" //
//#define REWARD_URL	L"http://sandboxie-netbar.leigod.com/pc_new/springActivities.html"//
//#define GAMELIST_CHN	"http://sandboxie-user.leigod.com/app/get_custom_games_list?area=china"
//#define GAMELIST_FOR	"http://sandboxie-user.leigod.com/app/get_custom_games_list?area=foreign"
//#define PAY_URL			L"http://sandboxie-netbar.leigod.com/pc/scanRecharge.html"
//#define BIND_URL		L"http://sandboxie-netbar.leigod.com/pc/wechatLogin.html"
//#endif

#define GET_PLAT_LIST	"http://netbar-api.leigod.com/app/get_game_platform_list"//"http://sandboxie-user.leigod.com/app/get_game_platform_list"
//#define GET_GAMELIST	"http://sandboxie-user.leigod.com/app/get_game_platform_list?area=foreign"
/*
http://netbar-api.leigod.com/app/get_custom_games_list?area=foreign
http://netbar-api.leigod.com/app/get_custom_games_list?area=china
二、平台列表接口 http://sandboxie-user.leigod.com/app/get_game_platform_list
三、游戏列表接口 http://sandboxie-user.leigod.com/app/get_custom_games_list
*/

#define TOOLBAR_KF_URL   "https://uchat.im-cc.com/webchat_new/static/html/index.html?ht=jHCMM6"//"http://uchat.im-cc.com/webchat_new/static/html/index.html?ht=jHCMM6"
#define SM_MODE_TIP		L"扫码登录"

#define LOGIN_PIN	"readpinuserinfo2"
#define LOGIN_KEY	"readipkeyuserinfo2"
#define SVR_SET		"get_server_setting"
#define GROUP_LINE	"get_line_group"
#define LIST_LINE	"get_line_list"
#define ACC_INFO	"get_startacc_info"
#define UPDATE_TKN	"reportstatus"
#define LOG_START	"connected"
#define LOG_END		"logout"

#define G_H_TAG		L"gpinfo"
#define L_H_TAG		L"s"

#define FILE_VER	"4.1.0.5"
#define CORE_VER	"1.0.0.5"

#define LOG_ENGIN	L"engin.log"

#define MAX_ALLSECTIONS 300  
#define MAX_SECTION		20 
int is_log_file();
int is_log_trace();
int is_dbg_engin();
wstring get_isp_name(int);
#define LOG_XML			is_log_file()//1
#define ENDBG_ON		is_dbg_engin()


#define ERRCODE_OK					0
#define ERRCODE_GETGLIST_ERR		1			//获取游戏信息错误
#define ERRCODE_GETGLINE_ERR		2			//获取游戏分组
#define ERRCODE_LOGIN_ERR			3			//登录失败

#define  ISP_ALL					0
#define  ISP_TELCOM					1
#define  ISP_UNICOM					2
#define  ISP_MOBILE					3

#define ENGIN_ERR					1
#define ENGIN_OK					2
#define ENGIN_SVR_KICK				3

#define EBGIN_TIMER_ELAPSE			4000

#define GA_CHN						51
#define GA_PLAT						10002

#define WEB_ENCHARGE_SUCC			L"subuser_fee_success"
#define WEB_ENCHARGE_FAIL			L"subuser_fee_fail"

#define WEB_BIND_SUCC				L"subuser_success"
#define WEB_BIND_ERR				L"subuser_error"
#define WEB_BIND_BINDTEL			L"subuser_bindtel"
#define WEB_BIND_FEE				L"subuser_fee"
#define WEB_JUMP_HOME				L"skip_leigod"

#define REWARD_BACK					L"back_home"
#define REWARD_KEYWORD				"lottery"
#define REWARD_CDK					L"copy_save"

#define UINQUEPRCESSSTR				L"leigod_netbar_20200110"

#define PRO_TRY_TIMES				3
#define ACC_USER_EXPIRED			20001

#define  SAFE_BOUND					200

struct STProtoBase
{
	virtual ~STProtoBase() {};
	int nCode;
	wstring wsMsg;
};

struct STGeoIP : public STProtoBase
{
	STGeoIP()
	{
		nRegionCode = 0;
		nMobileCode = 0;
		dbLatitude = 0.0;
		dbLongitude = 0.0;
		nIspType = 0;
	}
	int nRegionCode;
	int nMobileCode;
	string ssIP;
	string ssProviceCode;
	string ssIsp;
	wstring wsProvince;
	int nIspType;
	double dbLatitude;
	double dbLongitude;
};

struct STParamLoginPin
{
	string ssPin;
	string ssOem;
	string ssHardID;
	string ssCoreVer;
	string ssFileVer;
};

struct STParamLoginPinResponse
{
	STParamLoginPinResponse()
	{
		nAuthType = 0;
		nBussinFree = 0;
		nBussinNeedTel = 0;
		nBussinNeedWeixin = 0;
		nGroupID = 0;
		nUserID = 0;
		nSubUserID = 0;
		nMinuteLeft = 0;
	}
	bool IsValid()
	{
		return nGroupID && nUserID;
	}
	string ssAuthKey;
	int nAuthType;
	int nBussinFree;
	int nBussinNeedTel;
	int nBussinNeedWeixin;
	wstring wsGroup;
	int nGroupID;
	string ssUserName;
	int nUserID;
	int nSubUserID;
	wstring nWxID;
	string wsUserTitle;
	wstring wsNetPubTitle;
	wstring wsErrMsg;
	int nMinuteLeft;
	wstring wsExpireData;
	wstring wsExpireTime;
};

struct STParamLoginKey
{
	string ssKey;
	string ssOem;
	string ssHardID;
	string ssCoreVer;
	string ssFileVer;
};

struct STParamGetSvrSetting
{

};


struct STParamLineGroup
{
	STParamLineGroup()
	{
		nUserID = 0;
		nGroupID = 0;
		nGameID = 0;
		nAreaID = 0;
	}
	int nUserID;
	int nGroupID;
	int nGameID;
	int nAreaID;
	wstring wsProvName;
};

struct STParamLineList : public STParamLineGroup
{

};

struct STParamStartAccInfo
{
	STParamStartAccInfo()
	{
		nSvrID = 0;
		nGameID = 0;
		nSubUserID = 0;
	}
	string ssUser;
	string ssPassword;
	int nSvrID;
	string ssHardSeries;
	string ssCoreVersion;
	string ssFileVersion;
	int nGameID;
	string ssWallPaper;
	int nSubUserID;
};

//获取token 更新
struct STUpdateToken
{
	STUpdateToken()
	{
		nUserID = 0;
		nSvrID = 0;
		nConnType = 0;
		nGameID = 0;
		nAreaID = 0;
		nSubUserID = 0;
		nSendBY = 0;
		nRecvBY = 0;
	}
	int nUserID;
	int nSvrID;
	int nConnType;
	string ssHardSeries;
	int nSendBY;
	int nRecvBY;
	string ssLocalIP;
	string ssSvrName;
	int nGameID;
	int nAreaID;
	int nSubUserID;
};

//开始加速上报
struct STParamLogStart 
{
	STParamLogStart()
	{
		nUserID = nSvrID = nConnType = nGameID = nAreaID = nSubUserID;
	}
	int nUserID;
	int nSvrID;
	int nConnType;
	string ssHardSeries;
	string ssLocalIP;
	string ssSvrName;
	int nGameID;
	int nAreaID;
	int nSubUserID;
};

//停止加速上报
struct STParamLogEnd
{
	STParamLogEnd()
	{
		nUserID = nSvrID = nConnType = nSessionID = nSubUserID;
	}
	int nUserID;
	int nSvrID;
	int nConnType;
	string ssHardSeries;
	string ssLocalIP;
	int nSessionID;
	string ssNodeIP;
	int nSubUserID;
};

struct STGameArea
{
	STGameArea()
	{
		nID = 0;
	}
	int nID;
	wstring wsTitle;
	wstring wsSubTitle;
	string ssRules;
	string ssLogoUrl;
	wstring wsLogoPath;
	std::vector<string> vctSvr;
};

struct STGame
{
	STGame()
	{
		nGameID = 0;
		nHotGame = 0;
		nIsFree = 0;
		nPlatID = 0;
	}
	wstring wsGameTitle;
	wstring wsSubTitle;
	int nGameID;
	wstring wsAlias;
	int nHotGame;
	int nIsFree;
	int nPlatID;			//外服根据平台ID放入对应分组显示
	string ssRules;
	string ssLogoUrl;
	string ssBgUrl;
	wstring wsLogoPath;
	wstring wsBgPath;
	vector<STGameArea> vctAreas;
};

struct STGameList
{
	STGameList()
	{
		nCount = 0;
		nRegion = 0;
	}

	string ssLastEdit;
	int nCount;
	int nRegion;   //1:chn, 2:forign
	vector<STGame> vctGames;
};

struct STGamePlatItem
{
	int nPlatID;
	wstring wsPlatName;
	string ssRule;
	wstring wsTipMsg;
};

struct STGamePlat
{
	vector<STGamePlatItem> vctPlats;
};

struct STCountry
{
	STCountry()
	{
		nCode = 0;
	}
	int nCode;
	wstring wsName;
	string ssGroup;
	string ssIcoUrl;
	string ssIsoCode;
};

struct STCountryList
{
	vector<STCountry> vctTopCountry;
	vector<STCountry> vctListCountry;
};

struct STProvinceItem
{
	wstring wsKey;
	wstring wsName;
};

struct STProvice
{
	vector<STProvinceItem> vctProvince;
};

/*关于版本号的本地配置*/
struct STGameListConfigration
{
	STGameListConfigration()
	{
		nEnginDns = 0;
		nLogFile = 0;
		nLogTrace = 0;
		nEnginDbg = 0;
		nAccMode = 0;
		nTransPortMode = 0;
		nExitMode = 0;
		nExitFix = 0;
		nTestData = 0;
		nTestUrl = 0;
		nTestDebug = 0;
	}
	string ssVersion0;
	string ssVersion1;
	string ssLayoutVersion0;
	string ssLayoutVersion1;
	string ssUsed;
	int nEnginDns;
	int nLogFile;
	int nLogTrace;
	int nEnginDbg;
	int nAccMode;		//0:自动 \1:lsp模式\2:HOOK模式\3:Driver
	int nTransPortMode; //0:极速\1: 兼容
	int nExitMode;		//0:退出\1:到托盘
	int nExitFix;		//1:不再提示
	int nTestData;
	int nTestUrl;
	int nTestDebug;
	std::set<int> poolgame;
};

struct STLineItem
{
	STLineItem()
	{
		nLineID = 0;
		nProtoType = 0;
		nIspType = 0;
		nDelay = 0;
	}
	STLineItem(wstring Name, wstring Value)
	{
		wsName = Name;
		wsValue = Value;
		nLineID = 0;
		nProtoType = 0;
		nIspType = 0;
		nDelay = 0;
	}
	wstring wsName;
	wstring wsValue;
	wstring wsLineName;
	int nLineID;
	int nProtoType;
	int nIspType;
	int nDelay;
	string ssSvr;
};

struct STGroupItem
{
	STGroupItem()
	{
		nGroupID = 0;
		nDelay = 0;
	}
	wstring wsGroupName;
	wstring wsGroupValue;
	wstring wsProvName;
	int nGroupID;
	vector<STLineItem> vctGroupLines;
	int nDelay;

};

/*线路分组*/
struct STLineGroup
{
	wstring wsResult;
	int nGroupCount;
	vector<STGroupItem> vctGroups;
};

struct STLineGroupStrore
{
	STLineGroupStrore()
	{
		nGameID = 0;
		nAreaID = 0;
	}
	bool IsValid()
	{
		return nGameID && nAreaID && stData.vctGroups.size();
	}
	int nGameID;
	int nAreaID;
	STLineGroup stData;
};

/*线路列表*/
struct STLineList
{
	wstring wsResult;
	int nGroupCount;
	vector<STLineItem> vctMember;
};

/*节点扩展信息和认证*/
struct STACCInfo
{
	STACCInfo()
	{
		nID = 0;
		nUserID = 0;
		nFeePerTime = 0;
		nAddSvrPort = 0;
		nPort = 0;
		nErrCode = 0;
	}

	wstring wsResult;
	wstring wsUserName;
	int nID;
	int nUserID;
	int nFeePerTime;
	wstring wsPassWord;
	string ssRules;
	string ssSvr;
	int nPort;
	string ssVpnSvr;
	string ssVpnPwd;
	string ssTokenUser;
	string ssTokenPwd;
	string ssAddSvr;
	string ssAddSvrArea;
	int nAddSvrPort;
	string ssAddSvrEx;
	string ssAddSvrAreaEx;
	string ssAddSvrPortEx;
	string ssBandWidth;
	string ssProcessRule;
	int nErrCode;
};

/*服务器设置*/
struct STSvrSet
{
	wstring wsResult;
	string ssZantingUrl;
	string ssZantingUrl2;
	string ssBaseUrl;
	string ssUrl;
	string ssKeFuUrl;
	string ssHelpUrl;
	string ssNewUrl;
	string ssXufeiUrl;
	string ssADUrl;
	string ssUrl2;
	string ssClientProto;
	string ssPngUrlSM[2];
	string ssPngUrlBY[2];
	string ssTargetUrlSM[2];
	string ssTargetUrlBY[2];
	wstring wsPngPathSM[2];
	wstring wsPngPathBY[2];
	wstring wsADTitle;
	int nActiveID = 0;
};

struct STUpdateTokenResponse
{
	int nExpireed = 0;
	string ssResult;
	string ssTokenUser;
	string ssTokenPass;
};

/*地区信息*/

struct STAreaGreat
{
	string ssKey;
	wstring wsName;
};

struct STAreaList
{
	vector<STAreaGreat> vctAreas;
};

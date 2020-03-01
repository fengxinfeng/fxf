#pragma once
#include "base_singleton.h"
#include "engine_define.h"


class CAccountLogin
{
public:
	CAccountLogin();
	~CAccountLogin();
public:
	bool initLogin();
	void ThreadCallInit();
	STCountryList & GetCountryCode() { return m_CountryList; }
	STGameList & GetGameListChn() { return m_stGameListChn; }
	STGameList & GetGameListFor() { return m_stGameListFor; }
	STGamePlat & GetGamePlat() { return m_stGamePlat; }
private:
	STCountryList m_CountryList;
	STGameList	  m_stGameListChn;
	STGameList	  m_stGameListFor;
	STGamePlat    m_stGamePlat;
};

#define BH_LOGIC_CENTER		CSingleton<CAccountLogin>::instance
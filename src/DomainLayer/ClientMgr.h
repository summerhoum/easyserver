#pragma once

#include <map>
#include "Singleton.h"
#include "Client.h"

class CClientMgr : public CSingleton<CClientMgr>
{
public:
	int Initialize();
	CClient* CreateNewClient(int iFd, const char* ip, int iPort);
	void DestoryClient(CClient *pClient);
	CClient* FindClient(int iFd);
private:
	std::map<int, CClient*> m_mapClientMap;
};

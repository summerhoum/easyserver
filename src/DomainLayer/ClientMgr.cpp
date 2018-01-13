#include "ClientMgr.h"
#include "LogUtil.h"
#include "ConfigMgr.h"

int CClientMgr::Initialize()
{
	m_mapClientMap.clear();
	return 0;
}

CClient * CClientMgr::CreateNewClient(int iFd, const char * pszIP, int iPort)
{
	LOG(DEBUG) << "create new client: fd=" << iFd
		<< ", pszIP=" << pszIP << ", port=" << iPort;

	int iCurrentClientNumber = m_mapClientMap.size();
	if (iCurrentClientNumber >= CConfigMgr::Inst().GetMaxClientNumber())
	{
		LOG(ERROR) << "iCurrentClientNumber is " << iCurrentClientNumber << ", cannot create more client";
		return NULL;
	}

	if (FindClient(iFd) != NULL)
	{
		LOG(ERROR) << "fd is inuse, something is wrong";
		return NULL;
	}

	CClient *pClient = new CClient(iFd, pszIP, iPort);
	if (NULL == pClient)
	{
		LOG(ERROR) << "create client error, pszIP = " << pszIP << ", port = " << iPort;
		return NULL;
	}
	
	m_mapClientMap[iFd] = pClient;

	return pClient;
}

void CClientMgr::DestoryClient(CClient * pClient)
{
	APP_CHECK_NULL(pClient, );
	LOG(DEBUG) << "destory client: " << pClient->GetLogInfo();
	std::map<int, CClient *>::iterator iter = m_mapClientMap.find(pClient->GetFd());
	if (iter != m_mapClientMap.end())
	{
		m_mapClientMap.erase(iter);
	}
	else
	{
		LOG(ERROR) << "client is not in map: " << pClient->GetLogInfo();
	}

	delete pClient;
}

CClient* CClientMgr::FindClient(int iFd)
{
	std::map<int, CClient *>::iterator iter = m_mapClientMap.find(iFd);
	if (iter != m_mapClientMap.end())
	{
		return iter->second;
	}
	
	return NULL;
}

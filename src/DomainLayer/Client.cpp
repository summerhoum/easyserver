#include "Client.h"
#include "LogUtil.h"

#define LOGCLIENT(LEVEL) LOG(LEVEL) << GetLogInfo() << ": "

CClient::CClient(int iFd, const char *pszAddr, int iPort)
{
	m_stEvIo.data = (void*)this;
	m_iFd = iFd;
	snprintf(m_szAddress, sizeof(m_szAddress), "%s:%d", pszAddr, iPort);
	m_iLastRecvTime = 0;
}

CClient::~CClient()
{
}

std::string CClient::GetLogInfo()
{
	char szStr[1024];
	snprintf(szStr, sizeof(szStr), "client(%s)", m_szAddress);
	std::string sLogInfo(szStr);
	return sLogInfo;
}

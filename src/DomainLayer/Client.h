#pragma once

#include "ev.h"
#include "SvrDefine.h"
#include <string>

class CClient
{
public:
	CClient(int iFd, const char *pszAddr, int iPort);
	~CClient();

public:  //  Ù–‘œ‡πÿ
	void SetLastRecvTime(int iTime) { m_iLastRecvTime = iTime; }
	int GetLastRecvTime() { return m_iLastRecvTime; }

	ev_io *GetEvIo() { return &m_stEvIo; }
	int GetFd() { return m_iFd; }

	std::string GetLogInfo();
private:
	int m_iFd;
	char m_szAddress[128];
	ev_io m_stEvIo;
	int m_iLastRecvTime;
};

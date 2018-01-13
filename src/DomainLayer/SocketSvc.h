#pragma once

#include <iostream>
#include <map>
#include "ev.h"
#include "Singleton.h"

class CClient;

class CSocketSvc : public CSingleton<CSocketSvc>
{
public:
	CSocketSvc();
	virtual ~CSocketSvc();

	int Initialize(int iPort, struct ev_loop *pstMainLoop);
	void DisConnectClient(CClient *pClient);
private:
	int m_iSvrFD;
	ev_io m_stSvrEvIo;
	int m_iSvrPort;
	struct ev_loop *m_pstMainLoop;
};

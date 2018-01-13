#include "Application.h"
#include "LogUtil.h"
#include "ConfigMgr.h"
#include "SocketSvc.h"
#include "SvrDefine.h"
#include "ClientMgr.h"

static void TickHandle(struct ev_loop *pstMainLoop, struct ev_periodic *w, int revents)
{
	CApp::Inst().Tick();
}

CApp::CApp()
{

}

CApp::~CApp()
{
	ev_break(m_pstMainLoop, EVBREAK_ALL);
}

int CApp::Initialize()
{
	LOG(INFO) << "===========version: " << VERSION << " ===========";

	int iRet = 0; 
	iRet = CConfigMgr::Inst().LoadConf();
	APP_CHECK_RET(iRet, "CConfigMgr::Inst().LoadConf()");

	iRet = CClientMgr::Inst().Initialize();
	APP_CHECK_RET(iRet, "CClientMgr::Inst().Initialize");

	m_pstMainLoop = ev_default_loop(0);
	
	int iPort = CConfigMgr::Inst().GetPort();
	iRet = CSocketSvc::Inst().Initialize(iPort, m_pstMainLoop);
	APP_CHECK_RET(iRet, "m_oSocketSvc.Initialize");

	ev_periodic_init(&m_stTickWatcher, TickHandle, 0, 0.1, NULL); // 10ms
	ev_periodic_start(m_pstMainLoop, &m_stTickWatcher);

	return 0;
}

int CApp::Run()
{
	LOG(ERROR) << "capp proc...";
	ev_run(m_pstMainLoop, 0);

	return 0;
}

int CApp::Tick()
{
	//LOG(DEBUG) << "tick...";
	return 0;
}

int CApp::NowSec()
{
	ev_tstamp tstamp = ev_time();

	return (int)tstamp;
}

int64_t CApp::NowMilliSec()
{
	ev_tstamp tstamp = ev_time();
	long lMilliSec = (int)(tstamp * 1000);
	return lMilliSec;
}

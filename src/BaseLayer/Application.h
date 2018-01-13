#pragma once

#include <stdint.h>
#include "Singleton.h"
#include "ev.h"

class CApp : public CSingleton<CApp>
{
public:
	CApp();
	virtual ~CApp();

	int Initialize();
	int Run();
	int Tick();
	int NowSec();
	int64_t NowMilliSec();
	int ReRun();
	struct ev_loop *GetMainLoop() { return m_pstMainLoop; }
private:
	struct ev_loop *m_pstMainLoop;
	struct ev_periodic m_stTickWatcher;
};

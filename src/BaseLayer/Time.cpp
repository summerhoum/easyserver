#include "Time.h"
#include "Application.h"

int CTime::NowSec()
{
	return CApp::Inst().NowSec();;
}

int64_t CTime::NowMilliSec()
{
	return CApp::Inst().NowMilliSec();
}

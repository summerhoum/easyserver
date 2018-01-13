#include "Application.h"
#include "LogUtil.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
	el::Configurations conf("../cfg/log.cfg");
	el::Loggers::reconfigureAllLoggers(conf);

	OSSLOG << "system is starting...";

	int iRet = CApp::Inst().Initialize();
	if (iRet != 0)
	{
		LOG(ERROR) << "system start error";
		return -1;
	}

	OSSLOG << "system start succ";
	CApp::Inst().Run();
	
	return 0;
}

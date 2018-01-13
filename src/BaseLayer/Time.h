#pragma once

#include <stdint.h>

class CTime
{
public:
	static int NowSec();
	static int64_t NowMilliSec();
};

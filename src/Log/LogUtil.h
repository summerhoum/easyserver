#ifndef __LOG_UTIL_H_
#define __LOG_UTIL_H_

#include "easylogging++.h"

#define APP_CHECK_RET(iRet, msg) \
    do{\
        if (iRet != 0) { LOG(ERROR) << msg << " error, iRet=" << iRet; return iRet; }\
    } while (0);

#define APP_CHECK_NULL(pPointer, iReturn) \
    do{\
        if (NULL == pPointer) { LOG(ERROR) << "pPointer is null"; return iReturn; }\
    } while (0);

#define  OSSLOG LOG(TRACE)

#endif

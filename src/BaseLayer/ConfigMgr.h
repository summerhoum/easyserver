#pragma once

#include <fstream>
#include <map>
#include "StringUtil.h"
#include "Singleton.h"

using std::map;

#define CFG_COMMENT_CHAR '#'
#define CFG_PORT "Port"
#define CFG_MAX_CLIENT_NUMBER "MaxClientNumber"
#define CFG_DISCONNECT_AFTER_PROC_COMMAND "DisconnectAfterProcCommand"

class CConfigMgr : public CSingleton<CConfigMgr>
{
public:
	CConfigMgr() {}
	virtual ~CConfigMgr() {}
	int LoadConf();

	int GetPort() { return m_iPort; }
	int GetMaxClientNumber() { return m_iMaxClientNumber; }
	bool IsDisconnectAfterProcCommand() { return m_bDisconnectAfterProcCommand; }
private:
	bool IsCommentChar(char c);
	bool AnalyseLine(const string & line, string & key, string & value);
	void PrintConfig();
	string GetValue(const char *pszKey);
private:
	map<string, string> m_mConf;
	int m_iPort;
	int m_iMaxClientNumber;
	bool m_bDisconnectAfterProcCommand;
};

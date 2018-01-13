#include "ConfigMgr.h"
#include "LogUtil.h"

int CConfigMgr::LoadConf()
{
	m_mConf.clear();
	const char *pszFileName = "../cfg/system.cfg";
	std::ifstream infile(pszFileName);
	if (!infile) {
		LOG(ERROR) << "file open error";
		return -1;
	}

	string line, key, value;
	while (getline(infile, line)) {
		if (AnalyseLine(line, key, value)) {
			if (value.empty())
			{
				LOG(ERROR) << "configuration is invalid , key=" << key << ", value=" << value;
				return -1;
			}
			m_mConf[key] = value;
		}
	}

	infile.close();

	PrintConfig();

	m_iPort = atoi(GetValue(CFG_PORT).c_str());
	m_iMaxClientNumber = atoi(GetValue(CFG_MAX_CLIENT_NUMBER).c_str());
	m_bDisconnectAfterProcCommand = atoi(GetValue(CFG_DISCONNECT_AFTER_PROC_COMMAND).c_str());

	return 0;
}

string CConfigMgr::GetValue(const char *pszKey)
{
	string sKey(pszKey);
	if (m_mConf.find(sKey) != m_mConf.end())
	{
		return m_mConf[sKey];
	}

	return "";
}

bool CConfigMgr::IsCommentChar(char c)
{
	switch (c) {
	case CFG_COMMENT_CHAR:
		return true;
	default:
		return false;
	}
}

bool CConfigMgr::AnalyseLine(const string & line, string & key, string & value)
{
	string input = line;
	input = StrTrimEx(input);
	if (input.empty())
	{
		return false;
	}

	if (input[0] == CFG_COMMENT_CHAR)  // ×¢ÊÍÐÐ
	{
		return false;
	}

	int pos = input.find('=');
	if (pos == -1)
	{
		return false;  // Ã»ÓÐ=ºÅ
	}

	key = input.substr(0, pos);
	value = input.substr(pos + 1, input.size());

	key = StrTrim(key);
	if (key.empty()) {
		return false;
	}

	value = StrTrim(value);

	return true;
}

void CConfigMgr::PrintConfig()
{
	map<string, string>::const_iterator it = m_mConf.begin();
	for (; it != m_mConf.end(); ++it) {
		LOG(DEBUG) << it->first << "=" << it->second;
	}
}

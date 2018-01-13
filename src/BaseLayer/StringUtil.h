#ifndef __STRING_UTIL_H_
#define __STRING_UTIL_H_

#include <vector>  
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

inline void StrSplit(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}

	if (pos1 != s.length())
	{
		v.push_back(s.substr(pos1));
	}
}

inline std::string& StrTrim(std::string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

inline std::string& StrTrimEx(std::string &s)  // »áÈ¥µô»»ÐÐ·û
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	s.erase(s.find_last_not_of("\r") + 1);
	s.erase(s.find_last_not_of("\n") + 1);

	return s;
}

inline std::string LongToString(long l)
{
	stringstream stream;
	stream << l;
	return stream.str();
}

inline bool IsSpace(char c)
{
	if (' ' == c || '\t' == c)
		return true;
	return false;
}

#endif


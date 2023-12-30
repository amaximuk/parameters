#pragma once

#include <string>

namespace string_helper
{
	// trim from start (in place)
	void ltrim(std::string& s, char t = ' ')
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [t](int ch) { return !(ch == t); }));
	}

	// trim from end (in place)
	void rtrim(std::string& s, char t = ' ')
	{
		auto rptr = std::find_if(s.rbegin(), s.rend(), [t](int ch) { return !(ch == t); });
		if (rptr != s.rend())
			s.erase(rptr.base(), s.end());
	}

	// trim from both ends (in place)
	void trim(std::string& s, char t = ' ')
	{
		ltrim(s, t);
		rtrim(s, t);
	}

	// trim from start (copying)
	std::string ltrim_copy(const std::string& s, char t = ' ')
	{
		auto ss(s);
		ltrim(ss, t);
		return ss;
	}

	// trim from end (copying)
	std::string rtrim_copy(const std::string& s, char t = ' ')
	{
		auto ss(s);
		rtrim(ss, t);
		return ss;
	}

	// trim from both ends (copying)
	std::string trim_copy(const std::string& s, char t = ' ')
	{
		auto ss(s);
		trim(ss, t);
		return ss;
	}
}

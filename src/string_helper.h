#pragma once

#include <string>

namespace string_helper
{
	template <typename... Args> std::string make_string(Args&&... args)
	{
		std::stringstream out;
		using expander = int[];
		(void)expander {
			0, (void(out << std::forward<Args>(args)), 0)...
		};
		return out.str();
	}

	// trim from start (in place)
	void ltrim(std::string& s, char t = ' ');

	// trim from end (in place)
	void rtrim(std::string& s, char t = ' ');

	// trim from both ends (in place)
	void trim(std::string& s, char t = ' ');

	// trim from start (copying)
	std::string ltrim_copy(const std::string& s, char t = ' ');

	// trim from end (copying)
	std::string rtrim_copy(const std::string& s, char t = ' ');

	// trim from both ends (copying)
	std::string trim_copy(const std::string& s, char t = ' ');
}

#pragma once

#include <string>
#include <vector>
#include <list>
#include "yaml_types.h"

namespace yaml
{
	class wiki_formatter
	{
	private:
		bool is_batch_;
		file_info file_info_;

	public:
		wiki_formatter(bool is_batch);

	public:
		static bool format(const file_info& file_info, bool is_batch, std::list<std::string>& wiki);

	public:
		bool format(const file_info& file_info, std::list<std::string>& wiki);

	private:
		bool get_file_wiki(const file_info& fi, std::list<std::string>& wiki);
		bool get_type_wiki(const type_info& ti, std::list<std::string>& wiki);
		bool get_main_wiki(const file_info& fi, std::list<std::string>& wiki);
		bool get_parameter_wiki(const parameter_info& pi, std::list<std::string>& wiki);
		bool get_restrictions_wiki(const parameter_info& pi, std::string& wiki);

		std::string get_is_required_wiki(const parameter_info& pi) const;
		std::string get_type_wiki(const parameter_info& pi) const;
		std::string get_type_xml_wiki(const parameter_info& pi) const;
	};
}

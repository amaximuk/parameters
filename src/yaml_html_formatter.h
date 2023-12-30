#pragma once

#include <string>
#include <vector>
#include <list>
#include "yaml_types.h"

namespace yaml
{
	class html_formatter
	{
	private:
		bool is_batch_;
		file_info file_info_;

	public:
		html_formatter(bool is_batch);

	public:
		static bool format(const file_info& file_info, bool is_batch, std::list<std::string>& html);

	public:
		bool format(const file_info& file_info, std::list<std::string>& html);

	private:
		bool get_file_html(const file_info& fi, std::list<std::string>& html);
		bool get_type_html(const type_info& ti, std::list<std::string>& html);
		bool get_main_html(const file_info& fi, std::list<std::string>& html);
		bool get_parameter_html(const parameter_info& pi, std::list<std::string>& html);
		bool get_restrictions_html(const parameter_info& pi, std::string& html);

		std::string get_is_required_html(const parameter_info& pi) const;
		std::string get_type_html(const parameter_info& pi) const;
		std::string get_type_xml_html(const parameter_info& pi) const;
	};
}

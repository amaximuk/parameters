#pragma once

#include <string>
#include <vector>
#include <list>
#include "parameters/types.h"

namespace parameters
{
	namespace formatter
	{
		class code_formatter
		{
		private:
			bool is_batch_;
			file_info file_info_;

		public:
			code_formatter(bool is_batch);

		public:
			static bool format(const file_info& file_info, bool is_batch, std::list<std::string>& code);

		public:
			bool format(const file_info& file_info, std::list<std::string>& code);

		private:
			bool get_file_code(const file_info& fi, std::list<std::string>& code);
			bool get_type_code(const type_info& ti, std::list<std::string>& code);
			bool get_main_code(const file_info& fi, std::list<std::string>& code);
			bool get_struct_members_code(const std::vector<parameter_info>& parameters, std::list<std::string>& code);
			bool get_struct_parse_code(const std::vector<parameter_info>& parameters, std::list<std::string>& code);
			bool get_parameter_code(const parameter_info& pi, std::list<std::string>& code);
			bool get_restrictions_code(const parameter_info& pi, bool for_array, std::list<std::string>& code);


			std::string get_convert_function_name_text(const type_info& ti) const;
			std::string get_name_text(const parameter_info& pi) const;
			std::string get_optional_name_text(const parameter_info& pi) const;
			std::string get_name_name_text(const parameter_info& pi) const;
			std::string get_value_name_text(const parameter_info& pi) const;
			std::string get_count_name_name_text(const parameter_info& pi) const;
			std::string get_count_value_name_text(const parameter_info& pi) const;
			std::string get_count_xml_name_text(const parameter_info& pi) const;
			std::string get_value_set_name_text(const parameter_info& pi) const;
			std::string get_count_value_set_name_text(const parameter_info& pi) const;
		};
	}
}

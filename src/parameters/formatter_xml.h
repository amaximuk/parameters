#pragma once

#include <string>
#include <vector>
#include <list>
#include "parameters/types.h"

namespace parameters
{
	namespace formatter
	{
		class xml_formatter
		{
		private:
			bool is_batch_;
			file_info file_info_;

		public:
			xml_formatter(bool is_batch);

		public:
			static bool format(const file_info& file_info, bool is_batch, std::list<std::string>& xml);

		public:
			bool format(const file_info& file_info, std::list<std::string>& xml);

		private:
			bool get_file_xml(const file_info& fi, std::list<std::string>& xml);
			bool get_parameters_xml(const std::vector<parameter_info>& parameters, std::list<std::string>& xml);
			bool get_parameter_xml(const parameter_info& pi, std::list<std::string>& xml);

			int get_restricted_count_xml(const parameter_info& pi) const;
			std::string get_restricted_value_xml(const parameter_info& pi) const;
		};
	}
}

#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace file
		{
			// Получить версию формата файла
			int get_format(const file_info& fi);

			//
			std::string get_display_name(const file_info& fi);
			
			//
			std::vector<std::string> get_type_types();
			
			//
			std::vector<std::string> get_parameter_types(const file_info& fi);
			
			//
			std::vector<std::string> get_user_types(const file_info& fi);


			bool set_type(file_info& fi, const std::string& type, const parameters::type_info& ti, bool exclude_parameters);
			bool rename_type(file_info& fi, const std::string& oldName, const std::string& newName);
			bool rearrange_types(file_info& fi, bool& have_type_loop);
			bool validate(file_info& fi, std::string& message);

		}
	}
}

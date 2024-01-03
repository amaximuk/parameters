#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{

		// parameter_info (restrictions_info)

		namespace restrictions
		{
			bool have_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool add_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool remove_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool move_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up);

			bool have_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool add_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool remove_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool move_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up);

			bool have_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool add_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool remove_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool move_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up);

		}
	}
}

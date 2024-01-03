#pragma once

#include <string>
#include <vector>
#include "json/json.h"
#include "parameters/types.h"

namespace parameters
{
	namespace json
	{
		class writer
		{
		public:
			static bool write(const std::string& filename, parameters::file_info& fi);

		private:
			static bool write_file_info(Json::Value& emitter, const parameters::file_info& fi);
			static bool write_info_info(Json::Value& emitter, const parameters::info_info& ii);
			static bool write_type_info(Json::Value& emitter, const parameters::type_info& ti);
			static bool write_parameter_info(Json::Value& emitter, const parameters::parameter_info& pi);
		};
	}
}

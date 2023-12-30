#pragma once

#include <string>
#include <vector>
#include "json/json.h"
#include "yaml_types.h"

namespace json
{
	class writer
	{
	public:
		static bool write(const std::string& filename, yaml::file_info& fi);

	private:
		static bool write_file_info(Json::Value& emitter, const yaml::file_info& fi);
		static bool write_info_info(Json::Value& emitter, const yaml::info_info& ii);
		static bool write_type_info(Json::Value& emitter, const yaml::type_info& ti);
		static bool write_parameter_info(Json::Value& emitter, const yaml::parameter_info& pi);
	};
}

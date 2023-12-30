#pragma once

#include <string>
#include <vector>
#include "yaml_types.h"
#include "yaml-cpp/yaml.h"

namespace yaml
{
	struct writer
	{
	public:
		bool write(const std::string& filename, yaml::file_info& fi);

	private:
		bool write_file_info(YAML::Emitter& emitter, const yaml::file_info& fi);
		bool write_info_info(YAML::Emitter& emitter, const yaml::info_info& ii);
		bool write_type_info(YAML::Emitter& emitter, const yaml::type_info& ti);
		bool write_parameter_info(YAML::Emitter& emitter, const yaml::parameter_info& pi);
	};
}

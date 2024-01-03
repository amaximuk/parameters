#pragma once

#include <string>
#include <vector>
#include "parameters/types.h"
#include "yaml-cpp/yaml.h"

namespace parameters
{
	namespace yaml
	{
		struct writer
		{
		public:
			bool write(const std::string& filename, parameters::file_info& fi);

		private:
			bool write_file_info(YAML::Emitter& emitter, const parameters::file_info& fi);
			bool write_info_info(YAML::Emitter& emitter, const parameters::info_info& ii);
			bool write_type_info(YAML::Emitter& emitter, const parameters::type_info& ti);
			bool write_parameter_info(YAML::Emitter& emitter, const parameters::parameter_info& pi);
		};
	}
}

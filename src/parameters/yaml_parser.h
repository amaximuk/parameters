#pragma once

#include <string>
#include <vector>
#include "parameters/types.h"
#include "yaml-cpp/yaml.h"

namespace parameters
{
	namespace yaml
	{
		class parser
		{
		private:
			bool is_batch_;

		public:
			parser(bool is_batch);

		public:
			static bool parse(const std::string& filename, bool is_batch, file_info& file_info);

		public:
			bool parse(const std::string& filename, file_info& file_info);

		private:
			template<typename T> bool try_get_yaml_value(const YAML::Node& node, const std::string& name, T& value);
			bool get_file_info(const YAML::Node& node, file_info& fi);
			bool get_info_info(const YAML::Node& node, info_info& ui);
			bool get_type_info(const YAML::Node& node, const std::vector<type_info>& type_infos, type_info& ti);
			bool get_parameter_info(const YAML::Node& node, const std::vector<type_info>& type_infos, parameter_info& pi);
		};
	}
}

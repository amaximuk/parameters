#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include "parameters/types.h"

namespace parameters
{
	const std::map<std::string, std::string> type_to_xml_types = { {"yml", ""}, {"enum", "str"} };
	const std::map<std::string, std::string> system_to_xml_types = { {"unit", "str"}, {"path", "str"}, {"string", "str"}, {"library", "lib"} };
	const std::map<std::string, std::string> cpp_to_xml_types = { {"double", "dbl"}, {"int", "int"}, {"bool", "bool"}, {"std::string", "str"},
		{"float", "dbl"}, {"int8_t", "int"}, {"int16_t", "int"}, {"int32_t", "int"}, {"int64_t", "int"}, {"uint8_t", "int"}, {"uint16_t", "int"},
		{"uint32_t", "int"}, {"uint64_t", "int"} };
	const std::map<std::string, std::string> xml_to_cpp_types = { {"dbl", "double"}, {"int", "int"}, {"bool", "bool"}, {"str", "std::string"},
		{"lib", "std::string"} };

	const std::vector<std::string> type_types = { "yml", "enum" };
	const std::vector<std::string> parameter_types = { "unit", "path", "string", "library",
		"double", "int", "bool", "std::string", "float", "int8_t", "int16_t",
		"int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
		"array<unit>", "array<path>", "array<string>", "array<library>",
		"array<double>", "array<int>", "array<bool>", "array<std::string>", "array<float>", "array<int8_t>", "array<int16_t>",
		"array<int32_t>", "array<int64_t>", "array<uint8_t>", "array<uint16_t>", "array<uint32_t>", "array<uint64_t>" };
	const std::map<std::string, base_types> parameter_to_base_types = {
		{"unit", base_types::string}, {"path", base_types::string}, {"string", base_types::string},
		{"library", base_types::string}, {"std::string", base_types::string},
		{"double", base_types::floating}, {"float", base_types::floating},
		{"bool", base_types::bool_},
		{"int", base_types::integer}, {"int8_t", base_types::integer}, {"int16_t", base_types::integer},
		{"int32_t", base_types::integer}, {"int64_t", base_types::integer},
		{"uint8_t", base_types::integer}, {"uint16_t", base_types::integer},
		{"uint32_t", base_types::integer}, {"uint64_t", base_types::integer} };
}

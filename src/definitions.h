#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include "parameters/types.h"

namespace parameters
{
	const std::map<type_types, std::string> type_type_strings = { {type_types::yml, "yml"}, {type_types::enum_, "enum"} };
	const std::map<system_types, std::string> system_type_strings = { {system_types::unit, "unit"}, {system_types::path, "path"},
		{system_types::string, "string"}, {system_types::library, "library"} };
	const std::map<cpp_types, std::string> cpp_type_strings = { {cpp_types::double_, "dbl"}, {cpp_types::int_, "int"},
		{cpp_types::bool_, "bool"}, {cpp_types::std_string, "str"}, {cpp_types::float_, "dbl"}, {cpp_types::int8_t, "int"},
		{cpp_types::int16_t, "int"}, {cpp_types::int32_t, "int"}, {cpp_types::int64_t, "int"}, {cpp_types::uint8_t, "int"},
		{cpp_types::uint16_t, "int"}, {cpp_types::uint32_t, "int"}, {cpp_types::uint64_t, "int"} };
	
	const std::map<std::string, std::string> type_to_xml_types = { {"yml", ""}, {"enum", "str"} };
	const std::map<std::string, std::string> system_to_xml_types = { {"unit", "str"}, {"path", "str"}, {"string", "str"}, {"library", "lib"} };
	const std::map<std::string, std::string> cpp_to_xml_types = { {"double", "dbl"}, {"int", "int"}, {"bool", "bool"}, {"std::string", "str"},
		{"float", "dbl"}, {"int8_t", "int"}, {"int16_t", "int"}, {"int32_t", "int"}, {"int64_t", "int"}, {"uint8_t", "int"}, {"uint16_t", "int"},
		{"uint32_t", "int"}, {"uint64_t", "int"} };
	const std::map<std::string, std::string> xml_to_cpp_types = { {"dbl", "double"}, {"int", "int"}, {"bool", "bool"}, {"str", "std::string"},
		{"lib", "std::string"} };

	const std::vector<std::string> type_types_as_string = { "yml", "enum" };
	const std::vector<std::string> parameter_types_as_string = { "unit", "path", "string", "library",
		"double", "int", "bool", "std::string", "float", "int8_t", "int16_t",
		"int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
		"array<unit>", "array<path>", "array<string>", "array<library>",
		"array<double>", "array<int>", "array<bool>", "array<std::string>", "array<float>", "array<int8_t>", "array<int16_t>",
		"array<int32_t>", "array<int64_t>", "array<uint8_t>", "array<uint16_t>", "array<uint32_t>", "array<uint64_t>" };
	const std::map<std::string, base_item_types> parameter_to_base_item_types = {
		{"unit", base_item_types::string}, {"path", base_item_types::string}, {"string", base_item_types::string},
		{"library", base_item_types::string}, {"std::string", base_item_types::string},
		{"double", base_item_types::floating}, {"float", base_item_types::floating},
		{"bool", base_item_types::boolean},
		{"int", base_item_types::integer}, {"int8_t", base_item_types::integer}, {"int16_t", base_item_types::integer},
		{"int32_t", base_item_types::integer}, {"int64_t", base_item_types::integer},
		{"uint8_t", base_item_types::integer}, {"uint16_t", base_item_types::integer},
		{"uint32_t", base_item_types::integer}, {"uint64_t", base_item_types::integer} };
	const std::map<std::string, base_item_types> xml_to_base_item_types = {
		{"dbl", base_item_types::floating}, {"int", base_item_types::integer}, {"bool", base_item_types::boolean},
		{"str", base_item_types::string}, {"lib", base_item_types::string} };
}

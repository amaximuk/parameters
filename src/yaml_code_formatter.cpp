#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <regex>
#include "git_commit_hash.h"
#include "string_helper.h"
#include "yaml_helper.h"
#include "yaml_code_formatter.h"

using namespace yaml;
using namespace yaml::helper;

#define ELRF(message) do { if (!is_batch_) std::cout << message << std::endl; return false; } while(0)
#define INFO_LOG_PREFIX "base->Log(core::LOG_INFO, std::string(\"", common::get_as_cp1251(file_info_.info.id), "\") + \" (\" + base->GetSelfName() + \")\" + "
#define ERROR_LOG_PREFIX "base->Log(core::LOG_ERROR, std::string(\"", common::get_as_cp1251(file_info_.info.id), "\") + \" (\" + base->GetSelfName() + \")\" + "

code_formatter::code_formatter(bool is_batch)
{
	is_batch_ = is_batch;
}

bool code_formatter::format(const file_info& file_info, bool is_batch, std::list<std::string>& code)
{
	code_formatter formatter(is_batch);
	return formatter.format(file_info, code);
}

bool code_formatter::format(const file_info& file_info, std::list<std::string>& code)
{
	file_info_ = file_info;
	if (!get_file_code(file_info, code))
		ELRF("Get file code failed");
	return true;
}

bool code_formatter::get_file_code(const file_info& fi, std::list<std::string>& code)
{
	code.push_back("#pragma once");
	code.push_back("");
	code.push_back("////////////////////////////////////////////////////////////////////////////////");
	code.push_back("// This file was autogenerated with parameters_compiler");
	code.push_back("// Any changes to this file will be overwritten by the next CMake run");
	code.push_back(string_helper::make_string("// Git commit hash of parameters_compiler: ", GIT_COMMIT_HASH));
	code.push_back(string_helper::make_string("// Input file format: ", common::get_as_cp1251(fi.format)));
	code.push_back(string_helper::make_string("// Unit name: ", common::get_as_cp1251(fi.info.display_name)));
	code.push_back(string_helper::make_string("// Unit ID: ", common::get_as_cp1251(fi.info.id)));
	code.push_back("////////////////////////////////////////////////////////////////////////////////");
	code.push_back("");
	code.push_back("#include <string>");
	code.push_back("#include <vector>");
	code.push_back("#include <set>");
	code.push_back("#include <map>");
	code.push_back("#include \"base_library/base_library.h\"");
	std::list<std::string> includes_list;
	for (const auto& ti : fi.types)
	{
		if (type::get_category(ti) == type_category::user_cpp)
		{
			for (const auto& s : ti.includes)
			{
				if (std::find(includes_list.cbegin(), includes_list.cend(), s) == includes_list.cend())
					includes_list.push_back(s);
			}
		}
	}
	for (const auto& i : includes_list)
		code.push_back(string_helper::make_string("#include \"", i, "\""));
	code.push_back("");
	code.push_back("namespace parameters_compiler");
	code.push_back("{");
	code.push_back(string_helper::make_string("\t", "static const std::string git_commit_hash = \"@GIT_COMMIT_HASH@\";"));
	code.push_back(string_helper::make_string("\t", "static const std::string git_status = \"@GIT_STATUS@\";"));
	code.push_back("");

	for (const auto& ti : fi.types)
	{
		//if (type.category == type_category::user_yml)
		{
			std::list<std::string> type_code;
			if (!get_type_code(ti, type_code))
				ELRF("Get type code failed");
			for (const auto& s : type_code)
				code.push_back(string_helper::make_string("\t", s));
		}
		code.push_back("");
	}

	std::list<std::string> main_parameters_code;
	if (!get_main_code(fi, main_parameters_code))
		ELRF("Get main parameters code failed");
	for (const auto& s : main_parameters_code)
		code.push_back(string_helper::make_string("\t", s));

	code.push_back("}");

	return true;
}

std::string code_formatter::get_convert_function_name_text(const type_info& ti) const
{
	std::string function_name = common::get_as_cp1251(ti.name);
	std::transform(function_name.begin(), function_name.end(), function_name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	function_name = std::regex_replace(function_name, std::regex("::"), "_");

	return "convert_" + function_name;
}

std::string code_formatter::get_name_text(const parameter_info& pi) const
{
	std::string name_text = common::get_as_cp1251(pi.name);
	std::transform(name_text.begin(), name_text.end(), name_text.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return name_text;
}

std::string code_formatter::get_optional_name_text(const parameter_info& pi) const
{
	return get_name_text(pi) + "_is_set";
}

std::string code_formatter::get_name_name_text(const parameter_info& pi) const
{
	return get_name_text(pi) + "_parameter_name";
}

std::string code_formatter::get_value_name_text(const parameter_info& pi) const
{
	return get_name_text(pi) + "_parameter_value";
}

std::string code_formatter::get_count_name_name_text(const parameter_info& pi) const
{
	return get_name_text(pi) + "_parameters_count_name";
}

std::string code_formatter::get_count_value_name_text(const parameter_info& pi) const
{
	return get_name_text(pi) + "_parameter_count_value";
}

std::string code_formatter::get_count_xml_name_text(const parameter_info& pi) const
{
	return common::get_as_cp1251(pi.name) + "__CNT";
}

std::string code_formatter::get_value_set_name_text(const parameter_info& pi) const
{
	return get_value_name_text(pi) + "_set";
}

std::string code_formatter::get_count_value_set_name_text(const parameter_info& pi) const
{
	return get_count_value_name_text(pi) + "_set";
}

bool code_formatter::get_type_code(const type_info& ti, std::list<std::string>& code)
{
	if (type::get_category(ti) == type_category::user_yml)
	{
		code.push_back(string_helper::make_string("class ", common::get_as_cp1251(ti.name)));
		code.push_back(string_helper::make_string("{"));
		code.push_back(string_helper::make_string("public:"));

		std::list<std::string> struct_members_code;
		if (!get_struct_members_code(ti.parameters, struct_members_code))
			ELRF("Get struct members code failed");
		for (const auto& s : struct_members_code)
			code.push_back(string_helper::make_string("\t", s));

		code.push_back(string_helper::make_string(""));
		code.push_back(string_helper::make_string("\t", "static bool parse(BaseLibrary* base, const std::string& parent_name, ",
			common::get_as_cp1251(ti.name), "& value)"));
		code.push_back(string_helper::make_string("\t", "{"));

		std::list<std::string> parse_members_code;
		if (!get_struct_parse_code(ti.parameters, parse_members_code))
			ELRF("Get parse members code failed");
		for (const auto& s : parse_members_code)
			code.push_back(string_helper::make_string("\t", s));

		code.push_back(string_helper::make_string("\t\t", "return true;"));
		code.push_back(string_helper::make_string("\t", "}"));
		code.push_back(string_helper::make_string("};"));
	}
	else if (type::get_category(ti) == type_category::user_cpp)
	{
		code.push_back(string_helper::make_string("static bool ", get_convert_function_name_text(ti),
			"(BaseLibrary* base, const ", type::get_type_cpp(ti), "& xml_value, ", common::get_as_cp1251(ti.name), "& value)"));
		code.push_back(string_helper::make_string("{"));

		code.push_back(string_helper::make_string("\t", "std::map<", type::get_type_cpp(ti), ", ", common::get_as_cp1251(ti.name), "> enum_values;"));
		for (const auto& v : ti.values)
			code.push_back(string_helper::make_string("\t", "enum_values[\"", common::get_as_cp1251(v.first), "\"] = ", common::get_as_cp1251(v.second), ";"));

		code.push_back(string_helper::make_string("\t", "if (enum_values.count(xml_value) == 0)"));
		code.push_back(string_helper::make_string("\t", "{"));
		code.push_back(string_helper::make_string("\t\t", ERROR_LOG_PREFIX, "\": �������� �������� \" + xml_value + \" ����������� � ������������\");"));
		code.push_back(string_helper::make_string("\t\t", "return false;"));
		code.push_back(string_helper::make_string("\t", "}"));

		code.push_back(string_helper::make_string("\t", "value = enum_values[xml_value];"));

		code.push_back(string_helper::make_string("\t", "return true;"));
		code.push_back(string_helper::make_string("};"));
	}
	else
		ELRF("Unknown type of type");

	return true;
}

bool code_formatter::get_main_code(const file_info& fi, std::list<std::string>& code)
{
	code.push_back(string_helper::make_string("class ", "parameters"));
	code.push_back(string_helper::make_string("{"));
	code.push_back(string_helper::make_string("public:"));

	std::list<std::string> struct_members_code;
	if (!get_struct_members_code(fi.parameters, struct_members_code))
		ELRF("Get struct members code failed");
	for (const auto& s : struct_members_code)
		code.push_back(string_helper::make_string("\t", s));

	code.push_back(string_helper::make_string(""));
	code.push_back(string_helper::make_string("\t", "static bool parse(BaseLibrary* base, ", "parameters", "& value)"));
	code.push_back(string_helper::make_string("\t", "{"));
	code.push_back(string_helper::make_string("\t\t", "std::string parent_name = \"\";"));
	code.push_back(string_helper::make_string(""));
	code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": ��� �������: \" + git_commit_hash);"));
	code.push_back(string_helper::make_string("\t\t", "if (git_status != \"\")"));
	code.push_back(string_helper::make_string("\t\t\t", INFO_LOG_PREFIX, "\": ������: \" + git_status);"));
	code.push_back(string_helper::make_string(""));

	std::list<std::string> parse_members_code;
	if (!get_struct_parse_code(fi.parameters, parse_members_code))
		ELRF("Get parse members code failed");
	for (const auto& s : parse_members_code)
		code.push_back(string_helper::make_string("\t", s));

	code.push_back(string_helper::make_string("\t\t", "return true;"));
	code.push_back(string_helper::make_string("\t", "}"));
	code.push_back(string_helper::make_string("};"));

	return true;
}

bool code_formatter::get_struct_members_code(const std::vector<parameter_info>& parameters, std::list<std::string>& code)
{
	for (const auto& pi : parameters)
	{
		code.push_back(string_helper::make_string(parameter::get_type_class(file_info_, pi), " ", get_name_text(pi), ";"));
		if (parameter::get_is_optional(pi))
			code.push_back(string_helper::make_string("bool ", get_optional_name_text(pi), ";"));
	}
	return true;
}

bool code_formatter::get_struct_parse_code(const std::vector<parameter_info>& parameters, std::list<std::string>& code)
{
	for (const auto& pi : parameters)
	{
		std::list<std::string> parameter_code;
		if (!get_parameter_code(pi, parameter_code))
			ELRF("Get parameter code failed");
		for (const auto& s : parameter_code)
			code.push_back(s);
	}
	return true;
}

bool code_formatter::get_parameter_code(const parameter_info& pi, std::list<std::string>& code)
{
	if (parameter::get_is_array(pi))
	{
		code.push_back(string_helper::make_string("\t", "std::string ", get_name_name_text(pi), " = parent_name + \"",
			common::get_as_cp1251(pi.name), "\";"));
		code.push_back(string_helper::make_string("\t", "std::string ", get_count_name_name_text(pi), " = parent_name + \"",
			get_count_xml_name_text(pi), "\";"));
		code.push_back(string_helper::make_string("\t", "int ", get_count_value_name_text(pi), " = 0;"));
		code.push_back(string_helper::make_string("\t", "if (!base->GetParameter(", get_count_name_name_text(pi), ", ",
			get_count_value_name_text(pi), "))"));
		code.push_back(string_helper::make_string("\t\t", get_count_value_name_text(pi), " = 0;"));

		std::list<std::string> restrictions_code;
		get_restrictions_code(pi, true, restrictions_code);
		for (const auto& s : restrictions_code)
			code.push_back(string_helper::make_string("\t", s));

		code.push_back(string_helper::make_string(""));
		code.push_back(string_helper::make_string("\t", "for (int i = 0; i < ", get_count_value_name_text(pi), "; i++)"));
		code.push_back(string_helper::make_string("\t", "{"));
		if (parameter::get_category(file_info_, pi) == type_category::user_yml)
		{
			code.push_back(string_helper::make_string("\t\t", "std::string ", get_name_name_text(pi), " = parent_name + \"",
				common::get_as_cp1251(pi.name), "\"", " + ", "\"__\" + ", "std::to_string(i) + \"__\";"));
			code.push_back(string_helper::make_string("\t\t", parameter::get_item_type_class(file_info_, pi), " ",
				get_value_name_text(pi), "{};"));
			code.push_back(string_helper::make_string("\t\t", "if (!", parameter::get_item_type_class(file_info_, pi), "::parse(base, ",
				get_name_name_text(pi), ", ", get_value_name_text(pi), "))"));
			code.push_back(string_helper::make_string("\t\t", "{"));
			code.push_back(string_helper::make_string("\t\t\t", ERROR_LOG_PREFIX, "\": �������� ������ ��������� \" + ",
				get_name_name_text(pi), " + \"\");"));
			code.push_back(string_helper::make_string("\t\t\t", "return false;"));
			code.push_back(string_helper::make_string("\t\t", "}"));
			code.push_back(string_helper::make_string("\t\t", "value.", get_name_text(pi), ".push_back(",
				get_value_name_text(pi), ");"));

			// ���������������� ���� ����� ���� ������� ��������, ������� ������� ��������� � ���
			// ��� �������� ������ ���� ������� �������, ���� ������� � �������
		}
		else
		{
			code.push_back(string_helper::make_string("\t\t", "std::string ", get_name_name_text(pi), " = parent_name + \"",
				common::get_as_cp1251(pi.name), "\"", " + ", "\"__\" + ", "std::to_string(i);"));
			code.push_back(string_helper::make_string("\t\t", parameter::get_item_type_cpp(file_info_, pi), " ",
				get_value_name_text(pi), "{};"));
			code.push_back(string_helper::make_string("\t\t", "if (!base->GetParameter(",
				get_name_name_text(pi), ", ", get_value_name_text(pi), "))"));
			code.push_back(string_helper::make_string("\t\t", "{"));
			code.push_back(string_helper::make_string("\t\t\t", ERROR_LOG_PREFIX, "\": �������� �� �������� \" + ",
				get_name_name_text(pi), " + \"\");"));
			code.push_back(string_helper::make_string("\t\t\t", "return false;"));
			code.push_back(string_helper::make_string("\t\t", "}"));
			code.push_back("");

			std::list<std::string> restrictions_code;
			get_restrictions_code(pi, false, restrictions_code);
			for (const auto& s : restrictions_code)
				code.push_back(string_helper::make_string("\t\t", s));
			code.push_back("");

			// � �������������� �������� ����������� ����� �� ����� ���� ���������� �� ���������,
			// ������� ��� �������� ������ ���� ������� �������, ���� ������� � �������

			// ������� � ��� �������� ���������
			// ��������� ������� �� ���������� ��� � �����������������, ��� ��� �� ��� ��� ����� ���� ������������� � ������
			// ������� ����� std::to_string() ��� parameter_xml_type, ����� �����, ������� ��������� ��� ����
			if (parameter::get_item_type_cpp(file_info_, pi) == "std::string")
			{
				code.push_back(string_helper::make_string("\t\t", "if (", get_value_name_text(pi), " == \"\")"));
				code.push_back(string_helper::make_string("\t\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
					get_name_name_text(pi), " + \": \\\"\\\"\"", ");"));
				code.push_back(string_helper::make_string("\t\t", "else"));
				code.push_back(string_helper::make_string("\t\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
					get_name_name_text(pi), " + \": \" + ", get_value_name_text(pi), ");"));
			}
			else
				code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
					get_name_name_text(pi), " + \": \" + std::to_string(", get_value_name_text(pi), "));"));

			code.push_back(string_helper::make_string(""));
			// �������� ���������, ���� ��� ����������, �����������
			if (parameter::get_category(file_info_, pi) == type_category::user_cpp)
			{
				type_info* ti = type::get_type_info(file_info_, parameter::get_item_type(pi));
				if (ti == nullptr)
					ELRF("Unknown cpp type" << parameter::get_item_type(pi));

				code.push_back(string_helper::make_string("\t\t", "{"));
				code.push_back(string_helper::make_string("\t\t\t", parameter::get_item_type(pi), " temp_value{};"));
				code.push_back(string_helper::make_string("\t\t\t", "if (!", get_convert_function_name_text(*ti), "(base, ",
					get_value_name_text(pi), ", temp_value))"));
				code.push_back(string_helper::make_string("\t\t\t", "{"));
				code.push_back(string_helper::make_string("\t\t\t\t", ERROR_LOG_PREFIX, "\": �������� �� ������������� \" + ",
					get_name_name_text(pi), ");"));
				code.push_back(string_helper::make_string("\t\t\t\t", "return false;"));
				code.push_back(string_helper::make_string("\t\t\t", "}"));
				code.push_back(string_helper::make_string("\t\t\t", "value.", get_name_text(pi), ".push_back(temp_value);"));
				code.push_back(string_helper::make_string("\t\t", "}"));
			}
			else if (parameter::get_item_type_class(file_info_, pi) != parameter::get_item_type_cpp(file_info_, pi))
				code.push_back(string_helper::make_string("\t\t", "value.", get_name_text(pi), ".push_back(static_cast<",
					parameter::get_item_type_class(file_info_, pi), ">(", get_value_name_text(pi), "));"));
			else
				code.push_back(string_helper::make_string("\t\t", "value.", get_name_text(pi), ".push_back(",
					get_value_name_text(pi), ");"));

		}
		code.push_back(string_helper::make_string("\t", "}"));
		code.push_back(string_helper::make_string(""));
	}
	else
	{
		code.push_back(string_helper::make_string("\t", "std::string ", get_name_name_text(pi), " = parent_name + \"", common::get_as_cp1251(pi.name), "\";"));
		code.push_back(string_helper::make_string("\t", parameter::get_item_type_cpp(file_info_, pi), " ", get_value_name_text(pi), "{};"));
		code.push_back(string_helper::make_string("\t", "if (!base->GetParameter(", get_name_name_text(pi), ", ", get_value_name_text(pi), "))"));

		if (parameter::get_is_required(pi))
		{
			code.push_back(string_helper::make_string("\t", "{"));
			code.push_back(string_helper::make_string("\t\t", ERROR_LOG_PREFIX, "\": �� ����� ������������ �������� \" + ",
				get_name_name_text(pi), " + \"\");"));
			code.push_back(string_helper::make_string("\t\t", "return false;"));
			code.push_back(string_helper::make_string("\t", "}"));
		}
		else
		{
			code.push_back(string_helper::make_string("\t", "{"));
			if (parameter::get_is_optional(pi))
			{
				code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": �� ����� ������������ �������� \" + ",
					get_name_name_text(pi), " + \". ����������� �������� ", get_optional_name_text(pi), ": false\");"));
				code.push_back(string_helper::make_string("\t\t", "value.", get_optional_name_text(pi), " = false;"));
			}
			else
			{
				code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": �� ����� �������� \" + ",
					get_name_name_text(pi), " + \". ����������� �������� ��-���������: ",
					common::get_as_cp1251(pi.default_) == "" ? "\\\"\\\"" : common::get_as_cp1251(pi.default_), "\");"));
				if (parameter::get_item_type_cpp(file_info_, pi) == "std::string")
					code.push_back(string_helper::make_string("\t\t", get_value_name_text(pi), " = \"", common::get_as_cp1251(pi.default_), "\"; "));
				else if (parameter::get_item_type_class(file_info_, pi) != parameter::get_item_type_cpp(file_info_, pi))
					code.push_back(string_helper::make_string("\t\t", get_value_name_text(pi), " = static_cast<",
						parameter::get_item_type_cpp(file_info_, pi), ">(", common::get_as_cp1251(pi.default_), ");"));
				else
					code.push_back(string_helper::make_string("\t\t", get_value_name_text(pi), " = ", common::get_as_cp1251(pi.default_), "; "));
			}
			code.push_back(string_helper::make_string("\t", "}"));
		}

		// ������� � ��� �������� ���������, ���� ��� ������� �������
		// ��������� ������� �� ���������� ��� � �����������������, ��� ��� �� ��� ��� ����� ���� ������������� � ������
		// ������� ����� std::to_string() ��� parameter_xml_type, ����� �����, ������� ��������� ��� ����
		code.push_back(string_helper::make_string("\t", "else"));
		code.push_back(string_helper::make_string("\t", "{"));

		std::list<std::string> restrictions_code;
		get_restrictions_code(pi, false, restrictions_code);
		for (const auto& s : restrictions_code)
			code.push_back(string_helper::make_string("\t\t", s));
		code.push_back("");

		if (parameter::get_is_optional(pi))
		{
			code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": ����� ������������ �������� \" + ",
				get_name_name_text(pi), " + \". ����������� �������� ", get_optional_name_text(pi), ": true\");"));
			code.push_back(string_helper::make_string("\t\t", "value.", get_optional_name_text(pi), " = true;"));
		}
		if (parameter::get_item_type_cpp(file_info_, pi) == "std::string")
		{
			code.push_back(string_helper::make_string("\t\t", "if (", get_value_name_text(pi), " == \"\")"));
			code.push_back(string_helper::make_string("\t\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
				get_name_name_text(pi), " + \": \\\"\\\"\"", ");"));
			code.push_back(string_helper::make_string("\t\t", "else"));
			code.push_back(string_helper::make_string("\t\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
				get_name_name_text(pi), " + \": \" + ", get_value_name_text(pi), ");"));
		}
		else
			code.push_back(string_helper::make_string("\t\t", INFO_LOG_PREFIX, "\": ����� �������� \" + ",
				get_name_name_text(pi), " + \": \" + std::to_string(", get_value_name_text(pi), "));"));
		code.push_back(string_helper::make_string("\t", "}"));

		// �������� ���������, ���� ��� ����������, �����������
		if (parameter::get_category(file_info_, pi) == type_category::user_cpp)
		{
			type_info* ti = type::get_type_info(file_info_, parameter::get_item_type(pi));
			if (ti == nullptr)
				ELRF("Unknown cpp type" << parameter::get_item_type(pi));

			code.push_back(string_helper::make_string("\t", "{"));
			code.push_back(string_helper::make_string("\t\t", parameter::get_item_type(pi), " temp_value{};"));
			code.push_back(string_helper::make_string("\t\t", "if (!", get_convert_function_name_text(*ti), "(base, ",
				get_value_name_text(pi), ", temp_value))"));
			code.push_back(string_helper::make_string("\t\t", "{"));
			code.push_back(string_helper::make_string("\t\t\t", ERROR_LOG_PREFIX, "\": �������� �� ������������� \" + ",
				get_name_name_text(pi), ");"));
			code.push_back(string_helper::make_string("\t\t\t", "return false;"));
			code.push_back(string_helper::make_string("\t\t", "}"));
			code.push_back(string_helper::make_string("\t\t", "value.", get_name_text(pi), " = temp_value;"));
			code.push_back(string_helper::make_string("\t", "}"));
		}
		else if (parameter::get_item_type_class(file_info_, pi) != parameter::get_item_type_cpp(file_info_, pi))
			code.push_back(string_helper::make_string("\t", "value.", get_name_text(pi), " = static_cast<",
				parameter::get_item_type_class(file_info_, pi), ">(", get_value_name_text(pi), ");"));
		else
			code.push_back(string_helper::make_string("\t", "value.", get_name_text(pi), " = ", get_value_name_text(pi), ";"));
		code.push_back(string_helper::make_string(""));
	}
	return true;
}

bool code_formatter::get_restrictions_code(const parameter_info& pi, const bool for_array, std::list<std::string>& code)
{
	if (parameter::get_has_restrictions(pi))
	{
		bool first_string = true;
		if (for_array && parameter::get_is_array(pi))
		{
			if (parameter::get_is_required(pi))
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "if (", get_count_value_name_text(pi), " == 0)"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": ������������ �������� \" + ",
					get_name_name_text(pi), " + \" ����\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}

			if (pi.restrictions.min_count != "")
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "if (", get_count_value_name_text(pi), " < ", pi.restrictions.min_count, ")"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": ���������� ��������� \" + ",
					get_name_name_text(pi), " + \" ������ �����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}

			if (pi.restrictions.max_count != "")
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "if (", get_count_value_name_text(pi), " > ", pi.restrictions.max_count, ")"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": ���������� ��������� \" + ",
					get_name_name_text(pi), " + \" ������ �����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}

			if (pi.restrictions.set_count.size() > 0)
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "std::set<int> ", get_count_value_set_name_text(pi), ";"));
				for (const auto& v : pi.restrictions.set_count)
					code.push_back(string_helper::make_string("", get_count_value_set_name_text(pi), ".insert(", v, ");"));
				code.push_back(string_helper::make_string("", "if (", get_count_value_set_name_text(pi), ".count(",
					get_count_value_name_text(pi), ") == 0)"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": ���������� ��������� \" + ",
					get_name_name_text(pi), " + \" ����������� � ������ ����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}
		}
		else
		{
			if (pi.restrictions.min != "")
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "if (", get_value_name_text(pi), " < ", pi.restrictions.min, ")"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": �������� �������� \" + ",
					get_name_name_text(pi), " + \" ������ �����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}

			if (pi.restrictions.max != "")
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "if (", get_value_name_text(pi), " > ", pi.restrictions.max, ")"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": �������� �������� \" + ",
					get_name_name_text(pi), " + \" ������ �����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}

			if (pi.restrictions.set_.size() > 0)
			{
				if (first_string) first_string = false; else code.push_back(string_helper::make_string(""));
				code.push_back(string_helper::make_string("", "std::set<", parameter::get_item_type_cpp(file_info_, pi), "> ",
					get_value_set_name_text(pi), ";"));
				for (const auto& v : pi.restrictions.set_)
				{
					if (parameter::get_item_type_cpp(file_info_, pi) == "std::string")
						code.push_back(string_helper::make_string("", get_value_set_name_text(pi), ".insert(\"", common::get_as_cp1251(v), "\");"));
					else
						code.push_back(string_helper::make_string("", get_value_set_name_text(pi), ".insert(static_cast<",
							parameter::get_item_type_cpp(file_info_, pi), ">(", common::get_as_cp1251(v), "));"));
				}
				code.push_back(string_helper::make_string("", "if (", get_value_set_name_text(pi), ".count(",
					get_value_name_text(pi), ") == 0)"));
				code.push_back(string_helper::make_string("", "{"));
				code.push_back(string_helper::make_string("\t", ERROR_LOG_PREFIX, "\": �������� �������� \" + ",
					get_name_name_text(pi), " + \" ����������� � ������ ����������\");"));
				code.push_back(string_helper::make_string("\t", "return false;"));
				code.push_back(string_helper::make_string("", "}"));
			}
		}
	}
	return true;
}

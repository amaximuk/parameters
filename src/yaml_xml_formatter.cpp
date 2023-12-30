#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <regex>
#include "git_commit_hash.h"
#include "string_helper.h"
#include "yaml_helper.h"
#include "yaml_xml_formatter.h"

using namespace yaml;
using namespace yaml::helper;

#define ELRF(message) do { if (!is_batch_) std::cout << message << std::endl; return false; } while(0)

xml_formatter::xml_formatter(bool is_batch)
{
	is_batch_ = is_batch;
}

bool xml_formatter::format(const file_info& file_info, bool is_batch, std::list<std::string>& xml)
{
	xml_formatter formatter(is_batch);
	return formatter.format(file_info, xml);
}

bool xml_formatter::format(const file_info& file_info, std::list<std::string>& xml)
{
	file_info_ = file_info;
	if (!get_file_xml(file_info, xml))
		ELRF("Get file html failed");
	return true;
}

bool xml_formatter::get_file_xml(const file_info& fi, std::list<std::string>& xml)
{
	xml.push_back(string_helper::make_string("<Unit Name=\"", common::get_as_cp1251(file_info_.info.hint), "\" Id=\"",
		common::get_as_cp1251(file_info_.info.id), "\">"));

	std::list<std::string> parameters_xml;
	if (!get_parameters_xml(file_info_.parameters, parameters_xml))
		ELRF("Get parameters xml failed");
	for (const auto& s : parameters_xml)
		xml.push_back(string_helper::make_string("\t", s));

	xml.push_back("</Unit>");
	return true;
}

bool xml_formatter::get_parameters_xml(const std::vector<parameter_info>& parameters, std::list<std::string>& xml)
{
	for (const auto& pi : parameters)
	{
		std::list<std::string> parameter_xml;
		if (!get_parameter_xml(pi, parameter_xml))
			ELRF("Get parameters xml failed");

		for (const auto& s : parameter_xml)
			xml.push_back(s);
	}
	return true;
}

bool xml_formatter::get_parameter_xml(const parameter_info& pi, std::list<std::string>& xml)
{
	if (parameter::get_is_array(pi))
	{
		int count = get_restricted_count_xml(pi);

		if (parameter::get_category(file_info_, pi) == type_category::user_yml)
		{
			// Ёто вложенный тип данных, вызываем get_parameters_xml реккурсивно
			xml.push_back(string_helper::make_string("<Array name=\"", common::get_as_cp1251(pi.name), "\">"));

			type_info* ti = type::get_type_info(file_info_, parameter::get_item_type(pi));
			if (ti == nullptr)
				ELRF("Get type failed, name = " << parameter::get_item_type(pi));

			std::list<std::string> parameters_xml;
			if (!get_parameters_xml(ti->parameters, parameters_xml))
				ELRF("Get parameters xml failed");

			for (int i = 0; i < count; i++)
			{
				xml.push_back("\t<Item>");
				for (const auto& s : parameters_xml)
					xml.push_back(string_helper::make_string("\t\t", s));
				xml.push_back("\t</Item>");
			}

			xml.push_back("</Array>");
		}
		else
		{
			// Ёто типизированный массив. “ип данных может быть стандартными или пользовательским типа cpp
			xml.push_back(string_helper::make_string("<Array name=\"", common::get_as_cp1251(pi.name), "\" type=\"",
				parameter::get_item_type_xml(file_info_, pi), "\">"));

			for (int i = 0; i < count; i++)
				xml.push_back(string_helper::make_string("\t<Item val=\"", get_restricted_value_xml(pi), "\"/>"));

			xml.push_back("</Array>");
		}
	}
	else
	{
		if (parameter::get_item_type(pi) == "unit")
			xml.push_back(string_helper::make_string("<Param name=\"", common::get_as_cp1251(pi.name), "\" type=\"",
				parameter::get_item_type_xml(file_info_, pi), "\" val=\"", get_restricted_value_xml(pi), "\" depends=\"true\"/>"));
		else
			xml.push_back(string_helper::make_string("<Param name=\"", common::get_as_cp1251(pi.name), "\" type=\"",
				parameter::get_item_type_xml(file_info_, pi), "\" val=\"", get_restricted_value_xml(pi), "\"/>"));
	}

	return true;
}












int xml_formatter::get_restricted_count_xml(const parameter_info& pi) const
{
	if (!pi.restrictions.min_count.empty())
	{
		int min_count = atoi(pi.restrictions.min_count.c_str());
		if (min_count == 0 && !pi.restrictions.min_count.empty())
		{
			int max_count = atoi(pi.restrictions.max_count.c_str());
			if (max_count > 0)
				return 1;
			else
				return 0;
		}
		else
			return min_count;
	}

	if (!pi.restrictions.max_count.empty())
	{
		int max_count = atoi(pi.restrictions.max_count.c_str());
		if (max_count > 0)
			return 1;
		else
			return 0;
	}

	if (!pi.restrictions.set_count.empty())
	{
		int min_set_count = std::numeric_limits<int>::max();
		for (const auto& s : pi.restrictions.set_count)
		{
			int set_count = atoi(s.c_str());
			if (set_count != 0 && min_set_count < set_count)
				min_set_count = set_count;
		}
		if (min_set_count != std::numeric_limits<int>::max())
			return min_set_count;
		else
			return 0;
	}

	return 0;
}

std::string xml_formatter::get_restricted_value_xml(const parameter_info& pi) const
{
	if (!pi.hint.empty())
		return common::get_as_cp1251(pi.hint);
	if (!pi.default_.empty())
		return common::get_as_cp1251(pi.default_);
	if (!pi.restrictions.min.empty())
		return common::get_as_cp1251(pi.restrictions.min);
	if (!pi.restrictions.max.empty())
		return common::get_as_cp1251(pi.restrictions.max);
	if (!pi.restrictions.set_.empty())
		return common::get_as_cp1251(pi.restrictions.set_[0]);
	const auto item_type_xml = parameter::get_item_type_xml(file_info_, pi);
	if (item_type_xml == "dbl")
		return "0.0";
	if (item_type_xml == "int")
		return "0";
	if (item_type_xml == "bool")
		return "false";
	if (item_type_xml == "str")
		return "Some value";
	return "";
}

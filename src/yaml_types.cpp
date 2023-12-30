#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <regex>
#include <limits>
#include "encoding_cp1251.h"
#include "string_helper.h"
#include "yaml_helper.h"
#include "yaml_types.h"

#undef min
#undef max
/*
namespace yaml
{
	// parameter_info
	std::string parameter_info::get_description_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.description);
	}

	std::string parameter_info::get_display_name_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.display_name);
	}

	std::string parameter_info::get_default_cp1251() const
	{
		if (yml.default_ != "")
			return EncodingCP1251::utf8_to_cp1251(yml.default_);
		return "";
	}

	std::string parameter_info::get_default_wiki() const
	{
		return get_default_cp1251();
	}

	std::string parameter_info::get_description_wiki() const
	{
		std::string description_wiki = helper::get_description(get_description_cp1251(), helper::description_type::wiki);
		return description_wiki;
	}

	std::string parameter_info::get_description_html() const
	{
		return helper::get_description(get_description_cp1251(), helper::description_type::html);
	}

	std::string parameter_info::get_hint_cp1251() const
	{
		if (yml.hint != "")
			return EncodingCP1251::utf8_to_cp1251(yml.hint);
		return "";
	}

	std::string parameter_info::get_type_xml_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(type_xml);
	}

	std::string parameter_info::get_type_html() const
	{
		std::string type_xml_html = yml.type;
		type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
		type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
		return type_xml_html;
	}

	std::string parameter_info::get_type_xml_html() const
	{
		std::string type_xml_html = get_type_xml_cp1251();
		type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
		type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
		return type_xml_html;
	}

	std::string parameter_info::get_type_wiki() const
	{
		return get_type_html();
	}

	std::string parameter_info::get_type_xml_wiki() const
	{
		return get_type_xml_html();
	}

	std::string parameter_info::get_is_required_cp1251() const
	{
		return is_required ? "да" : "нет";
	}

	std::string parameter_info::get_is_required_wiki() const
	{
		return get_is_required_cp1251();
	}

	int parameter_info::get_restricted_count_xml() const
	{
		if (has_restrictions && yml.restrictions.min_count != "")
		{
			int min_count = atoi(yml.restrictions.min_count.c_str());
			if (min_count == 0 && yml.restrictions.max_count != "")
			{
				int max_count = atoi(yml.restrictions.max_count.c_str());
				if (max_count > 0)
					return 1;
				else
					return 0;
			}
			else
				return min_count;
		}
		if (has_restrictions && yml.restrictions.max_count != "")
		{
			int max_count = atoi(yml.restrictions.max_count.c_str());
			if (max_count > 0)
				return 1;
			else
				return 0;
		}
		if (has_restrictions && yml.restrictions.set_count.size() > 0)
		{
			int min_set_count = std::numeric_limits<int>::max();
			for (const auto& s : yml.restrictions.set_count)
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

	std::string parameter_info::get_restricted_value_xml() const
	{
		if (yml.hint != "")
			return get_hint_cp1251();
		if (yml.default_ != "")
			return get_default_cp1251();
		if (has_restrictions && yml.restrictions.min != "")
			return yml.restrictions.min;
		if (has_restrictions && yml.restrictions.max != "")
			return yml.restrictions.max;
		if (has_restrictions && yml.restrictions.set_.size() > 0)
			return EncodingCP1251::utf8_to_cp1251(yml.restrictions.set_[0]);
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

	std::vector<std::string> parameter_info::get_restrictions_set_cp1251() const
	{
		std::vector<std::string> set_cp1251;
		for (const auto& s : yml.restrictions.set_)
			set_cp1251.push_back(EncodingCP1251::utf8_to_cp1251(s));
		return set_cp1251;
	}

	bool parameter_info::is_user_type() const
	{
		return (category == type_category::user_cpp) || (category == type_category::user_yml);
	}

	std::string parameter_info::get_name_text() const
	{
		std::string name_text = yml.name;
		std::transform(name_text.begin(), name_text.end(), name_text.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return name_text;
	}

	std::string parameter_info::get_optional_name_text() const
	{
		return get_name_text() + "_is_set";
	}

	std::string parameter_info::get_name_name_text() const
	{
		return get_name_text() + "_parameter_name";
	}

	std::string parameter_info::get_value_name_text() const
	{
		return get_name_text() + "_parameter_value";
	}

	std::string parameter_info::get_count_name_name_text() const
	{
		return get_name_text() + "_parameters_count_name";
	}

	std::string parameter_info::get_count_value_name_text() const
	{
		return get_name_text() + "_parameter_count_value";
	}

	std::string parameter_info::get_count_xml_name_text() const
	{
		return yml.name + "__CNT";
	}

	std::string parameter_info::get_value_set_name_text() const
	{
		return get_value_name_text() + "_set";
	}

	std::string parameter_info::get_count_value_set_name_text() const
	{
		return get_count_value_name_text() + "_set";
	}

	// type_info
	std::string type_info::get_description_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.description);
	}

	std::string type_info::get_description_wiki() const
	{
		std::string description_wiki = helper::get_description(get_description_cp1251(), helper::description_type::wiki);
		return description_wiki;
	}

	std::string type_info::get_description_html() const
	{
		return helper::get_description(get_description_cp1251(), helper::description_type::html);
	}

	bool type_info::is_user_yml() const
	{
		return (category == type_category::user_yml);
	}

	bool type_info::is_user_cpp() const
	{
		return (category == type_category::user_cpp);
	}

	std::string type_info::get_convert_function_name_text() const
	{
		std::string function_name = yml.name;
		std::transform(function_name.begin(), function_name.end(), function_name.begin(),
			[](unsigned char c) { return std::tolower(c); });
		function_name = std::regex_replace(function_name, std::regex("::"), "_");
		
		return "convert_" + function_name;
	}

	// info_info
	std::string info_info::get_display_name_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.display_name);
	}

	std::string info_info::get_description_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.description);
	}

	std::string info_info::get_description_wiki() const
	{
		std::string description_wiki = helper::get_description(get_description_cp1251(), helper::description_type::wiki);
		return description_wiki;
	}

	std::string info_info::get_description_html() const
	{
		std::string description_wiki = helper::get_description(get_description_cp1251(), helper::description_type::html);
		return description_wiki;
	}

	std::string info_info::get_description() const
	{
		return helper::get_description(get_description_cp1251(), helper::description_type::html);
	}

	std::string info_info::get_description_html_variables() const
	{
		return helper::get_description(get_description_cp1251(), helper::description_type::html);
	}

	std::string info_info::get_author_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.author);
	}

	std::string info_info::get_wiki_cp1251() const
	{
		return EncodingCP1251::utf8_to_cp1251(yml.wiki);
	}


	// file_info
	bool file_info::get_type(const std::string& name, type_info& type) const
	{
		for (const auto& ti : types)
		{
			if (ti.yml.name == name)
			{
				type = ti;
				return true;
			}
		}
		return false;
	}
}
*/

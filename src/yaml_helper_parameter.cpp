#pragma once

#include "definitions.h"
#include "yaml_types.h"
#include "yaml_helper_common.h"
#include "yaml_helper_type.h"
#include "yaml_helper_parameter.h"

namespace yaml
{
	namespace helper
	{
		type_category parameter::get_category(const file_info& fi, const parameter_info& pi)
		{
			type_category category{};
			const std::string item_type = get_item_type(pi);
			const auto it_system_type = system_to_xml_types.find(item_type);
			if (it_system_type != system_to_xml_types.cend())
				category = type_category::system;
			else
			{
				const auto it_cpp_type = cpp_to_xml_types.find(item_type);
				if (it_cpp_type != cpp_to_xml_types.cend())
					category = type_category::cpp;
				else
				{
					// Если тип пользовательский, то он должен быть в списке типов
					const auto pti = type::get_type_info(fi, item_type);
					if (pti != nullptr)
					{
						// Если тип - это описание параметров, то это yml тип, берем его имя
						// Если тип это псевдоним для типа из исходников - это тип cpp, для него должен быть задан TYPE_XML
						if (pti->type == "yml")
							category = type_category::user_yml;
						else
							category = type_category::user_cpp;
					}
				}
			}
			return category;
		}

		std::string parameter::get_type_xml(const file_info& fi, const parameter_info& pi)
		{
			std::string type_xml{};
			const auto item_type_xml = get_item_type_xml(fi, pi);
			if (get_is_array(pi))
				type_xml = std::string("array<") + item_type_xml + ">";
			else
				type_xml = item_type_xml;
			return type_xml;
		}

		std::string parameter::get_type_class(const file_info& fi, const parameter_info& pi)
		{
			std::string type_class{};
			const auto item_type_class = get_item_type_class(fi, pi);
			if (get_is_array(pi))
				type_class = std::string("std::vector<") + item_type_class + ">";
			else
				type_class = item_type_class;
			return type_class;
		}

		std::string parameter::get_item_type(const parameter_info& pi)
		{
			std::string item_type{ pi.type };
			if (pi.type.length() > std::string("array<>").length() &&
				pi.type.substr(0, std::string("array<").length()) == "array<" &&
				pi.type[pi.type.length() - 1] == '>')
				item_type = pi.type.substr(std::string("array<").length(), pi.type.length() - std::string("array<>").length());
			return item_type;
		}

		std::string parameter::get_item_type_xml(const file_info& fi, const parameter_info& pi)
		{
			std::string item_type_xml{};
			const std::string item_type = get_item_type(pi);
			const auto it_system_type = system_to_xml_types.find(item_type);
			if (it_system_type != system_to_xml_types.cend())
				item_type_xml = it_system_type->second;
			else
			{
				const auto it_cpp_type = cpp_to_xml_types.find(item_type);
				if (it_cpp_type != cpp_to_xml_types.cend())
					item_type_xml = it_cpp_type->second;
				else
				{
					// Если тип пользовательский, то он должен быть в списке типов
					const auto pti = type::get_type_info(fi, item_type);
					if (pti != nullptr)
					{
						// Если тип - это описание параметров, то это yml тип, берем его имя
						// Если тип это псевдоним для типа из исходников - это тип cpp, для него должен быть задан TYPE_XML
						const auto category = type::get_category(*pti);
						if (category == type_category::user_cpp)
						{
							const auto type_xml = type::get_type_xml(*pti);
							item_type_xml = type_xml;
						}
						else
							item_type_xml = pti->name;
					}
				}
			}
			return item_type_xml;
		}

		std::string parameter::get_item_type_cpp(const file_info& fi, const parameter_info& pi)
		{
			std::string item_type_get{};
			std::string item_type_xml = get_item_type_xml(fi, pi);
			const auto it_xml_type = xml_to_cpp_types.find(item_type_xml);
			if (it_xml_type != xml_to_cpp_types.cend())
			{
				// Если это не пользовательский yml тип, берем из карты xml_type_names_ по имени параметра для xml
				item_type_get = it_xml_type->second;
			}
			return item_type_get;
		}

		std::string parameter::get_item_type_class(const file_info& fi, const parameter_info& pi)
		{
			std::string item_type_class{};
			const std::string item_type = get_item_type(pi);

			const auto it_system_type = system_to_xml_types.find(item_type);
			if (it_system_type != system_to_xml_types.cend())
			{
				// Если тип системный, берем из карты xml_type_names_ по имени параметра для xml
				// т.к. системные типы (unit, path) не известны в cpp и должны замениться на cpp-шные
				item_type_class = xml_to_cpp_types.at(it_system_type->second);
			}
			else
			{
				const auto cpp_type = cpp_to_xml_types.find(item_type);
				if (cpp_type != cpp_to_xml_types.cend())
				{
					// Если тип стандартный, берем его как есть
					item_type_class = item_type;
				}
				else
				{
					// Если тип пользовательский, то он должен быть в списке типов
					const auto pti = type::get_type_info(fi, item_type);
					if (pti != nullptr)
					{
						// Если это yml тип, то параметр имеет смысл только для массивов,
						// т.к. в обычных параметрах yml типы не применимы
						// Если это тип cpp, то параметр может применяться без массива
						item_type_class = pti->name;
					}
				}
			}
			return item_type_class;
		}

		bool parameter::get_is_array(const parameter_info& pi)
		{
			bool is_array = false;
			if (pi.type.length() > std::string("array<>").length() &&
				pi.type.substr(0, std::string("array<").length()) == "array<" &&
				pi.type[pi.type.length() - 1] == '>')
				is_array = true;
			return is_array;
		}

		bool parameter::get_is_required(const parameter_info& pi)
		{
			bool is_required = false;
			if (pi.required == "true")
				is_required = true;
			return is_required;
		}

		bool parameter::get_is_optional(const parameter_info& pi)
		{
			bool is_optional = false;
			bool is_required = get_is_required(pi);
			if (!is_required && pi.default_.empty())
				is_optional = true;
			return is_optional;
		}

		bool parameter::get_has_restrictions(const parameter_info& pi)
		{
			bool has_restrictions = false;
			const auto& ri = pi.restrictions;
			if (!ri.min.empty() || !ri.max.empty() || ri.set_.size() > 0 ||
				!ri.min_count.empty() || !ri.max_count.empty() || ri.set_count.size() > 0 ||
				!ri.category.empty() || ri.ids.size() > 0 || !ri.max_length.empty())
				has_restrictions = true;
			return has_restrictions;
		}

		std::string parameter::get_display_name(const parameter_info& pi)
		{
			if (pi.display_name != "")
				return pi.display_name;
			else
				return pi.name;
		}

		variant parameter::get_initial_value(const file_info& fi, const parameter_info& pi, bool is_item)
		{
			base_types bt = common::get_base_type(pi.type);
			if (bt == base_types::array && is_item)
				bt = common::get_base_type(common::get_item_type(pi.type));

			if (bt == base_types::array)
			{
				int value = 0;
				if (pi.restrictions.set_count.size() > 0)
					value = std::stoi(pi.restrictions.set_count[0]);
				else if (pi.restrictions.min_count != "")
					value = std::stoi(pi.restrictions.min_count);
				else
					value = 0;
				return variant(value);
			}
			else
			{
				std::string hint;

				if (pi.required == "true")
					hint = pi.hint;
				else if (pi.default_ != "")
					hint = pi.default_;
				else
					hint = pi.hint;

				if (hint == "" && (bt == base_types::enum_ || bt == base_types::integer || bt == base_types::floating))
				{
					// try get initial from restrictions
					if (pi.restrictions.set_.size() > 0)
						hint = pi.restrictions.set_[0];
					else if (pi.restrictions.min != "" && pi.restrictions.max != "")
					{
						if (bt == base_types::enum_)
							hint = pi.restrictions.min;
						else if (bt == base_types::integer)
						{
							if (std::stoi(pi.restrictions.min) <= 0 && std::stoi(pi.restrictions.max) >= 0)
								hint = "0";
							else
							{
								int abs_min = std::abs(std::stoi(pi.restrictions.min));
								int abs_max = std::abs(std::stoi(pi.restrictions.max));
								hint = abs_min <= abs_max ? pi.restrictions.min : pi.restrictions.max;
							}
						}
						else if (bt == base_types::floating)
						{
							if (std::stod(pi.restrictions.min) <= 0 && std::stod(pi.restrictions.max) >= 0)
								hint = "0";
							else
							{
								double abs_min = std::abs(std::stod(pi.restrictions.min));
								double abs_max = std::abs(std::stod(pi.restrictions.max));
								hint = abs_min <= abs_max ? pi.restrictions.min : pi.restrictions.max;
							}
						}
					}
					else if (pi.restrictions.min != "")
					{
						if (bt == base_types::enum_)
							hint = pi.restrictions.min;
						else if (bt == base_types::integer)
						{
							if (std::stoi(pi.restrictions.min) <= 0)
								hint = "0";
							else
								hint = pi.restrictions.min;
						}
						else if (bt == base_types::floating)
						{
							if (std::stod(pi.restrictions.min) <= 0)
								hint = "0";
							else
								hint = pi.restrictions.min;
						}
					}
					else if (pi.restrictions.max != "")
					{
						if (bt == base_types::enum_)
							hint = pi.restrictions.min;
						else if (bt == base_types::integer)
						{
							if (std::stod(pi.restrictions.max) >= 0)
								hint = "0";
							else
								hint = pi.restrictions.max;
						}
						else if (bt == base_types::floating)
						{
							if (std::stod(pi.restrictions.max) >= 0)
								hint = "0";
							else
								hint = pi.restrictions.max;
						}
					}
				}

				if (bt == base_types::string)
				{
					std::string value = hint;
					return variant(value);
				}
				else if (bt == base_types::bool_)
				{
					bool value = false;
					if (hint != "")
						value = hint == "true" ? true : false;
					else
						value = false;
					return variant(value);
				}
				else if (bt == base_types::enum_)
				{
					int value = 0;
					// get index of hint value or 0 (index of first enum element)
					const yaml::type_info* ti = type::get_type_info(fi, pi.type);
					if (ti != nullptr && ti->type == "enum" && ti->values.size() > 0)
					{
						if (hint == "")
							value = 0;
						else
						{
							value = 0;

							//auto it = std::find_if(ti->values.cbegin(), ti->values.cend(), [hint](const auto& v) { return v.first == hint; });
							//if (it != ti->values.cend())
							//    value = static_cast<int>(std::distance(ti->values.cbegin(), it));
							//else
							//    value = 0;
						}
					}
					else
						value = 0;
					return variant(value);
				}
				else if (bt == base_types::integer)
				{
					int value = 0;
					if (hint != "")
						value = std::stoi(hint);
					else
						value = 0;
					return variant(value);
				}
				else if (bt == base_types::floating)
				{
					double value = 0.0;
					if (hint != "")
						value = std::stod(hint);
					else
						value = 0.0;
					return variant(value);
				}
			}

			return {};
		}

		std::vector<parameter_info>* parameter::get_parameters(file_info& fi, const std::string& type)
		{
			std::vector<parameter_info>* pvect = nullptr;
			if (type == type::main_type)
			{
				pvect = &fi.parameters;
			}
			else
			{
				auto pti = type::get_type_info(fi, type);
				if (pti) pvect = &pti->parameters;
			}
			return pvect;
		}

		parameter_info* parameter::get_parameter_info(file_info& fi, const std::string& type, const std::string& name)
		{
			parameter_info* ppi = nullptr;
			auto pvect = get_parameters(fi, type);
			if (pvect)
			{
				auto it = std::find_if(pvect->begin(), pvect->end(), [name](const auto& pi) { return pi.name == name; });
				if (it != pvect->end())
					ppi = &(*it);
			}
			return ppi;
		}
	}
}

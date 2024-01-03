#pragma once

#include "string_helper.h"
#include "definitions.h"
#include "parameters/types.h"
#include "parameters.h"

//#define ELRF(message) do { if (!is_batch_) std::cout << message << std::endl; return false; } while(0)

namespace parameters
{
	namespace helper
	{
		type_category type::get_category(const type_info& ti)
		{
			type_category category = ti.type == "yml" ? type_category::user_yml : type_category::user_cpp;
			return category;
		}

		std::string type::get_type_xml(const type_info& ti)
		{
			std::string type_xml{};
			const auto it = type_to_xml_types.find(ti.type);
			if (it != type_to_xml_types.cend())
				type_xml = it->second;
			return type_xml;
		}

		std::string type::get_type_cpp(const type_info& ti)
		{
			std::string type_get{};
			std::string type_xml = get_type_xml(ti);
			const auto it = xml_to_cpp_types.find(type_xml);
			if (it != xml_to_cpp_types.cend())
				type_get = it->second;
			return type_get;
		}

		bool type::set_parameter(file_info& fi, const std::string& type, const parameter_info& pi)
		{
			auto ppi = parameter::get_parameter_info(fi, type, pi.name);
			if (!ppi) return false;
			*ppi = pi;
			return true;
		}

		bool type::have_parameter(file_info& fi, const std::string& type, const std::string& name)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (ppi) return true;
			return false;
		}

		bool type::add_parameter(file_info& fi, const std::string& type, const parameter_info& pi)
		{
			auto ppi = parameter::get_parameter_info(fi, type, pi.name);
			if (ppi) return false;
			auto pvect = parameter::get_parameters(fi, type);
			if (!pvect) return false;
			pvect->push_back(pi);
			return true;
		}

		bool type::remove_parameter(file_info& fi, const std::string& type, const std::string& name)
		{
			auto pvect = parameter::get_parameters(fi, type);
			if (!pvect) return false;
			pvect->erase(std::remove_if(pvect->begin(), pvect->end(), [name](const auto& v) { return v.name == name; }), pvect->end());
			return true;
		}

		bool type::move_parameter(file_info& fi, const std::string& type, const std::string& name, const bool up)
		{
			auto pvect = parameter::get_parameters(fi, type);
			if (!pvect) return false;
			auto it = std::find_if(pvect->begin(), pvect->end(), [name](auto& p) { return p.name == name; });
			if (up && it != pvect->begin() && it != pvect->end())
				std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(pvect->end(), -1) && it != pvect->end())
				std::iter_swap(it, std::next(it, 1));
			return true;
		}

		bool type::rename_parameter(file_info& fi, const std::string& type, const std::string& oldName, const std::string& newName)
		{
			auto ppi = parameter::get_parameter_info(fi, type, oldName);
			if (!ppi) return false;
			ppi->name = newName;
			return true;
		}

		bool type::have_value(file_info& fi, const std::string& type, const std::string& name)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			return false;
		}

		bool type::add_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			if (have_value(fi, type, name))
				return false;
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			ti->values.push_back(std::make_pair(name, value));
			return true;
		}

		bool type::remove_value(file_info& fi, const std::string& type, const std::string& name)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			ti->values.erase(std::remove_if(ti->values.begin(), ti->values.end(), [name](const auto& v)
				{ return v.first == name; }), ti->values.end());
			return true;
		}

		bool type::move_value(file_info& fi, const std::string& type, const std::string& name, const bool up)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			auto it = std::find_if(ti->values.begin(), ti->values.end(), [name](const auto& v) { return v.first == name; });
			if (up && it != ti->values.begin() && it != ti->values.end())
			    std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(ti->values.end(), -1) && it != ti->values.end())
			    std::iter_swap(it, std::next(it, 1));
			return true;
		}

		bool type::have_include(file_info& fi, const std::string& type, const std::string& name)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			if (std::find_if(ti->includes.cbegin(), ti->includes.cend(), [name](const auto& v) {return v == name; }) != ti->includes.cend())
				return true;
			else
				return false;
		}

		bool type::add_include(file_info& fi, const std::string& type, const std::string& name)
		{
			if (have_include(fi, type, name))
				return false;
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			ti->includes.push_back(name);
			return true;
		}

		bool type::remove_include(file_info& fi, const std::string& type, const std::string& name)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			ti->includes.erase(std::remove_if(ti->includes.begin(), ti->includes.end(), [name](const auto& v) { return v == name; }), ti->includes.end());
			return true;
		}

		bool type::move_include(file_info& fi, const std::string& type, const std::string& name, const bool up)
		{
			auto ti = type::get_type_info(fi, type);
			if (!ti) return false;
			auto it = std::find_if(ti->includes.begin(), ti->includes.end(), [name](const auto& v) { return v == name; });
			if (up && it != ti->includes.begin() && it != ti->includes.end())
				std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(ti->includes.end(), -1) && it != ti->includes.end())
				std::iter_swap(it, std::next(it, 1));
			return true;
		}

		type_info* type::get_type_info(file_info& fi, const std::string& name)
		{
			const auto it = std::find_if(fi.types.cbegin(), fi.types.cend(),
				[&name](const type_info& ti) {return name == ti.name; });
			if (it != fi.types.cend())
				return &fi.types[std::distance(fi.types.cbegin(), it)];
			return nullptr;
		}

		const type_info* type::get_type_info(const file_info& fi, const std::string& name)
		{
			const auto it = std::find_if(fi.types.cbegin(), fi.types.cend(),
				[&name](const type_info& ti) {return name == ti.name; });
			if (it != fi.types.cend())
				return &fi.types[std::distance(fi.types.cbegin(), it)];
			return nullptr;
		}
	}
}

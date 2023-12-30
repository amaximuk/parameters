#pragma once

#include "definitions.h"
#include "yaml_types.h"
#include "yaml_helper_parameter.h"
#include "yaml_helper_restrictions.h"

namespace yaml
{
	namespace helper
	{
		bool restrictions::have_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			if (std::find(ppi->restrictions.set_.cbegin(), ppi->restrictions.set_.cend(), value) != ppi->restrictions.set_.cend())
				return true;
			else
				return false;
		}

		bool restrictions::add_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			if (have_set_value(fi, type, name, value))
				return false;
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.set_.push_back(value);
			return true;
		}

		bool restrictions::remove_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.set_.erase(std::remove_if(ppi->restrictions.set_.begin(), ppi->restrictions.set_.end(), [value](const auto& v) { return v == value; }), ppi->restrictions.set_.end());
			return true;
		}

		bool restrictions::move_set_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			auto it = std::find_if(ppi->restrictions.set_.begin(), ppi->restrictions.set_.end(), [value](const auto& v) { return v == value; });
			if (up && it != ppi->restrictions.set_.begin() && it != ppi->restrictions.set_.end())
				std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(ppi->restrictions.set_.end(), -1) && it != ppi->restrictions.set_.end())
				std::iter_swap(it, std::next(it, 1));
			return true;
		}

		bool restrictions::have_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			if (std::find(ppi->restrictions.set_count.cbegin(), ppi->restrictions.set_count.cend(), value) != ppi->restrictions.set_count.cend())
				return true;
			else
				return false;
		}

		bool restrictions::add_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			if (have_set_count_value(fi, type, name, value))
				return false;
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.set_count.push_back(value);
			return true;
		}

		bool restrictions::remove_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.set_count.erase(std::remove_if(ppi->restrictions.set_count.begin(), ppi->restrictions.set_count.end(), [value](const auto& v) { return v == value; }), ppi->restrictions.set_count.end());
			return true;
		}

		bool restrictions::move_set_count_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			auto it = std::find_if(ppi->restrictions.set_count.begin(), ppi->restrictions.set_count.end(), [value](const auto& v) { return v == value; });
			if (up && it != ppi->restrictions.set_count.begin() && it != ppi->restrictions.set_count.end())
				std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(ppi->restrictions.set_count.end(), -1) && it != ppi->restrictions.set_count.end())
				std::iter_swap(it, std::next(it, 1));
			return true;
		}

		bool restrictions::have_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			if (std::find(ppi->restrictions.ids.cbegin(), ppi->restrictions.ids.cend(), value) != ppi->restrictions.ids.cend())
				return true;
			else
				return false;
		}

		bool restrictions::add_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			if (have_ids_value(fi, type, name, value))
				return false;
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.ids.push_back(value);
			return true;
		}

		bool restrictions::remove_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			ppi->restrictions.ids.erase(std::remove_if(ppi->restrictions.ids.begin(), ppi->restrictions.ids.end(), [value](const auto& v) { return v == value; }), ppi->restrictions.ids.end());
			return true;
		}

		bool restrictions::move_ids_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value, const bool up)
		{
			auto ppi = parameter::get_parameter_info(fi, type, name);
			if (!ppi) return false;
			auto it = std::find_if(ppi->restrictions.ids.begin(), ppi->restrictions.ids.end(), [value](const auto& v) { return v == value; });
			if (up && it != ppi->restrictions.ids.begin() && it != ppi->restrictions.ids.end())
				std::iter_swap(it, std::next(it, -1));
			else if (!up && it != std::next(ppi->restrictions.ids.end(), -1) && it != ppi->restrictions.ids.end())
				std::iter_swap(it, std::next(it, 1));
			return true;
		}
	}
}

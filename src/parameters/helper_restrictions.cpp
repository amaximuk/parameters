#pragma once

#include <algorithm>
#include "definitions.h"
#include "parameters/types.h"
#include "parameters/helper_parameter.h"
#include "parameters/helper_restrictions.h"

using namespace parameters;
using namespace parameters::helper;

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

bool restrictions::compare(const restrictions_info& ri1, const restrictions_info& ri2, std::string message)
{
	message = "ok";

	// Optional members from yml
	if (ri1.min != ri2.min)
	{
		message = "���� restrictions_info.min �� ���������";
		return false;
	}

	if (ri1.max != ri2.max)
	{
		message = "���� restrictions_info.max �� ���������";
		return false;
	}

	if (ri1.set_.size() != ri2.set_.size())
	{
		message = "���������� restrictions_info.set_ �� ���������";
		return false;
	}

	for (const auto& s : ri1.set_)
	{
		const auto it = std::find(ri2.set_.cbegin(), ri2.set_.cend(), s);
		if (it == ri2.set_.cend())
		{
			message = "� restrictions_info.set_ �� ������ �������� " + s;
			return false;
		}
	}

	if (ri1.min_count != ri2.min_count)
	{
		message = "���� restrictions_info.min_count �� ���������";
		return false;
	}

	if (ri1.max_count != ri2.max_count)
	{
		message = "���� restrictions_info.max_count �� ���������";
		return false;
	}

	if (ri1.set_count.size() != ri2.set_count.size())
	{
		message = "���������� restrictions_info.set_count �� ���������";
		return false;
	}

	for (const auto& s : ri1.set_count)
	{
		const auto it = std::find(ri2.set_count.cbegin(), ri2.set_count.cend(), s);
		if (it == ri2.set_count.cend())
		{
			message = "� restrictions_info.set_count �� ������ �������� " + s;
			return false;
		}
	}

	if (ri1.category != ri2.category)
	{
		message = "���� restrictions_info.category �� ���������";
		return false;
	}

	if (ri1.ids.size() != ri2.ids.size())
	{
		message = "���������� restrictions_info.ids �� ���������";
		return false;
	}

	for (const auto& s : ri1.ids)
	{
		const auto it = std::find(ri2.ids.cbegin(), ri2.ids.cend(), s);
		if (it == ri2.ids.cend())
		{
			message = "� restrictions_info.ids �� ������ �������� " + s;
			return false;
		}
	}

	if (ri1.max_length != ri2.max_length)
	{
		message = "���� restrictions_info.max_length �� ���������";
		return false;
	}

	return true;
}

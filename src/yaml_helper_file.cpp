#pragma once

#include "definitions.h"
#include "yaml_types.h"
#include "yaml_helper_common.h"
#include "yaml_helper_type.h"
#include "yaml_helper_file.h"

namespace yaml
{
	namespace helper
	{
		int file::get_format(const file_info& fi)
		{
			return atoi(fi.format.c_str());
		}

		std::string file::get_display_name(const file_info& fi)
		{
			if (fi.info.hint != "")
				return fi.info.hint;
			return fi.info.id;
		}

		std::vector<std::string> file::get_type_types()
		{
			return type_types;
		}

		std::vector<std::string> file::get_parameter_types(const file_info& fi)
		{
			std::vector<std::string> result;
			for (const auto& v : fi.types)
			{
				// yml types can be used in array only
				if (v.type == "enum")
					result.push_back(v.name);
				result.push_back("array<" + v.name + ">");
			}
			result.insert(result.end(), parameter_types.cbegin(), parameter_types.cend());
			return result;
		}

		std::vector<std::string> file::get_user_types(const file_info& fi)
		{
			std::vector<std::string> result;
			for (const auto& v : fi.types)
				result.push_back(v.name);
			return result;
		}

		bool file::set_type(file_info& fi, const std::string& type, const yaml::type_info& ti, bool exclude_parameters)
		{
			yaml::type_info* pti = type::get_type_info(fi, type);
			if (!pti) return false;

			if (exclude_parameters)
			{
				yaml::type_info tit(ti);
				tit.parameters = pti->parameters;
				*pti = tit;
			}
			else
			{
				*pti = ti;
			}
			return true;
		}

		bool file::rename_type(file_info& fi, const std::string& oldName, const std::string& newName)
		{
			std::string arrayOldName = "array<" + oldName + ">";
			std::string arrayNewName = "array<" + newName + ">";

			for (auto& t : fi.types)
			{
				if (t.name == oldName)
				{
					t.name = newName;
					break;
				}
			}

			for (auto& p : fi.parameters)
			{
				if (p.type == oldName)
				{
					p.type = newName;
					break;
				}
				if (p.type == arrayOldName)
				{
					p.type = arrayNewName;
					break;
				}
			}
			for (auto& t : fi.types)
			{
				for (auto& p : t.parameters)
				{
					if (p.type == oldName)
					{
						p.type = newName;
						break;
					}
					if (p.type == arrayOldName)
					{
						p.type = arrayNewName;
						break;
					}
				}
			}

			return true;
		}

		bool file::rearrange_types(file_info& fi, bool& have_type_loop)
		{
			std::vector<std::string> sorted_names;
			have_type_loop = false;
			bool found_new = true;
			while (found_new)
			{
				found_new = false;
				for (const auto& ti : fi.types)
				{
					const auto tn = std::find(sorted_names.cbegin(), sorted_names.cend(), ti.name);
					if (tn != sorted_names.cend())
						continue;

					bool have_unresolved = false;
					for (const auto& pi : ti.parameters)
					{
						std::string item_type = helper::common::get_item_type(pi.type);
						const auto ts = std::find(sorted_names.cbegin(), sorted_names.cend(), item_type);

						if (!helper::common::get_is_inner_type(item_type) && ts == sorted_names.cend())
						{
							have_unresolved = true;
							break;
						}
					}

					if (!have_unresolved)
					{
						sorted_names.push_back(ti.name);
						found_new = true;
					}
				}
			}

			if (fi.types.size() > sorted_names.size())
			{
				// Type loop found
				have_type_loop = true;
				for (const auto& ti : fi.types)
				{
					const auto tn = std::find(sorted_names.cbegin(), sorted_names.cend(), ti.name);
					if (tn == sorted_names.cend())
						sorted_names.push_back(ti.name);
				}
			}

			std::vector<yaml::type_info> sorted_types;
			for (const auto& sn : sorted_names)
			{
				for (const auto& ti : fi.types)
				{
					if (sn == ti.name)
					{
						sorted_types.push_back(ti);
					}
				}
			}

			fi.types = std::move(sorted_types);

			return true;
		}

		bool file::validate(file_info& fi, std::string& message)
		{
			message = "ok";

			if (fi.info.id == "")
			{
				message = "Поле ID является обязательным (Main)";
				return false;
			}

			for (const auto& p : fi.parameters)
			{
				if (p.name == "")
				{
					message = "Поле NAME у параметров является обязательным (Main)";
					return false;
				}

				if (p.type == "") // add list of values !!!
				{
					message = "Поле TYPE у параметров является обязательным (Main/" + p.name + ")";
					return false;
				}
			}

			for (const auto& t : fi.types)
			{
				if (t.name == "")
				{
					message = "Поле NAME у типов является обязательным (" + t.name + ")";
					return false;
				}

				for (const auto& tv : t.parameters)
				{
					if (tv.name == "")
					{
						message = "Поле NAME у параметров является обязательным (" + t.name + ")";
						return false;
					}

					if (tv.type == "") // add list of values !!!
					{
						message = "Поле TYPE у параметров является обязательным (" + t.name + "/" + tv.name + ")";
						return false;
					}
				}
			}

			return true;
		}
	}
}

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
#include "yaml_wiki_formatter.h"

using namespace yaml;
using namespace yaml::helper;

#define ELRF(message) do { if (!is_batch_) std::cout << message << std::endl; return false; } while(0)

wiki_formatter::wiki_formatter(bool is_batch)
{
	is_batch_ = is_batch;
}

bool wiki_formatter::format(const file_info& file_info, bool is_batch, std::list<std::string>& wiki)
{
	wiki_formatter formatter(is_batch);
	return formatter.format(file_info, wiki);
}

bool wiki_formatter::format(const file_info& file_info, std::list<std::string>& wiki)
{
	file_info_ = file_info;
	if (!get_file_wiki(file_info, wiki))
		ELRF("Get file wiki failed");
	return true;
}

bool wiki_formatter::get_file_wiki(const file_info& fi, std::list<std::string>& wiki)
{
	wiki.push_back("== Назначение ==");
	wiki.push_back(string_helper::make_string(common::get_description_as_cp1251(fi.info.description, common::description_type::wiki), "<br>"));
	wiki.push_back("");

	for (const auto& ti : file_info_.types)
	{
		std::list<std::string> type_wiki;
		if (!get_type_wiki(ti, type_wiki))
			ELRF("Get type wiki failed");
		for (const auto& s : type_wiki)
			wiki.push_back(s);
		wiki.push_back("");
	}

	std::list<std::string> main_parameters_wiki;
	if (!get_main_wiki(fi, main_parameters_wiki))
		ELRF("Get main parameters wiki failed");
	for (const auto& s : main_parameters_wiki)
		wiki.push_back(s);
	wiki.push_back("");

	wiki.push_back("== Пример конфигурационного файла ==");
	wiki.push_back(string_helper::make_string("<syntaxhighlight lang=\"xml\">"));

	xml_formatter formatter(is_batch_);
	std::list<std::string> xml;
	if (!formatter.format(fi, xml))
		ELRF("Get file xml failed");
	for (const auto& s : xml)
		wiki.push_back(s);
	wiki.push_back(string_helper::make_string("</syntaxhighlight>"));
	wiki.push_back("");

	wiki.push_back("== Версия ==");
	wiki.push_back("@GIT_COMMIT_HASH@<br>");
	wiki.push_back("");

	wiki.push_back("== Автор ==");
	wiki.push_back(common::get_as_cp1251(file_info_.info.author));
	wiki.push_back("");

	if (file_info_.info.wiki != "")
	{
		wiki.push_back("== Ссылки ==");
		wiki.push_back(string_helper::make_string("Описание на wiki: ", common::get_as_cp1251(file_info_.info.wiki)));
	}

	return true;
}

bool wiki_formatter::get_type_wiki(const type_info& ti, std::list<std::string>& wiki)
{
	wiki.push_back(string_helper::make_string("== Тип ", common::get_as_cp1251(ti.name), " =="));
	if (type::get_category(ti) == type_category::user_yml)
	{
		wiki.push_back(string_helper::make_string("{| class=\"wikitable\""));
		wiki.push_back(string_helper::make_string("|-"));
		wiki.push_back(string_helper::make_string("! Имя параметра !! Описание !! Обязательный !! Тип !! Тип xml ",
			"!! Значение по умолчанию !! Типичное значение !! Ограничения"));

		for (const auto& pi : ti.parameters)
		{
			std::list<std::string> parameter_wiki;
			if (!get_parameter_wiki(pi, parameter_wiki))
				ELRF("Get parameter wiki failed");

			for (const auto& s : parameter_wiki)
				wiki.push_back(s);
		}

		wiki.push_back(string_helper::make_string("|}"));
	}
	else if (type::get_category(ti) == type_category::user_cpp)
	{
		wiki.push_back(string_helper::make_string("<p>"));
		wiki.push_back(common::get_description_as_cp1251(ti.description, common::description_type::wiki));
		wiki.push_back(string_helper::make_string("</p>"));

		wiki.push_back(string_helper::make_string("<p>Данный тип определен в исходных файлах проекта. ",
			"Значения из конфигурационного файла будут считаны как тип <b><i>",
			type::get_type_xml(ti), "</i></b> и приведены к типу <b><i>", common::get_as_cp1251(ti.name), "</i></b></p>"));

		if (ti.type == "enum")
		{
			if (ti.includes.size() > 0)
			{
				wiki.push_back(string_helper::make_string("<p>Тип использует следующие заголовочные файлы:</p>"));
				for (const auto& i : ti.includes)
					wiki.push_back(string_helper::make_string("* ", common::get_as_cp1251(i)));
			}

			wiki.push_back(string_helper::make_string("<p>В перечислении для использования в xml определены следующие значения:</p>"));
			wiki.push_back(string_helper::make_string("{| class=\"wikitable\""));
			wiki.push_back(string_helper::make_string("|-"));
			wiki.push_back(string_helper::make_string("! Имя !! Значение"));

			for (const auto& v : ti.values)
			{
				wiki.push_back(string_helper::make_string("|-"));
				wiki.push_back(string_helper::make_string("| ", common::get_as_cp1251(v.first), " || ", common::get_as_cp1251(v.second)));
			}

			wiki.push_back(string_helper::make_string("|}"));
		}
	}
	return true;
}

bool wiki_formatter::get_main_wiki(const file_info& fi, std::list<std::string>& wiki)
{
	wiki.push_back(string_helper::make_string("== Параметры юнита =="));
	wiki.push_back(string_helper::make_string("{| class=\"wikitable\""));
	wiki.push_back(string_helper::make_string("|-"));
	wiki.push_back(string_helper::make_string("! Имя параметра !! Описание !! Обязательный !! Тип !! ",
		"Тип xml !! Значение по умолчанию !! Типичное значение !! Ограничения"));

	for (const auto& pi : file_info_.parameters)
	{
		std::list<std::string> parameter_wiki;
		if (!get_parameter_wiki(pi, parameter_wiki))
			ELRF("Get parameter wiki failed");

		for (const auto& s : parameter_wiki)
			wiki.push_back(s);
	}

	wiki.push_back(string_helper::make_string("|}"));
	return true;
}

bool wiki_formatter::get_parameter_wiki(const parameter_info& pi, std::list<std::string>& wiki)
{
	std::string restrictions_wiki;
	if (!get_restrictions_wiki(pi, restrictions_wiki))
		ELRF("Get restrictions failed");

	wiki.push_back(string_helper::make_string("|-"));
	wiki.push_back(string_helper::make_string("| ", common::get_as_cp1251(pi.name), " || ", common::get_description_as_cp1251(pi.description, common::description_type::wiki), " || ",
		get_is_required_wiki(pi), " || ", get_type_wiki(pi), " || ", get_type_xml_wiki(pi), " || ",
		common::get_as_cp1251(pi.default_), " || ", common::get_as_cp1251(pi.hint), " || ", restrictions_wiki));

	return true;
}


bool wiki_formatter::get_restrictions_wiki(const parameter_info& pi, std::string& wiki)
{
	if (parameter::get_has_restrictions(pi))
	{
		if (parameter::get_is_array(pi))
		{
			if (pi.restrictions.min_count != "")
				wiki += string_helper::make_string("Минимальное количество элементов: ", common::get_as_cp1251(pi.restrictions.min_count), "<br>");
			if (pi.restrictions.max_count != "")
				wiki += string_helper::make_string("Максимальное количество элементов: ", common::get_as_cp1251(pi.restrictions.max_count), "<br>");
			if (pi.restrictions.set_count.size() > 0)
			{
				wiki += string_helper::make_string("Допустимое количество элементов: ");
				for (int i = 0; i < pi.restrictions.set_count.size(); i++)
				{
					std::string value_str = string_helper::make_string(common::get_as_cp1251(pi.restrictions.set_count[i]));
					if (value_str.length() > 10)
						wiki += "<br>";
					wiki += value_str;
					if (i < pi.restrictions.set_count.size() - 1)
						wiki += ", ";
				}
				wiki += "<br>";
			}
		}
		else
		{
			if (pi.restrictions.min != "")
				wiki += string_helper::make_string("Минимальное значение: ", common::get_as_cp1251(pi.restrictions.min), "<br>");
			if (pi.restrictions.max != "")
				wiki += string_helper::make_string("Максимальное значение: ", common::get_as_cp1251(pi.restrictions.max), "<br>");
			if (pi.restrictions.set_.size() > 0)
			{
				wiki += "Допустимые значения: ";
				for (int i = 0; i < pi.restrictions.set_.size(); i++)
				{
					std::string value_str = string_helper::make_string(common::get_as_cp1251(pi.restrictions.set_[i]));
					if (value_str.length() > 10)
						wiki += "<br>";
					wiki += value_str;
					if (i < pi.restrictions.set_.size() - 1)
						wiki += ", ";
				}
				wiki += "<br>";
			}

			if ((pi.type == "path" || pi.type == "library") && !pi.restrictions.max_length.empty())
				wiki += string_helper::make_string("Максимальная длина: ", common::get_as_cp1251(pi.restrictions.max_length), "<br>");

			if (pi.type == "unit" && !pi.restrictions.category.empty())
				wiki += string_helper::make_string("Допустимые категории: ", common::get_as_cp1251(pi.restrictions.category), "<br>");

			if (pi.type == "unit" && pi.restrictions.ids.size() > 0)
			{
				wiki += "Допустимые юниты: ";
				for (int i = 0; i < pi.restrictions.ids.size(); i++)
				{
					std::string value_str = common::get_as_cp1251(pi.restrictions.ids[i]);
					if (value_str.length() > 10)
						wiki += "<br>";
					wiki += value_str;
					if (i < pi.restrictions.ids.size() - 1)
						wiki += ", ";
				}
				wiki += "<br>";
			}
		}
	}
	return true;
}

std::string wiki_formatter::get_is_required_wiki(const parameter_info& pi) const
{
	return parameter::get_is_required(pi) ? "да" : "нет";
}

std::string wiki_formatter::get_type_wiki(const parameter_info& pi) const
{
	std::string type_xml_html = common::get_as_cp1251(pi.type);
	type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
	type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
	return type_xml_html;
}

std::string wiki_formatter::get_type_xml_wiki(const parameter_info& pi) const
{
	std::string type_xml_html = parameter::get_type_xml(file_info_, pi);
	type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
	type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
	return type_xml_html;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <regex>
#include "git_commit_hash.h"
#include "string_helper.h"
#include "parameters/helper_common.h"
#include "parameters/helper_type.h"
#include "parameters/helper_parameter.h"
#include "parameters/formatter_xml.h"
#include "parameters/formatter_html.h"

using namespace parameters;
using namespace parameters::formatter;

#define ELRF(message) do { if (!is_batch_) std::cout << message << std::endl; return false; } while(0)

html_formatter::html_formatter(bool is_batch)
{
	is_batch_ = is_batch;
}

bool html_formatter::format(const file_info& file_info, bool is_batch, std::list<std::string>& html)
{
	html_formatter formatter(is_batch);
	return formatter.format(file_info, html);
}

bool html_formatter::format(const file_info& file_info, std::list<std::string>& html)
{
	file_info_ = file_info;
	if (!get_file_html(file_info, html))
		ELRF("Get file html failed");
	return true;
}

bool html_formatter::get_file_html(const file_info& fi, std::list<std::string>& html)
{
	html.push_back("<!DOCTYPE html>");
	html.push_back("<html>");
	html.push_back("<head>");
	html.push_back(string_helper::make_string("\t<title>", helper::common::get_as_cp1251(file_info_.info.id), "</title>"));
	html.push_back("\t<link href=\"parameters_compiler.css\" rel=\"stylesheet\" type=\"text/css\">");
	html.push_back("\t<script src=\"highlight.min.js\"></script>");
	html.push_back("\t<script>hljs.initHighlightingOnLoad();</script>");
	html.push_back("</head>");
	html.push_back("");

	html.push_back("<pc-variables>");
	html.push_back(string_helper::make_string("\t<pc-id>", helper::common::get_as_cp1251(file_info_.info.id), "</pc-id>"));
	html.push_back(string_helper::make_string("\t<pc-display-name>", helper::common::get_as_cp1251(file_info_.info.display_name), "</pc-display-name>"));
	html.push_back(string_helper::make_string("\t<pc-description>", helper::common::get_description_as_cp1251(file_info_.info.description, helper::common::description_type::html), "</pc-description>"));
	html.push_back(string_helper::make_string("\t<pc-category>", helper::common::get_as_cp1251(file_info_.info.category), "</pc-category>"));
	html.push_back("</pc-variables>");
	html.push_back("");

	html.push_back("<body>");
	html.push_back(string_helper::make_string("<h1>", helper::common::get_as_cp1251(file_info_.info.display_name), "</h1>"));

	// Содержание
	html.push_back("<div class=\"content\">");
	html.push_back("<h2>Содержание</h2>");
	html.push_back("<ol>");
	html.push_back(string_helper::make_string("<li><a href=\"#purpose\">Назначение</a></li>"));
	for (const auto& ti : file_info_.types)
		html.push_back(string_helper::make_string("<li><a href=\"#", helper::common::get_as_cp1251(ti.name), "\">Тип ", helper::common::get_as_cp1251(ti.name), "</a></li>"));
	if (file_info_.parameters.size() > 0)
		html.push_back(string_helper::make_string("<li><a href=\"#params\">Параметры юнита</a></li>"));
	html.push_back(string_helper::make_string("<li><a href=\"#config_xml\">Пример конфигурационного файла</a></li>"));
	html.push_back(string_helper::make_string("<li><a href=\"#version\">Версия</a></li>"));
	html.push_back(string_helper::make_string("<li><a href=\"#author\">Автор</a></li>"));
	if (file_info_.info.wiki != "")
		html.push_back(string_helper::make_string("<li><a href=\"#links\">Ссылки</a></li>"));
	html.push_back("</ol>");
	html.push_back("</div>");


	html.push_back(string_helper::make_string("<a name=\"purpose\"/><h2>Назначение</h2>"));
	html.push_back(string_helper::make_string("<p>", helper::common::get_description_as_cp1251(file_info_.info.description, helper::common::description_type::html), "</p>"));
	html.push_back("");

	for (const auto& ti : file_info_.types)
	{
		std::list<std::string> type_html;
		if (!get_type_html(ti, type_html))
			ELRF("Get type html failed");
		for (const auto& s : type_html)
			html.push_back(s);
	}
	html.push_back("");

	std::list<std::string> main_parameters_html;
	if (!get_main_html(fi, main_parameters_html))
		ELRF("Get main parameters html failed");
	for (const auto& s : main_parameters_html)
		html.push_back(s);
	html.push_back("");

	html.push_back("<a name=\"config_xml\"/><h2>Пример конфигурационного файла</h2>");
	html.push_back("<pre><code class=\"xml\">");
	formatter::xml_formatter formatter(is_batch_);
	std::list<std::string> xml;
	if (!formatter.format(fi, xml))
		ELRF("Get file xml failed");
	for (const auto& s : xml)
	{
		std::string ss = s;
		ss = std::regex_replace(ss, std::regex("&"), "&amp;");
		ss = std::regex_replace(ss, std::regex("<"), "&lt;");
		ss = std::regex_replace(ss, std::regex(">"), "&gt;");
		ss = std::regex_replace(ss, std::regex("\""), "&quot;");
		html.push_back(string_helper::make_string(ss));
	}
	html.push_back("</code></pre>");
	html.push_back("");

	html.push_back("<a name=\"version\"/><h2>Версия</h2>");
	html.push_back("<p>@GIT_COMMIT_HASH@</p>");
	html.push_back("");

	html.push_back("<a name=\"author\"/><h2>Автор</h2>");
	html.push_back(string_helper::make_string("<p>", helper::common::get_as_cp1251(file_info_.info.author), "</p>"));
	html.push_back("");

	if (file_info_.info.wiki != "")
	{
		html.push_back("<a name=\"links\"/><h2>Ссылки</h2>");
		html.push_back(string_helper::make_string("<p>Описание на wiki: <a href=\"", helper::common::get_as_cp1251(file_info_.info.wiki),
			"\">", helper::common::get_as_cp1251(file_info_.info.wiki), "</a></p>"));
	}

	html.push_back("</body>");
	html.push_back("</html>");

	return true;
}

bool html_formatter::get_type_html(const type_info& ti, std::list<std::string>& html)
{
	html.push_back(string_helper::make_string("<a name=\"", helper::common::get_as_cp1251(ti.name), "\"/>", "<h2>Тип ", helper::common::get_as_cp1251(ti.name), "</h2>"));
	if (helper::type::get_category(ti) == type_category::user_yml)
	{
		html.push_back("<table>");
		html.push_back("<tr>");
		html.push_back("<th>Имя параметра</th>");
		html.push_back("<th>Описание</th>");
		html.push_back("<th>Обязательный</th>");
		html.push_back("<th>Тип</th>");
		html.push_back("<th>Тип xml</th>");
		html.push_back("<th>Значение<br/>по умолчанию</th>");
		html.push_back("<th>Типичное<br/>значение</th>");
		html.push_back("<th>Ограничения</th>");
		html.push_back("</tr>");

		for (const auto& pi : ti.parameters)
		{
			std::list<std::string> parameter_html;
			if (!get_parameter_html(pi, parameter_html))
				ELRF("Get parameter html failed");

			for (const auto& s : parameter_html)
				html.push_back(s);
		}

		html.push_back("</table>");
	}
	else if (helper::type::get_category(ti) == type_category::user_cpp)
	{
		html.push_back(string_helper::make_string("<p>"));
		html.push_back(helper::common::get_description_as_cp1251(ti.description, helper::common::description_type::html));
		html.push_back(string_helper::make_string("</p>"));
		html.push_back(string_helper::make_string("<p>"));
		html.push_back(string_helper::make_string("Данный тип определен в исходных файлах проекта. ",
			"Значения из конфигурационного файла будут считаны как тип <b><i>",
			helper::type::get_type_xml(ti), "</i></b> и приведены к типу <b><i>", helper::common::get_as_cp1251(ti.name), "</i></b><br/>"));
		html.push_back("</p>");

		if (ti.type == "enum")
		{
			if (ti.includes.size() > 0)
			{
				html.push_back("<p>");
				html.push_back(string_helper::make_string("Тип использует следующие заголовочные файлы:<br/>"));
				html.push_back(string_helper::make_string("<ul>"));
				for (const auto& i : ti.includes)
					html.push_back(string_helper::make_string("<li>", helper::common::get_as_cp1251(i), "</li>"));
				html.push_back(string_helper::make_string("</ul>"));
				html.push_back("</p>");
			}

			html.push_back("<p>");
			html.push_back(string_helper::make_string("В перечислении для использования в xml определены следующие значения:<br/>"));
			html.push_back("<table>");
			html.push_back("<tr>");
			html.push_back("<th>Имя</th>");
			html.push_back("<th>Значение</th>");
			html.push_back("</tr>");
			for (const auto& v : ti.values)
			{
				html.push_back("<tr>");
				html.push_back(string_helper::make_string("<td>", helper::common::get_as_cp1251(v.first), "</td>"));
				html.push_back(string_helper::make_string("<td style=\"text-align:center\">", helper::common::get_as_cp1251(v.second), "</td>"));
				html.push_back("</tr>");
			}
			html.push_back("</table>");
			html.push_back("</p>");
		}
	}
	return true;
}

bool html_formatter::get_main_html(const file_info& fi, std::list<std::string>& html)
{
	if (file_info_.parameters.size() > 0)
	{
		html.push_back("<a name=\"params\"/><h2>Параметры юнита</h2>");
		html.push_back("<table>");
		html.push_back("<tr>");
		html.push_back("<th>Имя параметра</th>");
		html.push_back("<th>Описание</th>");
		html.push_back("<th>Обязательный</th>");
		html.push_back("<th>Тип</th>");
		html.push_back("<th>Тип xml</th>");
		html.push_back("<th>Значение<br/>по умолчанию</th>");
		html.push_back("<th>Типичное<br/>значение</th>");
		html.push_back("<th>Ограничения</th>");
		html.push_back("</tr>");

		for (const auto& pi : file_info_.parameters)
		{
			std::list<std::string> parameter_html;
			if (!get_parameter_html(pi, parameter_html))
				ELRF("Get parameter html failed");

			for (const auto& s : parameter_html)
				html.push_back(s);
		}

		html.push_back("</table>");
	}
	return true;
}

bool html_formatter::get_parameter_html(const parameter_info& pi, std::list<std::string>& html)
{
	std::string restrictions_html;
	if (!get_restrictions_html(pi, restrictions_html))
		ELRF("Get restrictions failed");

	html.push_back("<tr>");
	html.push_back(string_helper::make_string("<td>", helper::common::get_as_cp1251(pi.name), "</td>"));
	html.push_back(string_helper::make_string("<td>", helper::common::get_description_as_cp1251(pi.description, helper::common::description_type::html), "</td>"));
	html.push_back(string_helper::make_string("<td style=\"text-align:center\">", get_is_required_html(pi), "</td>"));
	html.push_back(string_helper::make_string("<td style=\"text-align:center\">", get_type_html(pi), "</td>"));
	html.push_back(string_helper::make_string("<td style=\"text-align:center\">", get_type_xml_html(pi), "</td>"));
	html.push_back(string_helper::make_string("<td style=\"text-align:center\">", helper::common::get_as_cp1251(pi.default_), "</td>"));
	html.push_back(string_helper::make_string("<td style=\"text-align:center\">", helper::common::get_as_cp1251(pi.hint), "</td>"));
	html.push_back(string_helper::make_string("<td>", restrictions_html, "</td>"));
	html.push_back("</tr>");
	return true;
}


bool html_formatter::get_restrictions_html(const parameter_info& pi, std::string& html)
{
	if (helper::parameter::get_has_restrictions(pi))
	{
		if (helper::parameter::get_is_array(pi))
		{
			if (pi.restrictions.min_count != "")
				html += string_helper::make_string("Минимальное количество элементов: ", helper::common::get_as_cp1251(pi.restrictions.min_count), "<br>");
			if (pi.restrictions.max_count != "")
				html += string_helper::make_string("Максимальное количество элементов: ", helper::common::get_as_cp1251(pi.restrictions.max_count), "<br>");
			if (pi.restrictions.set_count.size() > 0)
			{
				html += string_helper::make_string("Допустимое количество элементов: ");
				for (int i = 0; i < pi.restrictions.set_count.size(); i++)
				{
					std::string value_str = string_helper::make_string(helper::common::get_as_cp1251(pi.restrictions.set_count[i]));
					if (value_str.length() > 10)
						html += "<br>";
					html += value_str;
					if (i < pi.restrictions.set_count.size() - 1)
						html += ", ";
				}
				html += "<br>";
			}
		}
		else
		{
			if (pi.restrictions.min != "")
				html += string_helper::make_string("Минимальное значение: ", helper::common::get_as_cp1251(pi.restrictions.min), "<br>");
			if (pi.restrictions.max != "")
				html += string_helper::make_string("Максимальное значение: ", helper::common::get_as_cp1251(pi.restrictions.max), "<br>");
			if (pi.restrictions.set_.size() > 0)
			{
				html += "Допустимые значения: ";
				for (int i = 0; i < pi.restrictions.set_.size(); i++)
				{
					std::string value_str = string_helper::make_string(helper::common::get_as_cp1251(pi.restrictions.set_[i]));
					if (value_str.length() > 10)
						html += "<br>";
					html += value_str;
					if (i < pi.restrictions.set_.size() - 1)
						html += ", ";
				}
				html += "<br>";
			}

			if ((pi.type == "path" || pi.type == "library") && !pi.restrictions.max_length.empty())
				html += string_helper::make_string("Максимальная длина: ", helper::common::get_as_cp1251(pi.restrictions.max_length), "<br>");

			if (pi.type == "unit" && !pi.restrictions.category.empty())
				html += string_helper::make_string("Допустимые категории: ", helper::common::get_as_cp1251(pi.restrictions.category), "<br>");

			if (pi.type == "unit" && pi.restrictions.ids.size() > 0)
			{
				html += "Допустимые юниты: ";
				for (int i = 0; i < pi.restrictions.ids.size(); i++)
				{
					std::string value_str = helper::common::get_as_cp1251(pi.restrictions.ids[i]);
					if (value_str.length() > 10)
						html += "<br>";
					html += value_str;
					if (i < pi.restrictions.ids.size() - 1)
						html += ", ";
				}
				html += "<br>";
			}
		}
	}
	return true;
}

std::string html_formatter::get_is_required_html(const parameter_info& pi) const
{
	return helper::parameter::get_is_required(pi) ? "да" : "нет";
}

std::string html_formatter::get_type_html(const parameter_info& pi) const
{
	std::string type_xml_html = helper::common::get_as_cp1251(pi.type);
	type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
	type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
	return type_xml_html;
}

std::string html_formatter::get_type_xml_html(const parameter_info& pi) const
{
	std::string type_xml_html = helper::parameter::get_type_xml(file_info_, pi);
	type_xml_html = std::regex_replace(type_xml_html, std::regex("<"), "&lt;");
	type_xml_html = std::regex_replace(type_xml_html, std::regex(">"), "&gt;");
	return type_xml_html;
}

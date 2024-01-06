#pragma once

#include <regex>
#include "encoding_cp1251.h"
#include "string_helper.h"
#include "definitions.h"
#include "parameters/types.h"
#include "parameters/helper_common.h"
#include "parameters/helper_type.h"
#include "parameters/helper_parameter.h"

using namespace parameters;
using namespace parameters::helper;

std::string common::get_as_cp1251(const std::string value)
{
	return EncodingCP1251::utf8_to_cp1251(value);
}

std::string common::get_description_as_cp1251(const std::string& description, const description_type type)
{
	std::string description_internal = EncodingCP1251::utf8_to_cp1251(description);
	string_helper::trim(description_internal, '\n');

	if (description_internal.size() == 0)
		return description_internal;

	// <plain> </plain>, <html> </html>, <wiki> </wiki> - специальные таги для включения в описание
	// блока plain/HTML/WIKI разметки
	// Вложенность и использование краткой формы <plain/>, <html/>, <wiki/> не допускается
	// При генерации описания соответствующего типа, таги других типов игнорируются

	const std::map<description_type, std::pair<std::string, std::string>> tags = {
		{ description_type::plain, {"<plain>", "</plain>"} },
		{ description_type::html, {"<html>", "</html>"} },
		{ description_type::wiki, {"<wiki>", "</wiki>"} }
	};

	std::string result;
	bool first_block = true;
	size_t after_close = 0;
	while (true)
	{
		// Проверка на выход
		if (after_close >= description_internal.size())
			break;

		// Ищем открывающий тег
		size_t open_position = description_internal.size();
		description_type open_type{};
		for (const auto& tag : tags)
		{
			size_t open = description_internal.find(tag.second.first, after_close);
			if (open == std::string::npos)
				open = description_internal.size();

			if (open < open_position)
			{
				open_position = open;
				open_type = tag.first;
			}
		}

		// Копируем простой текст
		{
			auto text = description_internal.substr(after_close, open_position - after_close);

			// Удаляем первый и последний перевод строки
			string_helper::trim(text, '\n');

			// Проверяем, надо ли добавить перевод строки перед блоком текста
			bool add_line_to_text = false;
			if (text != "" && !first_block)
				add_line_to_text = true;
			else if (text != "")
				first_block = false;

			// Добавляем блок простого текста с учетом типа результирующей строки 
			if (type == description_type::plain)
			{
				if (add_line_to_text)
					result += '\n';
				auto plain = text;
				result += plain;
			}
			else if (type == description_type::html)
			{
				if (add_line_to_text)
					result += "<br>";
				auto html = std::regex_replace(text, std::regex("\n"), "<br>");
				result += html;
			}
			else if (type == description_type::wiki)
			{
				if (add_line_to_text)
					result += '\n';
				auto wiki = std::regex_replace(text, std::regex("\n"), "<br>");
				result += wiki;
			}
		}

		// Проверка на выход
		if (open_position >= description_internal.size())
			break;

		// Ищем закрывающий тег
		size_t close = description_internal.find(tags.at(open_type).second, open_position);
		if (close == std::string::npos) // это ошибка
			return description_internal;

		// Считаем позицию
		after_close = close + tags.at(open_type).second.size();

		// Копируем содержимое тега
		if (type == open_type)
		{
			auto tag_text = description_internal.substr(open_position + tags.at(open_type).first.size(),
				(after_close - tags.at(open_type).second.size()) - (open_position + tags.at(open_type).first.size()));
			// Удаляем первый и последний перевод строки
			string_helper::trim(tag_text, '\n');

			// Проверяем, надо ли добавить перевод строки перед блоком текста
			bool add_line_to_tag_text = false;
			if (tag_text != "" && !first_block)
				add_line_to_tag_text = true;
			else if (tag_text != "")
				first_block = false;

			// Добавляем блок текста из тага (тип обязан совпадть с типом результирующей строки) 
			if (open_type == description_type::plain)
			{
				if (add_line_to_tag_text)
					result += "\n";
				auto plain = tag_text;
				result += plain;
			}
			else if (open_type == description_type::html)
			{
				if (add_line_to_tag_text)
					result += "<br>";
				auto html = std::regex_replace(tag_text, std::regex("\n"), "");
				result += html;
			}
			else if (open_type == description_type::wiki)
			{
				if (add_line_to_tag_text)
					result += "\n";
				auto wiki = tag_text;
				result += wiki;
			}
		}
	}

	//if (type == description_type::wiki)
	//	result = std::regex_replace(result, std::regex(" "), "&nbsp;");

	//if (type == description_type::wiki)
	//	result = std::regex_replace(result, std::regex(" "), "&nbsp;");

	return result;
}

std::string common::get_hint_html_as_cp1251(const struct_types type, const std::string& name)
{
	if (type == struct_types::file_info)
	{
		if (name == "FILE_FORMAT")
			return "<p>Номер версии формата yml файла, для обратной совместимости, в данный существует только одна версия.</p><p>Поле не обязательное, если не задано, будет использовано значение 1.</p>";
		else if (name == "INFO")
			return "<p>Общее описание юнита.</p><p>Обязательная секция.</p>";
		else if (name == "TYPES")
			return "<p>Пользовательские типы данных для использования в массивах..</p><p>Секция может отсутствовать, если пользовательские типы не используются.</p>";
		else if (name == "PARAMETERS")
			return "<p>Параметры юнита.</p><p>Секция может отсутствовать, если у юнита нет параметров.</p>";
	}
	else if (type == struct_types::info_info)
	{
		if (name == "ID")
			return "<p>Идентификатор юнита (имя библиотеки), совпадает с ID в xml файле.</p><p>Обязательное поле.</p>";
		else if (name == "DISPLAY_NAME")
			return "<p>Имя юнита для отображения, должно быть достаточно кратким, на русском языке.</p><p>Поле не обязательное, если не задано, в качестве имени берется ID<p>";
		else if (name == "DESCRIPTION")
			return "<p>Текстовое описание назначения юнита, на русском языке. Может содержать несколько строк.</p><p>Поле не обязательное.<p>";
		else if (name == "CATEGORY")
			return "<p>Категория юнита из матрешки (Handlers, Drivers…).</p><p>Поле не обязательное.<p>";
		else if (name == "HINT")
			return "<p>Подсказка на типичное значение имени экземпляра данного юнита.</p><p>Поле не обязательное.<p>";
		else if (name == "PICTOGRAM")
			return "<p>Иконка, закодированная в MIME/BASE64, для отображения в редакторе конфигов.</p><p>Поле не обязательное, в данный момент не используется.<p>";
		else if (name == "AUTHOR")
			return "<p>Имя автора.</p><p>Поле не обязательное.<p>";
		else if (name == "WIKI")
			return "<p>Адрес описания юнита в нашей wiki.</p><p>Поле не обязательное.<p>";
	}
	else if (type == struct_types::type_info)
	{
		if (name == "NAME")
			return "<p>Имя пользовательского типа, при компиляции имя типа остается без изменений и используется для наименования класса параметров и переменных.</p><p>Обязательное поле.</p>";
		else if (name == "TYPE")
			return "<p>Тип пользовательского типа, yml или enum.</p><p>Поле не обязательное, по умолчанию используется тип yml.<p>";
		else if (name == "DESCRIPTION")
			return "<p>Текстовое описание типа, на русском языке. Может содержать несколько строк.</p><p>Поле не обязательное.<p>";
		else if (name == "PARAMETERS")
			return "<p>Параметры, содержащиеся в данном типе. Формат такой же как в основной секции параметров.</p><p>Поле обязательное для пользовательских типов yml.<p>";
		else if (name == "VALUES")
			return "<p>Допустимые значения для перечисления и их эквивалент в файле cpp. Содержит список пар значений.</p><p>Поле обязательное для пользовательских типов enum.<p>";
		else if (name == "INCLUDES")
			return "<p>Дополнительно подключаемые заголовочные файлы, необходимые для данного типа.</p><p>Поле не обязательное.<p>";
	}
	else if (type == struct_types::parameter_info)
	{
		if (name == "NAME")
			return "<p>Имя параметра в xml файле.</p><p>Обязательное поле.</p>";
		else if (name == "TYPE")
			return "<p>Реальный тип параметра.</p><p>Обязательное поле.<p>";
		else if (name == "DISPLAY_NAME")
			return "<p>Имя параметра для отображения, должно быть достаточно кратким, на русском языке.</p><p>Поле не обязательное, если не задано, в качестве имени берется NAME.<p>";
		else if (name == "DESCRIPTION")
			return "<p>Текстовое описание параметра, на русском языке. Может содержать несколько строк.</p><p>Поле не обязательное.<p>";
		else if (name == "REQUIRED")
			return "<p>Является ли параметр обязательным.</p><p>Поле не обязательное, если не задано, считается равным true.<p>";
		else if (name == "DEFAULT")
			return "<p>Значение по умолчанию для необязательных параметров.</p><p>Поле не обязательное. Для обязательных параметров не используется. При отсутствии параметра в xml, берется значение по умолчанию, если оно задано, иначе, считаем параметр опциональным.<p>";
		else if (name == "HINT")
			return "<p>Подсказка на типичное значение параметра.</p><p>Поле не обязательное.<p>";
		else if (name == "RESTRICTIONS")
			return "<p>Ограничения на параметр (как на значение, так и на количество параметров для массивов).</p><p>Поле не обязательное.<p>";
	}
	else if (type == struct_types::restrictions_info)
	{
		if (name == "MIN")
			return "<p>Минимальное значение.</p><p>Поле не обязательное.</p>";
		else if (name == "MAX")
			return "<p>Максимальное значение.</p><p>Поле не обязательное.</p>";
		else if (name == "SET")
			return "<p>Допустимые значения в виде списка.</p><p>Поле не обязательное.</p>";
		else if (name == "MIN_COUNT")
			return "<p>Минимальное количество элементов массива.</p><p>Поле не обязательное.</p>";
		else if (name == "MAX_COUNT")
			return "<p>Максимальное количество элементов массива.</p><p>Поле не обязательное.</p>";
		else if (name == "SET_COUNT")
			return "<p>Допустимые значения количества элементов массива в виде списка.</p><p>Поле не обязательное.</p>";
		else if (name == "CATEGORY")
			return "<p>Категория юнитов, которые можно использовать для параметра типа unit.</p><p>Поле не обязательное.</p>";
		else if (name == "IDS")
			return "<p>Конкретные типы (id) юнитов, которые можно использовать для параметра типа unit, в виде списка.</p><p>Поле не обязательное.</p>";
		else if (name == "MAX_LENGTH")
			return "<p>Максимальная допустимая длина пути для параметров типа path.</p><p>Поле не обязательное.</p>";
	}

	return "";
}

bool common::extract_array_file_info(file_info& fi, const std::string& type, const std::string& name, file_info& afi)
{
	afi = {};

	// Работает только для массивов пользовательского типа данных, не перечислений
	// Находим тип элемента массива, переносим все его параметры в основную секцию
	// Все типы, которыые идут после этого нам не нужны,их удаляем
	// Имя и описание юнита заменяем на имя и описание параметра

	auto pi = parameter::get_parameter_info(fi, type, name);
	if (pi == nullptr)
		return false;

	bool is_array = common::get_is_array_type(pi->type);
	bool is_inner_type = common::get_is_inner_type(pi->type);

	if (is_array && !is_inner_type)
	{
		std::string array_type = common::get_item_type(pi->type);
		auto pti = type::get_type_info(fi, array_type);
		if (pti == nullptr || pti->type != "yml")
			return false;

		afi.format = fi.format;
		afi.info = fi.info;
		afi.info.id = pti->name;
		afi.info.display_name = pi->display_name;
		afi.info.description = pi->description;
		afi.parameters = pti->parameters;

		for (const auto& t : fi.types)
		{
			if (t.name != array_type)
				afi.types.push_back(t);
			else
				break;
		}
	}
	else
		return false;

	return true;
}

bool common::get_is_array_type(const std::string& name)
{
	bool is_array = false;
	if (name.length() > std::string("array<>").length() &&
		name.substr(0, std::string("array<").length()) == "array<" &&
		name[name.length() - 1] == '>')
		is_array = true;
	return is_array;
}

bool common::get_is_unit_type(const std::string& name)
{
	return name == "unit";
}

std::string common::get_item_type(const std::string& name)
{
	std::string item_type{ name };
	if (name.length() > std::string("array<>").length() &&
		name.substr(0, std::string("array<").length()) == "array<" &&
		name[name.length() - 1] == '>')
		item_type = name.substr(std::string("array<").length(), name.length() - std::string("array<>").length());
	return item_type;
}

base_types common::get_base_type(const std::string& name)
{
	if (get_is_array_type(name))
	{
		return base_types::array;
	}
	else if (common::get_is_inner_type(name))
	{
		const auto bit = get_base_item_type(name);
		switch (bit)
		{
		case base_item_types::string:
			return base_types::string;
		case base_item_types::integer:
			return base_types::integer;
		case base_item_types::floating:
			return base_types::floating;
		case base_item_types::boolean:
			return base_types::boolean;
		case base_item_types::none: // impossible, it's inner type
		case base_item_types::user: // impossible, it's inner type
		default:
			return base_types::none; // impossible, it's inner type
		}
	}
	else
	{
		// user_cpp (enum), yaml must be array
		return base_types::user_cpp;
	}
}

base_item_types common::get_base_item_type(const std::string& name)
{
	auto item_type = get_item_type(name);
	if (common::get_is_inner_type(item_type))
	{
		const auto bit = parameter_to_base_item_types.find(name);
		if (bit != parameter_to_base_item_types.cend())
		{
			return bit->second;
		}
		else
		{
			// Impossible, it's inner type
			// To avoid warning C4715: not all control paths return a value
			return base_item_types::none;
		}
	}
	else
	{
		// user_cpp (enum) or user_yml
		return base_item_types::user;
	}
}

base_item_types common::get_xml_base_item_type(const std::string& name)
{
	// Xml types have no arrays
	const auto bit = xml_to_base_item_types.find(name);
	if (bit != xml_to_base_item_types.cend())
	{
		return bit->second;
	}
	else
	{
		return base_item_types::none;
	}
}

bool common::get_is_inner_type(const std::string& name)
{
	return (std::find(parameter_types.cbegin(), parameter_types.cend(), name) != parameter_types.cend());
}

int common::get_min_for_integral_type(const std::string& name)
{
	if (name == "uint8_t" || name == "uint16_t" || name == "uint32_t" || name == "uint64_t")
		return 0;
	else if (name == "int" || name == "int32_t" || name == "int64_t")
		return std::numeric_limits<int32_t>::min();
	else if (name == "int8_t")
		return std::numeric_limits<int8_t>::min();
	else if (name == "int16_t")
		return std::numeric_limits<int16_t>::min();
	return std::numeric_limits<int32_t>::min();
}

int common::get_max_for_integral_type(const std::string& name)
{
	if (name == "int" || name == "int32_t" || name == "int64_t" || name == "uint32_t" || name == "uint64_t")
		return std::numeric_limits<int32_t>::max();
	else if (name == "int8_t")
		return std::numeric_limits<int8_t>::max();
	else if (name == "int16_t")
		return std::numeric_limits<int16_t>::max();
	else if (name == "uint8_t")
		return std::numeric_limits<uint8_t>::max();
	else if (name == "uint16_t")
		return std::numeric_limits<uint16_t>::max();
	return std::numeric_limits<int32_t>::max();
}

int common::get_min_for_floating_point_type(const std::string& name)
{
	if (name == "float")
		return std::numeric_limits<float>::lowest();
	else if (name == "double")
		return std::numeric_limits<double>::lowest();
	return std::numeric_limits<double>::lowest();
}

int common::get_max_for_floating_point_type(const std::string& name)
{
	if (name == "float")
		return std::numeric_limits<float>::max();
	else if (name == "double")
		return std::numeric_limits<double>::max();
	return std::numeric_limits<double>::max();
}

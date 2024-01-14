#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace parameter
		{
			// Получить категорию типа параметра pi
			type_category get_category(const file_info& fi, const parameter_info& pi);

			// Получить имя xml-типа параметра pi (например, unit->str, array<double>->array<dbl>)
			std::string get_type_xml(const file_info& fi, const parameter_info& pi);

			// Получить имя class-типа параметра pi (например, user_enum_name->user_enum_name, array<double>->std::vector<double>)
			std::string get_type_class(const file_info& fi, const parameter_info& pi);

			// Получить имя типа параметра pi, а если параметр - массив, то имя типа элемента массива
			// (например, unit->unit, array<double>->double)
			std::string get_item_type(const parameter_info& pi);

			// Получить имя xml-типа параметра pi, а если параметр - массив, то имя xml-типа элемента массива
			// (например, unit->str, array<double>->dbl)
			std::string get_item_type_xml(const file_info& fi, const parameter_info& pi);

			// Получить имя cpp-типа параметра pi, а если параметр - массив, то имя cpp-типа элемента массива
			// (например, enum->std::string, array<double>->double)
			std::string get_item_type_cpp(const file_info& fi, const parameter_info& pi);

			// Получить имя class-типа параметра pi, а если параметр - массив, то имя class-типа элемента массива
			// (например, user_enum_name->user_enum_name, array<double>->double)
			std::string get_item_type_class(const file_info& fi, const parameter_info& pi);



			// Получить флаг, является ли параметр pi массивом
			bool get_is_array(const parameter_info& pi);

			// Получить флаг, является ли параметр pi обязательным
			bool get_is_required(const parameter_info& pi);

			// Получить флаг, является ли параметр pi опциональным
			bool get_is_optional(const parameter_info& pi);

			// Получить флаг, имеет ли параметр pi ограничения
			bool get_has_restrictions(const parameter_info& pi);



			//
			std::string get_display_name(const parameter_info& pi);

			//
			variant get_initial_value(const file_info& fi, const parameter_info& pi, bool is_item);

			//
			bool compare(const parameter_info& pi1, const parameter_info& pi2, std::string message);













			// Низкоуровневые функции доступа к данным

			//
			std::vector<parameter_info>* get_parameters(file_info& fi, const std::string& type);
			
			//
			const std::vector<parameter_info>* get_parameters(const file_info& fi, const std::string& type);

			//
			parameter_info* get_parameter_info(file_info& fi, const std::string& type, const std::string& name);

			//
			const parameter_info* get_parameter_info(const file_info& fi, const std::string& type, const std::string& name);

			//
			parameter_info* get_parameter_info(type_info& ti, const std::string& name);

			//
			const parameter_info* get_parameter_info(const type_info& ti, const std::string& name);
		}
	}
}

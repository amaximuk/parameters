#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace common
		{
			// Тип описания (юнита, параметра или типа)
			// Из одной и той же строки в зависимости от типа формируются разные описания,
			// обрабатываются так же вставки форматированного текста с тэгами <plain>, <html>, <wiki>
			enum class description_type
			{
				plain,
				html,
				wiki
			};

			// Преобразовать кодировку строки из utf-8 в 1251
			std::string get_as_cp1251(const std::string value);

			// Преобразовать строку описания (юнита, параметра или типа) к одному из вариантов description_type
			std::string get_description_as_cp1251(const std::string& description, const description_type type);

			//
			std::string get_hint_html_as_cp1251(const struct_types type, const std::string& name);
			
			//
			bool extract_array_file_info(file_info& fi, const std::string& type, const std::string& name, file_info& afi);

			//
            bool get_is_array_type(const std::string& name);
            
			//
			std::string get_item_type(const std::string& name);
            
			//
			base_types get_base_type(const std::string& name);
			
			//
			bool get_is_inner_type(std::string name);

		}
	}
}

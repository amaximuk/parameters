#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace common
		{
			// ��� �������� (�����, ��������� ��� ����)
			// �� ����� � ��� �� ������ � ����������� �� ���� ����������� ������ ��������,
			// �������������� ��� �� ������� ���������������� ������ � ������ <plain>, <html>, <wiki>
			enum class description_type
			{
				plain,
				html,
				wiki
			};

			// ������������� ��������� ������ �� utf-8 � 1251
			std::string get_as_cp1251(const std::string value);

			// ������������� ������ �������� (�����, ��������� ��� ����) � ������ �� ��������� description_type
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

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

			// �������� ��������� ������������� ���������
			std::string get_type_type_as_string(const type_types value);

			// �������� ��������� ������������� ���������
			std::string get_system_type_as_string(const system_types value);

			// �������� ��������� ������������� ���������
			std::string get_cpp_type_as_string(const cpp_types value);

			// ������������� ��������� ������ �� utf-8 � 1251
			std::string get_as_cp1251(const std::string& value);

			// ������������� ������ �������� (�����, ��������� ��� ����) � ������ �� ��������� description_type
			std::string get_description_as_cp1251(const std::string& description, const description_type type);

			//
			std::string get_hint_html_as_cp1251(const struct_types type, const std::string& name);
			
			//
			bool extract_array_file_info(file_info& fi, const std::string& type, const std::string& name, file_info& afi);

			//
            bool get_is_array_type(const std::string& name);
            
			//
            bool get_is_unit_type(const std::string& name);

			//
			std::string get_item_type(const std::string& name);

			//
			base_types get_base_type(const std::string& name);
			
			//
			base_item_types get_base_item_type(const std::string& name);
			
			//
			base_item_types get_xml_base_item_type(const std::string& name);
            
			//
			bool get_is_inner_type(const std::string& name);

			//
			int get_min_for_integral_type(const std::string& name);

			//
			int get_max_for_integral_type(const std::string& name);

			//
			int get_min_for_floating_point_type(const std::string& name);

			//
			int get_max_for_floating_point_type(const std::string& name);
		}
	}
}

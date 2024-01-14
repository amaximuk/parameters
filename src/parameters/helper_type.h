#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace type
		{
			const std::string main_type = "Main";

			// �������� ��������� ���� ti (�� ������ ������ ���� user_yml, ���� user_cpp, � ������� ������ enum)
			type_category get_category(const type_info& ti);

			// �������� ��� xml-���� ���� ti (��������, enum->str)
			std::string get_type_xml(const type_info& ti);

			// �������� ��� cpp-���� ���� ti (��������, enum->std::string)
			std::string get_type_cpp(const type_info& ti);





			// type_info (and file_info as type)

			bool set_parameter(file_info& fi, const std::string& type, const parameter_info& pi);
			bool have_parameter(file_info& fi, const std::string& type, const std::string& name);
			bool add_parameter(file_info& fi, const std::string& type, const parameter_info& pi);
			bool remove_parameter(file_info& fi, const std::string& type, const std::string& name);
			bool move_parameter(file_info& fi, const std::string& type, const std::string& name, const bool up);
			bool rename_parameter(file_info& fi, const std::string& type, const std::string& oldName, const std::string& newName);

			bool have_value(file_info& fi, const std::string& type, const std::string& name);
			bool add_value(file_info& fi, const std::string& type, const std::string& name, const std::string& value);
			bool remove_value(file_info& fi, const std::string& type, const std::string& name);
			bool move_value(file_info& fi, const std::string& type, const std::string& name, const bool up);

			bool have_include(file_info& fi, const std::string& type, const std::string& name);
			bool add_include(file_info& fi, const std::string& type, const std::string& name);
			bool remove_include(file_info& fi, const std::string& type, const std::string& name);
			bool move_include(file_info& fi, const std::string& type, const std::string& name, const bool up);

			//
			bool compare(const type_info& ti1, const type_info& ti2, std::string message);





			// �������������� ������� ������� � ������

			// �������� ��������� �� ��������� �������� ���� �� ��� �����
			type_info* get_type_info(file_info& fi, const std::string& name);

			// �������� ����������� ��������� �� ��������� �������� ���� �� ��� �����
			const type_info* get_type_info(const file_info& fi, const std::string& name);

		}
	}
}

#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace parameter
		{
			// �������� ��������� ���� ��������� pi
			type_category get_category(const file_info& fi, const parameter_info& pi);

			// �������� ��� xml-���� ��������� pi (��������, unit->str, array<double>->array<dbl>)
			std::string get_type_xml(const file_info& fi, const parameter_info& pi);

			// �������� ��� class-���� ��������� pi (��������, user_enum_name->user_enum_name, array<double>->std::vector<double>)
			std::string get_type_class(const file_info& fi, const parameter_info& pi);

			// �������� ��� ���� ��������� pi, � ���� �������� - ������, �� ��� ���� �������� �������
			// (��������, unit->unit, array<double>->double)
			std::string get_item_type(const parameter_info& pi);

			// �������� ��� xml-���� ��������� pi, � ���� �������� - ������, �� ��� xml-���� �������� �������
			// (��������, unit->str, array<double>->dbl)
			std::string get_item_type_xml(const file_info& fi, const parameter_info& pi);

			// �������� ��� cpp-���� ��������� pi, � ���� �������� - ������, �� ��� cpp-���� �������� �������
			// (��������, enum->std::string, array<double>->double)
			std::string get_item_type_cpp(const file_info& fi, const parameter_info& pi);

			// �������� ��� class-���� ��������� pi, � ���� �������� - ������, �� ��� class-���� �������� �������
			// (��������, user_enum_name->user_enum_name, array<double>->double)
			std::string get_item_type_class(const file_info& fi, const parameter_info& pi);



			// �������� ����, �������� �� �������� pi ��������
			bool get_is_array(const parameter_info& pi);

			// �������� ����, �������� �� �������� pi ������������
			bool get_is_required(const parameter_info& pi);

			// �������� ����, �������� �� �������� pi ������������
			bool get_is_optional(const parameter_info& pi);

			// �������� ����, ����� �� �������� pi �����������
			bool get_has_restrictions(const parameter_info& pi);



			//
			std::string get_display_name(const parameter_info& pi);

			//
			variant get_initial_value(const file_info& fi, const parameter_info& pi, bool is_item);

			//
			bool compare(const parameter_info& pi1, const parameter_info& pi2, std::string message);













			// �������������� ������� ������� � ������

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

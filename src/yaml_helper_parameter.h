#pragma once

#include "yaml_types.h"

namespace yaml
{
	namespace helper
	{
        // ������ QVariant
        class variant
        {
        private:
            base_types t_;
            std::string s_;
            int i_;
            double d_;
            bool b_;

        public:
            variant() :
                t_(base_types::none),
                s_(""),
                i_(0),
                d_(0.0),
                b_(false)
            {}

            explicit variant(std::string s) :
                variant()
            {
                t_ = base_types::string;
                s_ = s;
            }

            explicit variant(int i) :
                variant()
            {
                t_ = base_types::integer;
                i_ = i;
            }

            explicit variant(double d) :
                variant()
            {
                t_ = base_types::floating;
                d_ = d;
            }

            explicit variant(bool b):
                variant()
            {
                t_ = base_types::bool_;
                b_ = b;
            }

            base_types get_type() const
            {
                return t_;
            }

            template <typename T>
            T get() const
            {
                switch (t_)
                {
                case yaml::base_types::string:
                    return {}; // specialization
                case yaml::base_types::integer:
                    return static_cast<T>(i_);
                case yaml::base_types::floating:
                    return static_cast<T>(d_);
                case yaml::base_types::bool_:
                    return static_cast<T>(b_);
                default:
                    return {};
                }
            }

            template <>
            std::string get<std::string>() const
            {
                if (t_ == yaml::base_types::string)
                    return s_;
                else
                    return {};
            }
        };

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














			// �������������� ������� ������� � ������

			//
			std::vector<parameter_info>* get_parameters(file_info& fi, const std::string& type);
			
			//
			parameter_info* get_parameter_info(file_info& fi, const std::string& type, const std::string& name);

		}
	}
}

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace parameters
{
	enum class struct_types
	{
		file_info,
		info_info,
		type_info,
		parameter_info,
		restrictions_info
	};

	enum class type_category
	{
		system,
		cpp,
		user_yml,
		user_cpp
	};

	// Базовые типы данных для классификации
	enum class base_types
	{
		none,
		string,
		integer,
		floating,
		boolean,
		array,
		user_cpp // enum only, user_yml must be array
	};

	enum class base_item_types
	{
		none,
		string,
		integer,
		floating,
		boolean,
		user // user_cpp or user_yml
	};

	enum class type_types
	{
		yml,
		enum_
	};

	enum class system_types
	{
		unit,
		path,
		string,
		library
	};

	//enum class parameter_types
	//{
	//	unit, path, string, library,
	//	double_, int_, bool_, std_string, float_, int8_t, int16_t,
	//	int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t,
	//	array_unit, array_path, array_string, array_library,
	//	array_double, array_int, array_bool, array_std_string, array_float, array_int8_t, array_int16_t,
	//	array_int32_t, array_int64_t, array_uint8_t, array_uint16_t, array_uint32_t, array_uint64_t
	//};

	enum class cpp_types
	{
		double_, int_, bool_, std_string, float_,
		int8_t, int16_t, int32_t, int64_t,
		uint8_t, uint16_t, uint32_t, uint64_t
	};

	enum class xml_types
	{
		dbl,
		int_,
		bool_,
		str,
		lib
	};

	struct restrictions_info
	{
		// Optional members from yml
		std::string min;
		std::string max;
		std::vector<std::string> set_;
		std::string min_count;
		std::string max_count;
		std::vector<std::string> set_count;
		std::string category;
		std::vector<std::string> ids;
		std::string max_length;
	};

	struct parameter_info
	{
		// Required members from yml
		std::string type;
		std::string name;
		// Optional members from yml
		std::string required;
		std::string default_;
		std::string display_name;
		std::string description;
		std::string hint;
		restrictions_info restrictions;
	};

	struct type_info
	{
		// Required members from yml
		std::string name;
		// Optional members from yml
		std::string type;
		std::string description;
		std::vector<std::pair<std::string, std::string>> values;
		std::vector<std::string> includes;
		std::vector<parameter_info> parameters;
	};

	struct info_info
	{
		// Required members from yml
		std::string id;
		// Optional members from yml
		std::string display_name;
		std::string description;
		std::string category;
		std::string hint;
		std::string pictogram;
		std::string author;
		std::string wiki;
	};

	struct file_info
	{
		// Optional members from yml
		std::string format;
		// Required members from yml
		info_info info;
		std::vector<type_info> types;
		std::vector<parameter_info> parameters;
	};

	// Замена QVariant
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

		explicit variant(bool b) :
			variant()
		{
			t_ = base_types::boolean;
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
			case parameters::base_types::string:
				return {}; // specialization
			case parameters::base_types::integer:
				return static_cast<T>(i_);
			case parameters::base_types::floating:
				return static_cast<T>(d_);
			case parameters::base_types::boolean:
				return static_cast<T>(b_);
			default:
				return {};
			}
		}

		template <>
		std::string get<std::string>() const
		{
			if (t_ == parameters::base_types::string)
				return s_;
			else
				return {};
		}
	};
}
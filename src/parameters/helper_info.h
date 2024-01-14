#pragma once

#include "parameters/types.h"

namespace parameters
{
	namespace helper
	{
		namespace info
		{
			// Получить версию формата файла
			bool compare(const info_info& ii1, const info_info& ii2, std::string& message);
		}
	}
}

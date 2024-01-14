#pragma once

#include "definitions.h"
#include "parameters/types.h"
#include "parameters/helper_info.h"

using namespace parameters;
using namespace parameters::helper;

bool info::compare(const info_info& ii1, const info_info& ii2, std::string& message)
{
	message = "ok";

	// Required members from yml
	if (ii1.id != ii2.id)
	{
		message = "Поле info_info.id не совпадает";
		return false;
	}

	// Optional members from yml
	if (ii1.display_name != ii2.display_name)
	{
		message = "Поле info_info.display_name не совпадает";
		return false;
	}

	if (ii1.description != ii2.description)
	{
		message = "Поле info_info.description не совпадает";
		return false;
	}

	if (ii1.category != ii2.category)
	{
		message = "Поле info_info.category не совпадает";
		return false;
	}

	if (ii1.hint != ii2.hint)
	{
		message = "Поле info_info.hint не совпадает";
		return false;
	}

	if (ii1.pictogram != ii2.pictogram)
	{
		message = "Поле info_info.pictogram не совпадает";
		return false;
	}

	if (ii1.author != ii2.author)
	{
		message = "Поле info_info.author не совпадает";
		return false;
	}

	if (ii1.wiki != ii2.wiki)
	{
		message = "Поле info_info.wiki не совпадает";
		return false;
	}

	return true;
}

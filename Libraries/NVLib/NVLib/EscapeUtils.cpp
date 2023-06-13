//--------------------------------------------------
// Implementation of class EscapeUtils
//
// @author: Wild Boar
//
// @date: 2021-10-14
//--------------------------------------------------

#include "EscapeUtils.h"
using namespace NVLib;

//--------------------------------------------------
// MySQLEscape
//--------------------------------------------------

/**
 * @brief Escape strings for putting into MySQL queries
 * @param value The value that we are escaping
 * @return string Returns a string
 */
string EscapeUtils::MySqlEscape(const string& value)
{
	auto result = stringstream();

	for (auto& character : value) 
	{
		if (character == '\'') result << "\\'";
		else if (character == '"') result << "\\\"";
		else result << character;
	}

	return result.str();
}

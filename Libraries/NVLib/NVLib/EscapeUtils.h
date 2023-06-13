//--------------------------------------------------
// Utility: Functionality for escaping characters
//
// @author: Wild Boar
//
// @date: 2021-10-14
//--------------------------------------------------

#pragma once

#include <sstream>
#include <iostream>
using namespace std;

namespace NVLib
{
	class EscapeUtils
	{
	public:
		static string MySqlEscape(const string& value);
	};
}

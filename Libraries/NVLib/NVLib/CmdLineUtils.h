//--------------------------------------------------
// Utility: CmdLineUtils functions for calling cmdline applications
//
// @author: Wild Boar
//
// @date: 2021-08-31
//--------------------------------------------------

#pragma once

#include <sstream>
#include <memory>
#include <array>
#include <iostream>
using namespace std;

namespace NVLib
{
	class CmdLineUtils
	{
		public:
			static string Execute(const string& command);
	};
}

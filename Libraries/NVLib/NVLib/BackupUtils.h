//--------------------------------------------------
// Utility: A tool for backing up files
//
// @author: Wild Boar
//
// @date: 2021-09-05
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include "FileUtils.h"

namespace NVLib
{
	class BackupUtils
	{
		public:
			static void Backup(const string& folder, const string& filePath);
	};
}

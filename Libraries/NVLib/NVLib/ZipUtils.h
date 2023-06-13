//--------------------------------------------------
// A utility that allows archiving and unarchiving (using libzip)
//
// @author: Wild Boar
//
// @date: 2022-01-06
//--------------------------------------------------

#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
using namespace std;

#include "FileUtils.h"

#include <zip.h>

namespace NVLib
{
	class ZipUtils
	{
	public:
		static void Zip(const string& zipFile, const string& folder);
		static void UnZip(const string& zipFile, const string& folder);
	private:
		static void InsertFolder(zip_t * handle, const string& folder);
		static void ExtractFolder(zip_t * handle, const string& folder);
	};
}

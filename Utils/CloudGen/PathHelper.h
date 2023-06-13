//--------------------------------------------------
// Add the logic to assist with the construction of the paths within the system
//
// @author: Wild Boar
//
// @date: 2023-03-28
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/FileUtils.h>
#include <NVLib/Parameters/Parameters.h>

#include "ArgReader.h"

namespace NVL_App
{
	class PathHelper
	{
	private:
		string _database;
		string _dataset;
	public:
		PathHelper(NVLib::Parameters * parameters);

		string GetFrameFolder();
		string GetMetaFolder();
		string GetModelFolder();
		string GetPoseFolder();

		inline string& GetDatabase() { return _database; }
		inline string& GetDataset() { return _dataset; }

	private:
		string GetPath(const string& folder);
	};
}

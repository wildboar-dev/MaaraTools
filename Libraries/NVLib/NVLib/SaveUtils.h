//--------------------------------------------------
// Utility: functions to saving things to disk
//
// @author: Wild Boar
//
// @date: 2022-03-27
//--------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Model/Model.h"

namespace NVLib
{
	class SaveUtils
	{
	public:
		static void SaveModel(const string& path, Model * model);
	};
}

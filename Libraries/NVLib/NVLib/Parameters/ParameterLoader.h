//--------------------------------------------------
// Utility: Loads the given parameters from disk
//
// @author: Wild Boar
//
// @date: 2022-02-19
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Parameters.h"

namespace NVLib
{
	class ParameterLoader
	{
	public:
		static Parameters * Load(const string& path);
	};
}

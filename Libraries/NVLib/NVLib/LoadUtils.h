//--------------------------------------------------
// A set of utilities for loading things
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Model/StereoFrame.h"
#include "Model/StereoCalibration.h"
#include "Model/MonoCalibration.h"
#include "Model/DepthFrame.h"

namespace NVLib
{
	class LoadUtils
	{
	public:
		static Mat LoadImage(const string& path);
		static Mat LoadMask(const string& path);
		static Mat LoadGray(const string& path);
		static StereoFrame * LoadStereoFrame(const string& left, const string& right);
		static StereoCalibration * LoadStereoCalibration(const string& path);
		static MonoCalibration * LoadCalibration(const string& path);
		static DepthFrame * LoadDepthFrame(const string& color, const string& depth);
	};
}

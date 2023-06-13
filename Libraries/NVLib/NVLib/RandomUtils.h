//--------------------------------------------------
// Generate random numbers for the purpose of generating training data
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#include <opencv2/opencv.hpp>

#include "Model/Range.h"

#include "PoseUtils.h"

namespace NVLib
{
	class RandomUtils
	{
	public:
		static cv::Mat GetKMatrix(const Range<double> & focal, const cv::Size & imageSize, const Range<double>& centerOffset, bool sameFocal);
		static cv::Mat GetPoseMatrix(const Range<double>&  rx, const Range<double>& ry, const Range<double>& rz, const Range<double>& tx, const Range<double> ty, const Range<double>& tz);
		static cv::Mat GetDistortionMatrix(const Range<double> & k1, const Range<double> & k2, const Range<double> & p1, const Range<double>& p2);
		static double GetValue(const Range<double>& range);
		static Point GetRandomPoint(const Rect region);
		static void TimeSeedRandomNumbers();
		static bool GetBinaryChoice(double probTrue);
		static int GetInteger(const NVLib::Range<int>& range);
		static int GetInteger(int min, int max);
	};
}

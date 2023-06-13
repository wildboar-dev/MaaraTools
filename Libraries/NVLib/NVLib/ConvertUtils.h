//--------------------------------------------------
// Common conversions
//
// @author: Wild Boar
//
// @date: 2021-08-27
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class ConvertUtils
	{
		public:
			static void ConvertPoints(vector<Point2d>& input, vector<Point2f>& output);
			static void ConvertPoints(vector<Point2d>& input, vector<Point>& output);
			static void ConvertPoints(vector<Point>& input, vector<Point2d>& output);
	};
}

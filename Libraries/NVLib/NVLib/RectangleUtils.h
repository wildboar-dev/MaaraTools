//--------------------------------------------------
// A set of utilities associated with projective rectangles and projective rectangle masks.
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "DisplayUtils.h"

namespace NVLib
{
	class RectangleUtils
	{
	public:
		static void GetBestFitCorners(const Size& fitSize, double aspectRatio, vector<Point2d>& outPoints);
		static void GetMaskCorners(Mat& mask, vector<Point>& outCorners);
		static bool ApproxPoly(std::vector<cv::Point> contours, std::vector<cv::Point>& rects, double minepsilon, double maxepsilon, int sides);
		static void OrderPoints(vector<Point>& input, vector<Point>& output);
	};
}

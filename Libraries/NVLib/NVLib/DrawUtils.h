//--------------------------------------------------
// Defines a library of drawing utilities
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "PoseUtils.h"

namespace NVLib
{
	class DrawUtils
	{
	public:
		static void DrawPoints(Mat& image, vector<Point2d>& points, int radius, const Scalar& color);
		static void DrawFilledRect(Mat& image, const Rect& rectangle, const Scalar& color);
		static Mat GetRandomGray(const Size& size);
		static void DrawPolyOutline(Mat& image, const Scalar& color, vector<Point>& points);
		static Mat GetPolyMask(const Size& imageSize, vector<Point>& points);
		static Mat GetRectangleMask(const Size& imageSize, const Rect& rect);
		static Mat OverlayMask(Mat& image, Mat& overlay, const Scalar& color);
		static Mat ExtractMask(Mat& image, Mat& mask);
		static Mat DrawDirectionMap(Mat& direction);
		static Mat DrawEightBit(Mat& floatImage);
	};
}

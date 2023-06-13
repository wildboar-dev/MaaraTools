//--------------------------------------------------
// A set of math utilities for 2D
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Line2d.h"

namespace NVLib
{
	class Math2D
	{
	public:
		static double GetDistance(const Point & point1, const Point & point2);
		static double GetDistance(const Point2d& point1, const Point2d& point2);
		static int GetLongestLineIndex(vector<Point2d>& polygon, bool closed = true);
		static Point2d GetLineIntercept(Line2d& line1, Line2d& line2);
		static Point2d Transform(Mat& H, const Point2d& point);
		static double GetPointLineDistance(Line2d& line, Point2d& point);
		static double GetMagnitude(const Vec2d& vector);
		static Vec2d Normalize(const Vec2d& vector);
		static Vec2d GetNGradient(const Point2d& point1, const Point2d& point2);
	};
}

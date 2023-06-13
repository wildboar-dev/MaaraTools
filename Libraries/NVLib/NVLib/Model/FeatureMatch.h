//--------------------------------------------------
// Defines a set of corresponding features
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class FeatureMatch
	{
	private:
        Point2d _point1;
        Point2d _point2;
	public:
		FeatureMatch(Point2d& point1, Point2d& point2) : _point1(point1), _point2(point2) {}
		FeatureMatch(Point2f& point1, Point2f& point2) : _point1(point1), _point2(point2) {}

		inline Point2d& GetPoint1()  { return _point1; }
		inline Point2d& GetPoint2() { return _point2; }
	};
}

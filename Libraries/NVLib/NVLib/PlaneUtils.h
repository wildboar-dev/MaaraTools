//--------------------------------------------------
// A set of utilities associated with a plane
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Math3D.h"

namespace NVLib
{
	class PlaneUtils
	{
	public:
		static Vec4d FitPlane(vector<Point3d> & points);
		static Vec2d AvePlaneError(const Vec4d& plane, const vector<Point3d>& points);
		static Point3d ProjectPoint(const Vec4d& planeParameters, const Point3d& point);
		static Mat FindAxis(const Vec4d& plane);
		static Point2d Convert2d(Mat& axes, Point3d& origin, Point3d& point);
		static Point3d Convert3d(Mat& axes, Point3d& origin, Point2d& point);
		static void BuildPlane(const Vec4d& plane, vector<Point3d>& points, const Range& rangeX, const Range& rangeY, int stepCount);
	};
}

//--------------------------------------------------
// A container for holding the details of the a line
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
	class Line2d
	{
	private:
		Point2d _start;
		Point2d _end;
		Vec2d _gradient;
		double _magnitude;
	public:
		Line2d(const Point2d& start, const Point2d& end) 
		{
			_start = start; _end = end;

			auto vector = Vec2d(_end.x - _start.x, _end.y - _start.y);
			_magnitude = sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
		
			_gradient = Vec2d(vector[0] / _magnitude, vector[1] / _magnitude);
		}

		Line2d(Point& start, Point& end) : Line2d(Point2d(start.x, start.y), Point2d(end.x, end.y)) {}

		Line2d(Vec4d& params) : Line2d(Point2d(params[2], params[3]), Point2d(params[2] + params[0], params[3] + params[1])) {}

		inline Point2d& GetStart() { return _start; }
		inline Point2d& GetEnd() { return _end; }
		inline Vec2d& GetGradient()  { return _gradient; }
		inline double& GetMagnitude() { return _magnitude; }
	};
}

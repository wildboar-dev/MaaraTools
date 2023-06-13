//--------------------------------------------------
// Model: Color Point is a 3D point that contains location and color information
//
// @author: Wild Boar
//
// @date: 2022-02-15
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class ColorPoint
	{
		private:
			Point3d _location;
			Vec3d _color;
		public:

			ColorPoint(double x, double y, double z, int red, int green, int blue) :
				_location(Point3d(x,y,z)), _color(red, green, blue) {}

			ColorPoint(const Point3d& location, const Vec3i& color) :
				_location(location), _color(color) {}

			inline Point3d& GetLocation() { return _location; }
			inline Vec3d& GetColor() { return _color; }
	};
}

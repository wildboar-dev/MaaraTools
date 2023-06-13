//--------------------------------------------------
// Model: The graph series that we are adding to the system
//
// @author: Wild Boar
//
// @date: 2022-02-07
//--------------------------------------------------

#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include "opencv2/opencv.hpp"
using namespace cv;

#include "GraphSeries.h"

namespace NVLib
{
	class GraphSeries
	{
	private:
		string _name;
		Vec3i _color;
		vector<Point2d> _values;
	public:
		GraphSeries(const string& name, const Vec3i& color, vector<Point2d>& values) :
			_name(name), _color(color), _values(values) {}

		inline string GetName() { return _name; }
		inline Vec3i GetColor()  { return _color; }
		inline vector<Point2d> GetValues() { return _values; }
	};
}

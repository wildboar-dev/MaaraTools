//--------------------------------------------------
// Defines a 3D model with the system
//
// @author: Wild Boar
//
// @date: 2022-02-15
//--------------------------------------------------

#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include "ColorPoint.h"

namespace NVLib
{
	class Model
	{
		private:
			vector<ColorPoint> _vertices;

		public:
			Model();

			void AddVertex(ColorPoint& vertex);
			void AddVertex(const Point3d& vertex, const Vec3i& color);
			void AddVertices(vector<Point3d>& points, const Vec3i& color);
			void Transform(Mat& transform);

			int VertexCount();

			inline vector<ColorPoint>& GetVertices() { return _vertices; }
	};
}

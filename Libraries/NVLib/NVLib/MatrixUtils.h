//--------------------------------------------------
// A set of helper methods for Matrices
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
	class MatrixUtils
	{
	public:
		static void Matrix2Vector(Mat& matrix, vector<double>& output);
		static Mat Vector2Matrix(int rows, int columns, vector<double>& data);
	};
}

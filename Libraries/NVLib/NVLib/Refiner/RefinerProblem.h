//--------------------------------------------------
// A template for a refiner problem
//
// @author: Wild Boar
//
// @date: 2022-04-07
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class RefinerProblem
	{
	public:
		virtual double GetError(Mat& params, int problemId) = 0;
		virtual int GetTrainingSize() = 0;
	};
}
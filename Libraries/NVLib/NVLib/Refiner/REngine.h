//--------------------------------------------------
// A refiner engine based on the Gauss Newton approach
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

#include "RefinerProblem.h"

namespace NVLib
{
	class REngine
	{
	private:
		RefinerProblem * _problem;
		double _epsilon;
	public:
		REngine(RefinerProblem * problem, double epsilon=1e-8) : _problem(problem), _epsilon(epsilon) {}
		virtual ~REngine() { delete _problem; }

		Mat GetJacobian(Mat& params, double baseError, int problemId);
		Mat GetErrors(Mat& params);
		Mat Iterate(Mat& params, Mat& errors);
		Vec2d Minimize(Mat& params, int maxIterations = 1000, double minError = 1e-3);

		Vec2d GetAveError(Mat& errors);

		inline RefinerProblem *& GetProblem() { return _problem; }
		inline double& GetEpsilon() { return _epsilon; }
	};
}

//--------------------------------------------------
// Defines a set of utilities for helping with stereo matching
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Model/FeatureMatch.h"
#include "Math3D.h"

namespace NVLib
{
	class StereoUtils
	{
	public:
		static Mat GetFMatrix(const vector<FeatureMatch>& matches);
		static Vec2d FindFError(Mat& F, vector<FeatureMatch>& matches, vector<uchar>& mask);
		static double GetSampsonError(const Mat& F, FeatureMatch& match);
		static Point2d GetMatchPoint(Mat& camera, Mat& pose, const Point2d& point, double Z);
	private:
		static double GetSquared(const Mat& matrix, int row, int column);
		static Point2d ApplyH(const Mat& H, const Point2f& point);
	};
}

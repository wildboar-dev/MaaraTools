//--------------------------------------------------
// A set of utilities for displaying various things within the project.
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Model/StereoFrame.h"
#include "Model/FeatureMatch.h"
#include "Model/DepthFrame.h"
#include "Model/StereoCalibration.h"

namespace NVLib
{
	class DisplayUtils
	{
	public:
		static void ShowImage(const string& caption, Mat& image, int width);
		static void ShowFloatMap(const string& caption, Mat& map, int width);\
		static void ShowFeatures(const string& caption, Mat& image, vector<Point2d>& features, int circleRadius, const Vec3i& color, int width);
		static void ShowStereoFeatures(const string& caption, StereoFrame* frame, int width, vector<FeatureMatch>& matches, const Vec3i& color);
		static void ShowToggleImages(string caption, Mat& image1, Mat& image2, int width);
		static void ShowStereoFrame(const string& caption, StereoFrame& frame, int width);
		static void ShowDepthFrame(const string& caption, DepthFrame* frame, int width);
		static void ShowDepthFrame(const string& caption, DepthFrame* frame, double minDepth, double maxDepth, int width);
		static void ShowStereoFeatures(const string& caption, StereoFrame* frame, int width, vector<Point>& leftPoints, vector<Point>& rightPoints, const Vec3b& color);
	
		static void ShowStereoCalibration(StereoCalibration * calibration);
	};
}

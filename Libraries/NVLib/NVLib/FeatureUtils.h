//--------------------------------------------------
// Defines a set of utilities for helping with stereo matching
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Model/StereoFrame.h"
#include "Model/FeatureMatch.h"
#include "Model/DepthFrame.h"

#include "Math3D.h"
#include "PoseUtils.h"

namespace NVLib
{
	class FeatureUtils
	{
	public:
		static void Find(Mat& image, int blockSize, vector<Point2d>& result);
		static void Match(StereoFrame* frame, vector<Point2d>& points, vector<FeatureMatch>& result);
		static void GetScenePoints(Mat& camera, DepthFrame * frame, vector<FeatureMatch>& matches, vector<Point3d>& outScene, vector<Point2d>& outImage);
		static Mat FindPose(Mat& camera, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
		static double FindPoseError(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
	private:
		static int GetIndex(const Point2d& point, int blockSize, int width);
		static void EpipolarFilter(vector<FeatureMatch>& input, vector<FeatureMatch>& output);
	};
}
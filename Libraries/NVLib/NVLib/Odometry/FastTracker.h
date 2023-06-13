//--------------------------------------------------
// Defines a tracker that uses "FAST" features for pose estimation
//
// @author: Wild Boar
//
// @date: 2022-06-04
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "../PoseUtils.h"
#include "../DisplayUtils.h"
#include "../Model/DepthFrame.h"
#include "../Model/StereoFrame.h"
#include "../Model/FeatureMatch.h"

#include "FastDetector.h"

namespace NVLib
{
	class FastTracker
	{
	private:
		Mat _camera;
		NVLib::DepthFrame * _frame;
		vector<KeyPoint> _keypoints;
		FastDetector * _detector;
	public:
		FastTracker(Mat& camera, NVLib::DepthFrame * firstFrame);
		~FastTracker();

		Mat GetPose(NVLib::DepthFrame * frame, vector<KeyPoint>& keypoints, Vec2d& error);

		void UpdateNextFrame(NVLib::DepthFrame * frame, vector<KeyPoint>& keypoints, bool free);

		inline NVLib::DepthFrame *& GetFrame() { return _frame; }
		inline vector<KeyPoint>& GetKeypoints() { return _keypoints; }
	private:
		Mat FindPoseProcess(vector<KeyPoint>& keypoints_2, vector<MatchIndices *>& matches, Vec2d& error);
		void GetScenePoints(Mat& camera, Mat& depth, vector<MatchIndices *>& matches, vector<KeyPoint>& keypoints, vector<Point3f>& out);
		void GetImagePoints(vector<KeyPoint>& keypoints, vector<MatchIndices *>& matches, vector<Point2f>& out);
		void FilterBadDepth(vector<Point3f>& scenePoints, vector<Point2f>& imagePoints);
		Mat EstimatePose(Mat& camera, vector<Point3f>& scenePoints, vector<Point2f>& imagePoints);	
		void EstimateError(Mat& camera, Mat& pose, vector<Point3f>& scenePoints, vector<Point2f>& imagePoints, Vec2d& error);

		float ExtractDepth(Mat& depth, const Point2f& location);
		void ShowMatchingPoints(NVLib::StereoFrame& frame, vector<MatchIndices *>& matches, vector<KeyPoint>& keypoints_1, vector<KeyPoint>& keypoints_2);
	};
}

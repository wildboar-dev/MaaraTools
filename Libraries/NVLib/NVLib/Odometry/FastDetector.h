//--------------------------------------------------
// Defines a detector that uses FAST
//
// @author: Wild Boar
//
// @date: 2022-06-03
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "../FeatureUtils.h"
#include "../Model/StereoFrame.h"

#include "MatchIndices.h"

namespace NVLib
{
	class FastDetector
	{
	private:
		int _blockSize;
		NVLib::StereoFrame * _frame;
	public:
		FastDetector(int blockSize) : _blockSize(blockSize) { _frame = nullptr; }
		~FastDetector() { if (_frame != nullptr) delete _frame; }

		void Extract(Mat& image, vector<KeyPoint>& keypoints); 
		void Match(vector<KeyPoint>& kp_1, vector<KeyPoint>& kp_2, vector<MatchIndices *>& output, double threshold = 1.0);

		void SetFrame(Mat& image1, Mat& image2);
	private:
		int GetIndex(const Point2d& point, int blockSize, int width);
		void FindMatches(vector<Point2f>& pointSet1, vector<Point2f>& pointSet2, vector<MatchIndices *>& matches, double threshold);
		void FilterOnError(vector<uchar>& status, vector<float>& errors, vector<MatchIndices *>& matches);
		void EpipolarFilter(vector<Point2f>& pointSet1, vector<Point2f>& pointSet2, vector<MatchIndices *>& output);
		Mat BuildPointDescriptor(vector<Point2f>& points);
	};
}

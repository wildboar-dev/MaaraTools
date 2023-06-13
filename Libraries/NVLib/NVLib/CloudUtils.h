//--------------------------------------------------
// A set of utilities wrt dealing with point clouds
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Math3D.h"

namespace NVLib
{
	class CloudUtils
	{
	public:
		static Mat BuildColorCloud(Mat & camera, Mat& color, Mat& depth);
		static Mat SampleCloud(Mat& colorCloud, int step = 1);
		static Mat RenderImage(Mat& colorCloud, Mat& camera, Mat& pose, int step = 1);
		static Mat TransformCloud(Mat& colorCloud, Mat& pose);
		static Mat ProjectImagePoints(Mat& camera, Mat& cloud);
		static int GetVertexCount(Mat& colorCloud);
		static void Save(const string& path, Mat& colorCloud);
	};
}

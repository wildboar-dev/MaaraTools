//--------------------------------------------------
// Model: Defines a model for holding a stereo calibration
//
// @author: Wild Boar
//
// @date: 2022-03-22
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class StereoCalibration
	{
	private:
		Mat _camera1;
		Mat _distortion1;
		Mat _camera2;
		Mat _distortion2;
		Mat _pose;
		Size _imageSize1;
		Size _imageSize2;
	public:
		StereoCalibration(Mat& camera1, Mat& distortion1, Mat& camera2, Mat& distortion2, Mat& pose, Size& imageSize1, Size& imageSize2) :
			_camera1(camera1), _distortion1(distortion1), _camera2(camera2), _distortion2(distortion2), _pose(pose), _imageSize1(imageSize1), _imageSize2(imageSize2) {}

		inline Mat& GetCamera1() { return _camera1; }
		inline Mat& GetDistortion1() { return _distortion1; }
		inline Mat& GetCamera2() { return _camera2; }
		inline Mat& GetDistortion2() { return _distortion2; }
		inline Mat& GetPose() { return _pose; }
		inline Size& GetImageSize1() { return _imageSize1; }
		inline Size& GetImageSize2() { return _imageSize2; }

		inline bool Validate() 
		{
			if (_camera1.empty()) return false;
			else if (_camera2.empty()) return false;
			else if (_distortion1.empty()) return false;
			else if (_distortion2.empty()) return false;
			else if (_imageSize1.width == 0 || _imageSize1.height == 0) return false;
			else if (_imageSize2.width == 0 || _imageSize2.height == 0) return false;
			return true;
		}

	};
}
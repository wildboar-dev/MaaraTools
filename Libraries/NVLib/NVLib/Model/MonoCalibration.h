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
	class MonoCalibration
	{
	private:
		Mat _camera;
		Mat _distortion;
		Size _imageSize;
	public:
		MonoCalibration(Mat& camera, Mat& distortion, Size& imageSize) :
			_camera(camera), _distortion(distortion), _imageSize(imageSize) {}

		inline Mat& GetCamera() { return _camera; }
		inline Mat& GetDistortion() { return _distortion; }
		inline Size& GetImageSize() { return _imageSize; }
	};
}
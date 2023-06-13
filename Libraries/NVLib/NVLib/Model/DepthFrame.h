//--------------------------------------------------
// A container for holding depth frames
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class DepthFrame
	{
	private:
        Mat _color;
        Mat _depth;
	public:
		DepthFrame(Mat& color, Mat& depth) : _color(color), _depth(depth) {}
        DepthFrame(DepthFrame& frame) : _color(frame._color), _depth(frame._depth) {}
		inline Mat& GetColor()  { return _color; }
		inline Mat& GetDepth() { return _depth; }
	};
}
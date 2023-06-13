//--------------------------------------------------
// A container for holding stereo frames
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
	class StereoFrame
	{
	private:
        Mat _left;
        Mat _right;
	public:
		StereoFrame(Mat& left, Mat& right) : _left(left), _right(right) 
		{ 
			assert(left.rows == right.rows && left.cols == right.cols); 
		}
		
		inline Mat& GetLeft()  { return _left; }
		inline Mat& GetRight() { return _right; }
		inline Size GetSize() { return _left.size(); }
	};
}

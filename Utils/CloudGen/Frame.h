//--------------------------------------------------
// The frame that we are adding
//
// @author: Wild Boar
//
// @date: 2023-03-25
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class Frame 
	{
	private:
		int _index;
		Mat _color;
		Mat _depth;
	public:
		Frame (int index, Mat& color, Mat& depth) :
			_index(index), _color(color), _depth(depth) {}

		inline int& GetIndex() { return _index; }
		inline Mat& GetColor() { return _color; }
		inline Mat& GetDepth() { return _depth; }
	};
}

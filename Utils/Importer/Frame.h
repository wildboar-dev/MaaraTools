//--------------------------------------------------
// The frame that we are dealing with
//
// @author: Wild Boar
//
// @date: 2023-03-02
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
		Mat _camera;
		Mat _pose;
		Mat _color;
		Mat _depth;
	public:
		Frame(int index, Mat& camera, Mat& pose, Mat& color, Mat& depth) :
			_index(index), _camera(camera), _pose(pose), _color(color), _depth(depth) {}

		inline int& GetIndex() { return _index; }
		inline Mat& GetCamera() { return _camera; }
		inline Mat& GetPose() { return _pose; }
		inline Mat& GetColor() { return _color; }
		inline Mat& GetDepth() { return _depth; }
	};
}

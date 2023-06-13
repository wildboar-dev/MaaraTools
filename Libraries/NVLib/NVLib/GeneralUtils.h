//--------------------------------------------------
// Contains a set of general operation
//
// @author: Wild Boar
//
// @date: 2023-03-09
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class GeneralUtils
	{
	public:
		static void Swap(float& value1, float& value2);
	};
}

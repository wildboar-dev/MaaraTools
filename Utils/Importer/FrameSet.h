//--------------------------------------------------
// The frameset that we are dealing with
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

#include <yaml-cpp/yaml.h>

#include <NVLib/FileUtils.h>
#include <NVLib/StringUtils.h>
#include <NVLib/PoseUtils.h>

#include "Frame.h"

namespace NVL_App
{
	class FrameSet
	{
	private:
		int _currentIndex;
		string _folder;
		int _frameCount;
		int _startIndex;
	public:
		FrameSet(const string& folder);

		Frame * GetFrame(int index);
		Frame * GetNext();
		Mat GetPose(int index);
		Mat GetK(int index);
		Mat GetDepth(int index);
		Mat GetColor(int index);

		inline void Reset() { _currentIndex = _startIndex; }
	
		inline int& GetStartIndex() { return _startIndex; }
		inline int& GetLastIndex() { return _currentIndex; }
		inline int& GetFrameCount() { return _frameCount; }
	private:
		int InitFrameCount(const string& folder);
	};
}

//--------------------------------------------------
// Model: The given PnP point for testing
//
// @author: Wild Boar
//
// @date: 2022-04-11
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class PnPoint
	{
    private:
        Point3d _scenePoint;
        Point2d _imagePoint;
    public:
        PnPoint(const Point3d& scenePoint, const Point2d& imagePoint) : 
            _scenePoint(scenePoint), _imagePoint(imagePoint) {}

        inline Point3d& GetScenePoint() { return _scenePoint; }
        inline Point2d& GetImagePoint() { return _imagePoint; }
	};
}

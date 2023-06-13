//--------------------------------------------------
// Color Definitions
//
// @author: Wild Boar
//
// @date: 2021-08-27
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib 
{
    class Colors 
    {
    public:
        static const Vec3i RED;
        static const Vec3i GREEN;
        static const Vec3i BLUE;
        static const Vec3i BLACK;
        static const Vec3i WHITE;

        static Scalar Convert(const Vec3i& color); 
    };
}

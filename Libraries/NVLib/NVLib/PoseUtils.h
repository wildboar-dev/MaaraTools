//--------------------------------------------------
// A set of utilities for controlling the pose.
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#if !defined(_USE_MATH_DEFINES)
	#define _USE_MATH_DEFINES
#endif

#include <cmath> 

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class PoseUtils
	{
	public:
		static Mat Vectors2Pose(const Vec3d & rvec, const Vec3d & tvec);
		static void Pose2Vectors(Mat & pose, Vec3d & rvec, Vec3d & tvec);
		static Vec3d GetPoseTranslation(Mat& pose);
		static Mat GetPoseRotation(Mat& pose);
		static Vec4d NormalizeQuaternion(const Vec4d& quaternion);
		static Mat Quaternion2Matrix(const Vec4d& quaternion);
		static Vec4d Matrix2Quaternion(Mat& R);
		static Mat Euler2Matrix(const Vec3d& angles);
		static Vec3d Matrix2Euler(Mat& R);
		static Mat GetPose(Mat& rotation, Vec3d& translation);
		static double Degree2Radian(double degrees);
		static double Radian2Degree(double radians);

		static Mat CreateTPose(const Vec3d& translation);
		static Mat CreateRX(double radians);
		static Mat CreateRY(double radians);
		static Mat CreateRZ(double radians);
	};
}

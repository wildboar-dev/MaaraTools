//--------------------------------------------------
// A set of utilities for performing 3D maths
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
	class Math3D
	{
	public:
		static Point2d Project(const Mat& cameraMatrix, const Point3d& point);
		static Point3d UnProject(const Mat& cameraMatrix, const Point2d& point, double Z);
		static Point3d TransformPoint(const Mat& pose, const Point3d & point);
		static double ExtractDepth(Mat& depthMap, const Point2d & position);
		static Vec3i ExtractColor(Mat& colorMap, const Point2d& position);
		static Mat ExtractK(Mat& Q);
		static Mat BuildKMatrix(double f, const Size& imageSize);
		static void GetViewLimits(Mat& cameraMatrix, const Size& imageSize, double zmin, double zmax, vector<Point3d>& output);
		static void TransformPointSet(const Mat& pose, const vector<Point3d>& input, vector<Point3d>& output);
		static Vec3d NormalizeVector(const Vec3d& vector);
		static Mat GetOrientationRotation(const Vec3d& newNormal, const Vec3d& oldNormal);
		static Point3d RotatePoint(Mat& rotation, const Point3d& point);
		static Vec3d RotateVector(Mat& rotation, const Vec3d& vector);
		static double GetDistance(const Point3d& point1, const Point3d& point2);
		static Vec6d GetCloudBounds(vector<Point3d>& points);
		static double GetLinePointDistance(const Point3d& start, const Vec3d& gradient, const Point3d& point);
		static double GetMagnitude(const Vec3d& vector);
	};
}

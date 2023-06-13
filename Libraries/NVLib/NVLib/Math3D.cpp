//--------------------------------------------------
// Implementation code for Math3D
//
// @author: Wild Boar
//--------------------------------------------------

#include "Math3D.h"
using namespace NVLib;

//--------------------------------------------------
// Project
//--------------------------------------------------

/**
 * Project a point based on the associated camera matrix
 * @param cameraMatrix The camera matrix that we are basing our project upon
 * @param point The point that we are projecting
 */
Point2d Math3D::Project(const Mat& cameraMatrix, const Point3d& point) 
{
	auto cdata = (double*)cameraMatrix.data;

	auto fx = cdata[0]; auto fy = cdata[4];
	auto cx = cdata[2]; auto cy = cdata[5];

	auto u = (fx * point.x / point.z) + cx;
	auto v = (fy * point.y / point.z) + cy;

	return Point2d(u, v);
}

//--------------------------------------------------
// UnProject
//--------------------------------------------------

/**
 * Find the original 3D point an image point, given the camera matrix and the depth 
 * @param cameraMatrix The camera matrix defining the projecting transform
 * @param point The image point that we are projecting back
 * @param Z The depth expected in 3D space
 * @return The original 3D location of the point
 */
Point3d Math3D::UnProject(const Mat& cameraMatrix, const Point2d& point, double Z) 
{
	auto cdata = (double*)cameraMatrix.data;

	auto fx = cdata[0]; auto fy = cdata[4];
	auto cx = cdata[2]; auto cy = cdata[5];

	auto X = (point.x - cx) * (Z / fx);
	auto Y = (point.y - cy) * (Z / fy);

	return Point3d(X, Y, Z);
}

//--------------------------------------------------
// TransformPoint
//--------------------------------------------------

/**
 * Transform a 3D point with a given pose
 * @param pose The pose matrix that we are transforming with
 * @param point The point that we are transforming
 * @return Return a Point3d
 */
Point3d Math3D::TransformPoint(const Mat& pose, const Point3d & point)
{
	auto data = (double*)pose.data;

	auto X = data[0] * point.x + data[1] * point.y + data[2] * point.z + data[3];
	auto Y = data[4] * point.x + data[5] * point.y + data[6] * point.z + data[7];
	auto Z = data[8] * point.x + data[9] * point.y + data[10] * point.z + data[11];

	return Point3d(X, Y, Z);
}

//--------------------------------------------------
// ExtractDepth
//--------------------------------------------------

/**
 * Retrieve the depth of a 2D point from the depth map
 * @param depthMap The depth map that we are retrieving from
 * @param position The position that we are retrieving from
 * @return The depth value as a double
 */
double Math3D::ExtractDepth(Mat& depthMap, const Point2d & position)
{
	auto u = (int)round(position.x); auto v = (int)round(position.y);

	if (u < 0 || u >= depthMap.cols || v < 0 || v >= depthMap.rows) return 0;

	return ((double*)depthMap.data)[u + v * depthMap.cols];
}

//--------------------------------------------------
// ExtractColor
//--------------------------------------------------

/**
 * Retrieve the color of a 2D point from a color map
 * @param colorMap The color map that we are getitng the color from
 * @param position The position that we are getting the color from
 * @return The associated color value
 */
Vec3i Math3D::ExtractColor(Mat& colorMap, const Point2d& position) 
{
	auto u = (int)round(position.x); auto v = (int)round(position.y);

	if (u < 0 || u >= colorMap.cols || v < 0 || v >= colorMap.rows) return Vec3i();

	auto index = u + v * colorMap.cols;

	auto blue = colorMap.data[index * 3 + 0];
	auto green = colorMap.data[index * 3 + 1];
	auto red = colorMap.data[index * 3 + 2];

	return Vec3i(red, green, blue);
}

//--------------------------------------------------
// ExtractK
//--------------------------------------------------

/**
 * Retrieve the camera matrix from the Q matrix 
 */
Mat Math3D::ExtractK(Mat& Q) 
{
	Mat result = Mat_<double>::eye(3, 3);

	auto qdata = (double*)Q.data;
	auto kdata = (double*)result.data;

	kdata[2] = -qdata[3];
	kdata[5] = -qdata[7];
	kdata[0] = qdata[11];
	kdata[4] = qdata[11];

	return result;
}

//--------------------------------------------------
// ExtractK
//--------------------------------------------------

/**
 * Build a camera matrix from a given focal length and optical center
 * @param f The given focal length
 * @param image Size the image size that we are using
 */
Mat Math3D::BuildKMatrix(double f, const Size& imageSize) 
{
	auto fx = f; auto fy = f; auto cx = imageSize.width / 2.0; auto cy = imageSize.height / 2.0;
	return Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1;
}

//--------------------------------------------------
// GetViewLimits
//--------------------------------------------------

/**
 * Find the view limits for the given camera
 * @param cameraMatrix The projection model for the given camera
 * @param imageSize The size of the image that we are processing
 * @param zmin The minimum distance we care about from the camera
 * @param zmax The maximum distance we care about from the camera
 */
void Math3D::GetViewLimits(Mat& cameraMatrix, const Size& imageSize, double zmin, double zmax, vector<Point3d>& output)
{
	// Add the close points
	output.push_back(UnProject(cameraMatrix, Point2d(0, 0), zmin));
	output.push_back(UnProject(cameraMatrix, Point2d(imageSize.width, 0), zmin));
	output.push_back(UnProject(cameraMatrix, Point2d(imageSize.width, imageSize.height), zmin));
	output.push_back(UnProject(cameraMatrix, Point2d(0, imageSize.height), zmin));

	// Add the far points
	output.push_back(UnProject(cameraMatrix, Point2d(0, 0), zmax));
	output.push_back(UnProject(cameraMatrix, Point2d(imageSize.width, 0), zmax));
	output.push_back(UnProject(cameraMatrix, Point2d(imageSize.width, imageSize.height), zmax));
	output.push_back(UnProject(cameraMatrix, Point2d(0, imageSize.height), zmax));
}

//--------------------------------------------------
// TransformPointSet
//--------------------------------------------------

/**
 * Transform the given point set
 * @param input The input set of points that we are transforming
 * @param output The output set of points after the transformation
 */
void Math3D::TransformPointSet(const Mat& pose, const vector<Point3d>& input, vector<Point3d>& output)
{
	for (const auto& point : input)
	{
		output.push_back(TransformPoint(pose, point));
	}
}

//--------------------------------------------------
// NormalizeVector
//--------------------------------------------------

/**
 * Normalize the given vector
 * @param vector The vector that we are normalizing
 * @return The resultant vector
 */
Vec3d Math3D::NormalizeVector(const Vec3d& vector) 
{
	auto magnitude = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	return Vec3d(vector[0] / magnitude, vector[1] / magnitude, vector[2] / magnitude);
}

//--------------------------------------------------
// GetOrientationRotation
//--------------------------------------------------

/**
 * Get the 3x3 rotation matrix that orientates the normal from one system to match the next
 * @param newNormal The normal that we want to match
 * @param oldNormal The old normal that we are matching
 * @return A 3x3 rotation matrix
 */
Mat Math3D::GetOrientationRotation(const Vec3d& newNormal, const Vec3d& oldNormal) 
{
	// Check that the values are not identical
	if (newNormal[0] == oldNormal[0] && newNormal[1] == oldNormal[1] && newNormal[2] == oldNormal[2]) return Mat_<double>::eye(3, 3);

	// Find the axis of rotation between both normals
	Vec3d axis = oldNormal.cross(newNormal);

	// Find the angle of rotation in radians
	auto angle = acos(oldNormal.dot(newNormal));

	// Get the rodriguez representation
	Vec3d rvec = NormalizeVector(axis) * angle;

	// Get the rotation matrix
	Mat result; Rodrigues(rvec, result);

	// return the result
	return result;
}

//--------------------------------------------------
// RotatePoint
//--------------------------------------------------

/**
 * Add the functionality to rotate a given point
 * @param rotation The rotation that we are applying
 * @param point The point that is being rotated
 */
Point3d Math3D::RotatePoint(Mat& rotation, const Point3d& point) 
{
	auto data = (double*)rotation.data;

	auto X = data[0] * point.x + data[1] * point.y + data[2] * point.z;
	auto Y = data[3] * point.x + data[4] * point.y + data[5] * point.z;
	auto Z = data[6] * point.x + data[7] * point.y + data[8] * point.z;

	return Point3d(X, Y, Z);
}

//--------------------------------------------------
// RotateVector
//--------------------------------------------------

/**
 * Add the logic to rotate a vector 
 */
Vec3d Math3D::RotateVector(Mat& rotation, const Vec3d& vector) 
{
	auto data = (double*)rotation.data;

	auto X = data[0] * vector[0] + data[1] * vector[1] + data[2] * vector[2];
	auto Y = data[3] * vector[0] + data[4] * vector[1] + data[5] * vector[2];
	auto Z = data[6] * vector[0] + data[7] * vector[1] + data[8] * vector[2];

	return Vec3d(X, Y, Z);
}

//--------------------------------------------------
// GetDistance
//--------------------------------------------------

/**
 * @brief Retrieve the distance between two 3D points
 * @param point1 The first point
 * @param point2 The second point
 * @return double The resultant distance
 */
double Math3D::GetDistance(const Point3d& point1, const Point3d& point2) 
{
	auto xdiff = point1.x - point2.x;
	auto ydiff = point1.y - point2.y;
	auto zdiff = point1.z - point2.z;

	return sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
}

//--------------------------------------------------
// Get Cloud Bounds
//--------------------------------------------------

/**
 * @brief Retrieve the bounds of a point cloud
 * @param points the points that are making up the cloud
 * @return Vec6d(xmin, xmax, ymin, ymax, zmin, zmax) 
 */
Vec6d Math3D::GetCloudBounds(vector<Point3d>& points) 
{
	// Validate that I have at least 1 point
	assert(points.size() >= 1);

	// Get the first point and initialize the counters;
	auto xmin = points[0].x; auto xmax = xmin;
	auto ymin = points[0].y; auto ymax = ymin;
	auto zmin = points[0].z; auto zmax = zmin;

	// Loop through the points and update the counters
	for (auto& point : points) 
	{
		xmin = min(xmin, point.x);
		xmax = max(xmax, point.x);

		ymin = min(ymin, point.y);
		ymax = max(ymax, point.y);

		zmin = min(zmin, point.z);
		zmax = max(zmax, point.z);
	}

	// Return the result
	return Vec6d(xmin, xmax, ymin, ymax, zmin, zmax);
}

//--------------------------------------------------
// GetLinePointDistance
//--------------------------------------------------

/**
 * @brief Retrieve the associated line point distance 
 * @param start The start of the line
 * @param gradient The gradient of the line
 * @param point The point we are comparing with
 * @return double The distance result
 */
double Math3D::GetLinePointDistance(const Point3d& start, const Vec3d& gradient, const Point3d& point) 
{
	auto v1 = Vec3d(point) - Vec3d(start);
	auto ngradient = NormalizeVector(gradient);
	auto v2 = v1.cross(ngradient);
	return GetMagnitude(v2);
}

//--------------------------------------------------
// GetMagnitude
//--------------------------------------------------

/**
 * @brief Retrieve the magnitude of a vector 
 * @param vector The vector
 * @return double The magnitude
 */
double Math3D::GetMagnitude(const Vec3d& vector) 
{
	return sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

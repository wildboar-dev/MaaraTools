//--------------------------------------------------
// Implementation code for PlaneUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "PlaneUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Plane fitting
//--------------------------------------------------

/**
 * @brief Fit a plane to a set of points
 * @param points The points that we are fitting to
 * @return The resultant Plane Model as Ax + By + Cz + D = 0 aka Vec4d(A, B, C, D)
 */
Vec4d PlaneUtils::FitPlane(vector<Point3d> & points) 
{
	// Create matrices and alias for holding the point information
	Mat A = Mat_<double>(points.size(), 3); Mat B = Mat_<double>(points.size(), 1);
	auto dataA = (double *)A.data; auto dataB = (double *)B.data;

	// Fill the matrices with points
	for (auto i = 0; i < points.size(); i++) 
	{
		dataA[i * 3 + 0] = points[i].x;
		dataA[i * 3 + 1] = points[i].y;
		dataA[i * 3 + 2] = 1;
		dataB[i] = -points[i].z;
	}

	// Solve for the problem
	Mat result; solve(A, B, result, DECOMP_SVD);
	auto rdata = (double *) result.data;

	// Return the result
	return Vec4d(rdata[0], rdata[1], 1, rdata[2]);
}

//--------------------------------------------------
// AvePlaneError
//--------------------------------------------------

/**
 * @brief Find the average plane fit error
 * @param plane The plane that we have fitted
 * @param points The number of points that we have fitted
 * @return double The average error
 */
Vec2d PlaneUtils::AvePlaneError(const Vec4d& plane, const vector<Point3d>& points) 
{
	// Setup working variables
	auto errors = vector<double>();

	// Loop thru the elements
	for (auto& point : points) 
	{
		auto epoint = ProjectPoint(plane, point);
		auto offset = Math3D::GetDistance(point, epoint);
		errors.push_back(offset);
	}

	// Find the average error
	auto mean = Scalar(); auto stddev = Scalar();
	meanStdDev(errors, mean, stddev);

	// Return the result
	return Vec2d(mean[0], stddev[0]);
}

//--------------------------------------------------
// ProjectPoint
//--------------------------------------------------

/**
 * Project a point into a plane
 * @param planeParameters The definition of the plane
 * @param point The point that is being projected
 * @return Return a Point3d
 */
Point3d PlaneUtils::ProjectPoint(const Vec4d& planeParameters, const Point3d& point)
{
	// Retrieve the core values
	auto normal = Vec3d(planeParameters[0], planeParameters[1], planeParameters[2]);
	auto magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

	// Normalize plane
	auto plane = planeParameters * (1.0 / magnitude);

	// Find alpha
	auto alphaTop = plane[0] * point.x + plane[1] * point.y + plane[2] * point.z + plane[3];
	auto alphaBottom = plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2];
	auto alpha = alphaTop / alphaBottom;

	// Build the projected point
	auto X = point.x - alpha * plane[0];
	auto Y = point.y - alpha * plane[1];
	auto Z = point.z - alpha * plane[2];

	// Return the point
	return Point3d(X, Y, Z);
}

//--------------------------------------------------
// FindAxis
//--------------------------------------------------

/**
 * Find the orthogonal axis system associated with the given plane
 * @param plane The plane that we are finding the axis system for
 * @return The matrix representing the plane direction
 */
Mat PlaneUtils::FindAxis(const Vec4d& plane) 
{
	// Create the resultant matrix
	Mat result = Mat_<double>(3, 3);

	// extract the data from the matrix
	auto output = (double*)result.data;

	// Add the first row into the matrix
	auto normal = Vec3d(plane[0], plane[1], plane[2]); normal = Math3D::NormalizeVector(normal);
	output[0] = normal[0]; output[1] = normal[1]; output[2] = normal[2];

	// Determine the second vector
	auto c3 = 0; if (abs(normal[1]) < abs(normal[0])) c3 = 1; if (abs(normal[2]) < abs(normal[c3])) c3 = 2;
	auto c1 = 0; if (abs(normal[1]) > abs(normal[0])) c1 = 1; if (abs(normal[2]) > abs(normal[c1])) c1 = 2;
	if (c1 == c3) c1++; if (c1 > 2) c1 = 0;
	auto lookup = Vec3i(); lookup[c1] = 1; lookup[c3] = 1; auto c2 = (lookup[0] == 0) ? 0 : ((lookup[1] == 0) ? 1 : 2);
	auto v1 = Vec3d(); v1[c1] = -normal[c2]; v1[2] = normal[c1]; v1 = Math3D::NormalizeVector(v1);
	output[3] = v1[0]; output[4] = v1[1]; output[5] = v1[2];

	// Determine the third vector
	auto v2 = normal.cross(v1); v2 = Math3D::NormalizeVector(v2);
	output[6] = v2[0]; output[7] = v2[1]; output[8] = v2[2];

	// Return the result
	return result;
}

//--------------------------------------------------
// Convert2d
//--------------------------------------------------

/**
 * Convert a 2D point into Plane 2D parameters with respect to the axes that we have been passed
 * @param axes The axes associated with the normal (assume that the first row is the normal)
 * @param point the point that we want to convert
 * @return The resultant converted point
 */
Point2d PlaneUtils::Convert2d(Mat& axes, Point3d& origin, Point3d& point) 
{
	auto axisData = (double*)axes.data;
	
	auto v1 = Vec3d(axisData[3], axisData[4], axisData[5]);
	auto v2 = Vec3d(axisData[6], axisData[7], axisData[8]);

	auto npoint = point - origin;

	auto u = v1.dot(npoint);
	auto v = v2.dot(npoint);

	return Point2d(u, v);
}

//--------------------------------------------------
// Convert3d
//--------------------------------------------------

Point3d PlaneUtils::Convert3d(Mat& axes, Point3d& origin, Point2d& point) 
{
	auto axisData = (double*)axes.data;

	auto v1 = Vec3d(axisData[3], axisData[4], axisData[5]);
	auto v2 = Vec3d(axisData[6], axisData[7], axisData[8]);

	auto p1 = v1 * point.x + v2 * point.y;
	auto p2 = Vec3d(origin) + p1;

	return Point3d(p2);
}

//--------------------------------------------------
// Generate a plane of points
//--------------------------------------------------

/**
 * @brief Add the logic to build a set of plane points
 * @param plane The plane equation
 * @param points The points that we are filling
 * @param rangeX The range of the plane in the x-direction
 * @param rangeY The range of the plane in the y-direction
 * @param stepSize The count of the steps
 */
void PlaneUtils::BuildPlane(const Vec4d& plane, vector<Point3d>& points, const Range& rangeX, const Range& rangeY, int stepCount) 
{
	points.clear();

	auto xstep = (rangeX.end - rangeX.start) / (double) stepCount;
	auto ystep = (rangeY.end - rangeY.start) / (double) stepCount;

	for (auto i = 0; i <= stepCount; i++) 
	{
		for (auto j = 0; j <= stepCount; j++) 
		{
			auto X = rangeX.start + i * xstep;
			auto Y = rangeY.start + j * ystep;

			auto Z = (plane[0] * X + plane[1] * Y + plane[3]) / -plane[2];		
			points.push_back(Point3d(X, Y, Z));
		}
	}
}

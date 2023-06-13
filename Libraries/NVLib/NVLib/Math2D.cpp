//--------------------------------------------------
// Implementation code for Math2D
//
// @author: Wild Boar
//--------------------------------------------------

#include "Math2D.h"
using namespace NVLib;

//--------------------------------------------------
// GetDistance
//--------------------------------------------------

/**
 * Get the distance between points
 * @param point1 The first point
 * @param point2 The second point
 * @return Return a double
 */
double Math2D::GetDistance(const Point & point1, const Point & point2)
{
	auto xdiff = point1.x - point2.x;
	auto ydiff = point1.y - point2.y;

	return sqrt(xdiff * xdiff + ydiff * ydiff);
}

/**
 * Get the distance between points
 * @param point1 The first point
 * @param point2 The second point
 * @return Return a double
 */
double Math2D::GetDistance(const Point2d& point1, const Point2d& point2) 
{
	auto xdiff = point1.x - point2.x;
	auto ydiff = point1.y - point2.y;

	return sqrt(xdiff * xdiff + ydiff * ydiff);

}

//--------------------------------------------------
// GetLongestLineIndex
//--------------------------------------------------

/**
 * Get the index of the start point of the longest line
 * @param polygon The points that make up the polygon
 * @param closed Indicates whether the polygon should be considered open or closed
 */
int Math2D::GetLongestLineIndex(vector<Point2d>& polygon, bool closed) 
{
	if (polygon.size() <= 1) throw string("A polygon needs to have more than 1 corner");

	auto lineCount = closed ? (int)polygon.size() : (int)polygon.size() - 1;

	int bestIndex = 0; double bestLength = 0;

	for (auto i = 0; i < lineCount; i++) 
	{
		auto j = i + 1; if (j >= (int)polygon.size()) j = j - (int)polygon.size();
		auto length = GetDistance(polygon[i], polygon[j]);
		if (length > bestLength) { bestIndex = i; bestLength = length; }
	}

	return bestIndex;
}

//--------------------------------------------------
// GetLongestIntercept
//--------------------------------------------------

/**
 * Calculate the point of interception between lines
 * @param line1 Defined by a two point array
 * @param line2 Defined by a two point array
 * @return The point of intersection as a Point2d
 */
Point2d Math2D::GetLineIntercept(Line2d& line1, Line2d& line2) 
{
	auto sx = line1.GetStart().x; auto sy = line1.GetStart().y;
	auto gx = line1.GetEnd().x - sx; auto gy = line1.GetEnd().y - sy;

	auto ssx = line2.GetStart().x; auto ssy = line2.GetStart().y;
	auto ggx = line2.GetEnd().x - ssx; auto ggy = line2.GetEnd().y - ssy;

	Mat A = (Mat_<double>(2, 2) << gx, -ggx, gy, -ggy);
	Mat b = (Mat_<double>(2, 1) << ssx - sx, ssy - sy);

	Mat solution; solve(A, b, solution);
	auto result = (double*)solution.data;

	return Point2d(sx + gx * result[0], sy + gy * result[0]);
}

//--------------------------------------------------
// Transform
//--------------------------------------------------

/**
 * Transform a point using a homography
 * @param homography The homography that we are using to transform the point
 * @param point The point that we are transforming
 */
Point2d Math2D::Transform(Mat& homography, const Point2d& point) 
{
	auto H = (double*)homography.data;

	auto X = point.x * H[0] + point.y * H[1] + H[2];
	auto Y = point.x * H[3] + point.y * H[4] + H[5];
	auto Z = point.x * H[6] + point.y * H[7] + H[8];

	auto u = X / Z; auto v = Y / Z;

	return Point2d(u, v);
}

//--------------------------------------------------
// GetPointLineDistance
//--------------------------------------------------

/**
 * Get the distance a point is from a line
 * @param line The line that we are comparing with
 * @param point The point we are comparing against
 */
double Math2D::GetPointLineDistance(Line2d& line, Point2d& point) 
{
	auto pointVector = Vec2d(point - line.GetStart());
	auto magnitude = sqrt(pointVector[0] * pointVector[0] + pointVector[1] * pointVector[1]);
	auto normalizedPointVector = pointVector / magnitude;
	auto crossProduct = normalizedPointVector.dot(line.GetGradient());
	auto angle = acos(crossProduct);
	auto result = sin(angle) * magnitude;
	return result;
}

//--------------------------------------------------
// GetMagnitude
//--------------------------------------------------

/**
 * @brief Retrieve the magnitude associated with a vector
 * @param vector The vector that we are getting the magnitude of
 * @return double The magnitude that we are managed to acquire
 */
double Math2D::GetMagnitude(const Vec2d& vector) 
{
	return sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
}

//--------------------------------------------------
// Normalize
//--------------------------------------------------

/**
 * @brief Retrieve the normalized version of the vector
 * @param vector The vector that we are normalizing
 * @return Vec2d The normalized version of the vector
 */
Vec2d Math2D::Normalize(const Vec2d& vector) 
{
	auto magnitude = GetMagnitude(vector);
	return Vec2d(vector[0] / magnitude, vector[1] / magnitude);
}

//--------------------------------------------------
// Get Normalized Gradient
//--------------------------------------------------

/**
 * @brief Retrieve the normalized gradient 
 * @param point1 The first point within the set 
 * @param point2 The second point within the set
 * @return Vec2d The vector that we are getting the gradient for
 */
Vec2d Math2D::GetNGradient(const Point2d& point1, const Point2d& point2) 
{
	auto xDiff = point2.x - point1.x;
	auto yDiff = point2.y - point1.y;

	auto gradient = Vec2d(xDiff, yDiff);

	return Normalize(gradient);
}
//--------------------------------------------------
// Implementation of class ConvertUtils
//
// @author: Wild Boar
//
// @date: 2021-08-27
//--------------------------------------------------

#include "ConvertUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Point Vector Conversion
//--------------------------------------------------

/**
 * @brief Converts double points into float points 
 * @param input The input points
 * @param output The output points
 */
void ConvertUtils::ConvertPoints(vector<Point2d>& input, vector<Point2f>& output)
{
	output.clear();

	for (auto& point : input) 
	{
		output.push_back(Point2f((float)point.x, (float)point.y));
	}
}

/**
 * @brief Convert double points into integer points
 * @param input The input points
 * @param output The output points
 */
void ConvertUtils::ConvertPoints(vector<Point2d>& input, vector<Point>& output)
{
	output.clear();

	for (auto& point : input) 
	{
		output.push_back(Point((int)round(point.x), (int)round(point.y)));
	}
}

/**
 * @brief Converts integer points into double points
 * @param input The set of integer points as input
 * @param output The set of double points as output
 */
void ConvertUtils::ConvertPoints(vector<Point>& input, vector<Point2d>& output) 
{
	output.clear(); for (auto& point : input) output.push_back(Point2d(point.x, point.y));
}
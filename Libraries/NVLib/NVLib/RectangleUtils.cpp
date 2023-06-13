//--------------------------------------------------
// Implementation code for RectangleUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "RectangleUtils.h"
using namespace NVLib;

//--------------------------------------------------
// GetBestFitCorners
//--------------------------------------------------

/**
 * @brief Find the best fit corners for a rectangle with a fixed aspect ratio
 * @param fitSize The bounds of the region we are fitting
 * @param aspectRatio The required aspect ratio of the fitted rectangle
 * @param outPoints The output points that we are generating
 */
void RectangleUtils::GetBestFitCorners(const Size& fitSize, double aspectRatio, vector<Point2d>& outPoints)
{
	// Validation
	if (aspectRatio <= 0) throw runtime_error("The aspect ratio cannot be negative or zero");
	if (fitSize.width <= 0 || fitSize.height <= 0) throw runtime_error("Invalid rectangle dimensions");

	// Build the scenarios
	auto width_1 = fitSize.width; auto height_1 = fitSize.width * aspectRatio;
	auto width_2 = fitSize.height / aspectRatio; auto height_2 = fitSize.height;

	// Select the best scenarios
	auto bestWidth = (width_1 <= fitSize.width && height_1 <= fitSize.height) ? width_1 : width_2;
	auto bestHeight = (width_1 <= fitSize.width && height_1 <= fitSize.height) ? height_1 : height_2;

	// Generate the points
	outPoints.push_back(Point2d(-bestWidth * 0.5, +bestHeight * 0.5));
	outPoints.push_back(Point2d(+bestWidth * 0.5, +bestHeight * 0.5));
	outPoints.push_back(Point2d(+bestWidth * 0.5, -bestHeight * 0.5));
	outPoints.push_back(Point2d(-bestWidth * 0.5, -bestHeight * 0.5));
}

//--------------------------------------------------
// GetMaskCorners
//--------------------------------------------------

/**
 * @brief Extract the corners of a projective rectangular mask
 * @param mask The mask that we are extracting from
 * @param outCorners The corners that we have extracted
 */
void RectangleUtils::GetMaskCorners(Mat& mask, vector<Point>& outCorners)
{
	// Find Edges
	Mat edges; Canny(mask, edges, 250, 255);

	// find contours
	auto contours = vector< vector<Point> >();
	cv::findContours(edges, contours, cv::RetrievalModes::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	if (contours.size() == 0) throw runtime_error("Unable to find an object of interest");

	// Find the largest contour
	auto maxLength = contours[0].size(); auto bestIndex = 0;
	for (auto i = 0; i < contours.size(); i++) 
	{
		auto length = contours[i].size();
		if (length > maxLength) { maxLength = length; bestIndex = i; }
	}

	//Mat displayImage; cvtColor(mask, displayImage, COLOR_GRAY2BGR);
	//for (auto i = 0; i < maxLength; i++) circle(displayImage, contours[bestIndex][i], 3, Scalar(0, 0, 255), cv::FILLED);
	//DisplayUtils::ShowImage("Contour", displayImage, 1000); cv::waitKey();

	// Fit polygon to best contour
	auto rectangle = vector<Point>();
	auto success = ApproxPoly(contours[bestIndex], rectangle, 0.1, 100, 4);
	if (!success) throw runtime_error("Could not approximate the shape with a 4 sided poly");

	// Add points to outcorners
	OrderPoints(rectangle, outCorners);
}

//--------------------------------------------------
// ApproxPoly
//--------------------------------------------------

/**
 * @brief Attempts to find the optimal polygon with a given set of sides (hack version)
 * @param contours the contour
 * @param rects The rectangles
 * @param minepsilon The minimum epsilon
 * @param maxepsilon The maximum epsilon
 * @param sides The number of sides
 * @return Whether we were succesfull
*/
bool RectangleUtils::ApproxPoly(std::vector<cv::Point> contours, std::vector<cv::Point>& rects, double minepsilon, double maxepsilon, int sides)
{
	std::vector<cv::Point> rect1;
	std::vector<cv::Point> rect2;
	cv::approxPolyDP(contours, rect1, minepsilon, true);
	cv::approxPolyDP(contours, rect2, maxepsilon, true);
	if (rect1.size() > sides && rect2.size() > sides)
	{
		rects = contours;
		return false;
	}
	if (rect1.size() < sides && rect2.size() < sides)
	{
		rects = contours;
		return false;
	}
	else
	{
		if (rect1.size() == sides)
		{
			rects.resize(sides);
			for (int i = 0; i < sides; i++)
			{
				rects[i] = rect1[i];
			}
			return true;
		}
		else if (rect2.size() == sides)
		{
			rects.resize(sides);
			for (int i = 0; i < sides; i++)
			{
				rects[i] = rect2[i];
			}
			return true;
		}
		else
		{
			double midepsilon = (minepsilon + maxepsilon) / 2.0;
			std::vector<cv::Point> rect3;
			cv::approxPolyDP(contours, rect3, midepsilon, true);
			if (rect3.size() == sides)
			{
				rects.resize(sides);
				for (int i = 0; i < sides; i++)
				{
					rects[i] = rect3[i];
				}
				return true;
			}
			else if (rect3.size() < sides)
			{
				return ApproxPoly(contours, rects, minepsilon, midepsilon, sides);
			}
			else
			{
				return ApproxPoly(contours, rects, midepsilon, minepsilon, sides);
			}
		}
	}
	return true;
}

//--------------------------------------------------
// OrderPoints
//--------------------------------------------------

/**
 * @brief Add the logic to order points
 * @param input The input list
 * @param output The output list
*/
void RectangleUtils::OrderPoints(vector<Point>& input, vector<Point>& output) 
{
	// Find the point closest to the origin
	auto index1 = 0; auto d1 = input[0].x * input[0].x + input[0].y * input[0].y;
	for (auto i = 1; i < input.size(); i++) 
	{
		auto distance = input[i].x * input[i].x + input[i].y * input[i].y;
		if (distance < d1) { d1 = distance; index1 = i; }
	}
	auto p1 = input[index1]; 

	// Find the furthest to the origin
	auto index2 = index1; auto d2 = d1;
	for (auto i = 1; i < input.size(); i++)
	{
		auto distance = input[i].x * input[i].x + input[i].y * input[i].y;
		if (distance > d2) { d2 = distance; index2 = i; }
	}
	auto p3 = input[index2];

	// Find the remaining indices;
	auto r = vector<int>();
	for (auto i = 0; i < input.size(); i++) 
	{
		if (i != index1 && i != index2) r.push_back(i);
	}

	// Find the last two points
	auto p2 = (input[r[0]].x > input[ r[1]].x) ? input[r[0]] : input[r[1]];
	auto p4 = (input[r[0]].x <= input[r[1]].x) ? input[r[0]] : input[r[1]];

	// Save the points
	output.push_back(p1);
	output.push_back(p2);
	output.push_back(p3);
	output.push_back(p4);
}
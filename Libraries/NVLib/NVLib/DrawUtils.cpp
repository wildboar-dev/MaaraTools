//--------------------------------------------------
// Implementation code for DrawUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "DrawUtils.h"
using namespace NVLib;

//--------------------------------------------------
// DrawPoints
//--------------------------------------------------

/**
 * Render points on an image
 * @param image The image that we are drawing the points from
 * @param points The points that we are rendering
 * @param radius The radius of the point drawn
 * @param color The color of the  point drawn
 */
void DrawUtils::DrawPoints(Mat& image, vector<Point2d>& points, int radius, const Scalar& color)
{
	for (auto point : points) 
	{
		circle(image, point, radius, color, cv::FILLED);
		circle(image, point, radius,  Scalar(), 1);
	}
}

//--------------------------------------------------
// DrawRectnagle
//--------------------------------------------------

/**
 * @brief Render a rectangle on the provided image
 * @param image The image we are to render upon
 * @param rectangle The rectangle that we want
 * @param color The color of the rectangle
 */
void DrawUtils::DrawFilledRect(Mat& image, const Rect& rectangle, const Scalar& color) 
{
	cv::rectangle(image, rectangle, color, cv::FILLED);
}

//--------------------------------------------------
// GetRandomGray
//--------------------------------------------------

/**
 * @brief Generate a gray scale image with random values
 * @param size The size of the image
 * @return Mat The values of the images
 */
Mat DrawUtils::GetRandomGray(const Size& size) 
{
	Mat result = Mat_<uchar>(size);

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			result.data[column + row + result.cols] = rand() % 255;
		}
	}

	return result;
}

//--------------------------------------------------
// DrawPolyOutline
//--------------------------------------------------

/**
 * @brief Render a polygon outline
 * @param image The image we are drawing upon
 * @param color The color that we want the outline to be
 * @param points The polygon Points
 */
void DrawUtils::DrawPolyOutline(Mat& image, const Scalar& color, vector<Point>& points)
{
	polylines(image, points, true, color);
}

//--------------------------------------------------
// GetPolyMask
//--------------------------------------------------

/**
 * @brief Retrieve a mask for this card
 * @param imageSize The size of the image that we want
 * @param points The ponts that make up the mask
 */
Mat DrawUtils::GetPolyMask(const Size& imageSize, vector<Point>& points)
{
	Mat result = Mat_<uchar>::zeros(imageSize);
	auto polyArray = vector< vector<Point> > { points };
	fillPoly(result, polyArray, Scalar(255), 8, 0);
	return result;
}

//--------------------------------------------------
// Get a rectangular mask
//--------------------------------------------------

/**
 * @brief Create a rectangular mask
 * @param imageSize The size of the image
 * @param rect The rectangle parameters 
 * @return Mat The mask that we are rendering
 */
Mat DrawUtils::GetRectangleMask(const Size& imageSize, const Rect& rect) 
{
	Mat result = Mat_<uchar>::zeros(imageSize);
	rectangle(result, rect, Scalar(255), cv::FILLED);
	return result;
}

//--------------------------------------------------
// OverlayMask
//--------------------------------------------------

/**
 * @brief Defines the functionality to overlay a mask
 * @param image The image that we are overlaying
 * @param overlay The overlay image
 * @param color The color that we want the overlay to be
 * @return Mat The resultant image
 */
Mat DrawUtils::OverlayMask(Mat& image, Mat& overlay, const Scalar& color) 
{
	Mat result = image.clone();

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			auto index = column + row * result.cols;

			auto flag = overlay.data[index] != 0;

			if (flag) 
			{
				result.data[index * 3 + 0] = color[0];
				result.data[index * 3 + 1] = color[1];
				result.data[index * 3 + 2] = color[2];
			}	
		}
	}

	return result;
}

//--------------------------------------------------
// Extract Mask
//--------------------------------------------------

/**
 * @brief Add the logic to extract a mask
 * @param image The image that we are extracting from
 * @param mask The mask of values that we want
 * @return Mat The resultant mask
 */
Mat DrawUtils::ExtractMask(Mat& image, Mat& mask) 
{
	if (mask.rows != image.rows || mask.cols != image.cols) throw runtime_error("The image and the mask need to be the same size");

	auto result = image.clone();

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			auto index = column + row * image.cols;
			auto maskValue = mask.data[index];
			
			if (maskValue != 0) continue;

			result.data[index * 3 + 0] = 0;
			result.data[index * 3 + 1] = 0;
			result.data[index * 3 + 2] = 0;	
		}
	}

	return result;
}

//--------------------------------------------------
// DrawGradientmap
//--------------------------------------------------

/**
 * @brief Add the logic to draw a gradient map
 * @param direction The direction that I am drawing
 * @return Mat The resultant gradient map
 */
Mat DrawUtils::DrawDirectionMap(Mat& direction) 
{
	Mat result = Mat_<Vec3b>::zeros(direction.size());
	auto input = (double *) direction.data;

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			auto index = column + row * result.cols;
			auto radians = input[index];	
			auto degrees = PoseUtils::Radian2Degree(radians) + 180;

			if ( (degrees >= 45 && degrees <=135) || (degrees >=225 && degrees <= 315) ) 
			{
				result.data[index * 3 + 0] = 0;
				result.data[index * 3 + 1] = 0;
				result.data[index * 3 + 2] = 255;
			}
			else if ( (degrees > 315) || (degrees >= 0 && degrees <= 45) || (degrees >= 135 || degrees <= 225) ) 
			{
				result.data[index * 3 + 0] = 0;
				result.data[index * 3 + 1] = 255;
				result.data[index * 3 + 2] = 0;
			}

		}
	}

	return result;
}

//--------------------------------------------------
// DrawEightBit
//--------------------------------------------------

/**
 * @brief Converts a float image into an eight bit image
 * @param floatImage The float image
 * @return Mat The resultant 8 bit image
 */
Mat DrawUtils::DrawEightBit(Mat& floatImage) 
{
	double min=0, max=1000; minMaxIdx(floatImage, &min, &max);
	Mat result(floatImage.size(), CV_8UC1); floatImage.convertTo(result, CV_8UC1, 255.0 / (max - min), -min);
	return result;
}

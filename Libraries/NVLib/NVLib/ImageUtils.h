//--------------------------------------------------
// A collection of image processing utilitiies
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

typedef pair<int, int> Pair;

namespace NVLib
{
	class ImageUtils
	{
	public:
		static Mat GetHistogram(Mat& grayImage);
		static Mat GetCumulativeHistogram(Mat& histogram);
		static int GetUpperPercentileIntensity(Mat& chistogram, double percentile);
		static void GetOrderedLabelCounts(Mat& labelMap, int labelCount, vector<Pair>& result);
		static Mat GetBinaryLabelMap(Mat& fullMap, int foregroundLabel);
		static void GetBiggestContour(Mat& binaryImage, vector<Point>& contour);
		static void GetContours(Mat& binaryImage, vector<vector<Point>> & contours);
		static Mat GetColorBinary(Mat& image, const Vec3i& color);
		static Mat GetMaskImage(Mat& image, Mat& mask, const Vec3i& color);
		static Mat GetGradientX(Mat& image);
		static Mat GetGradientY(Mat& image);
		static Mat GetGradientMap(Mat& image);
		static Mat GetColorGradient(Mat& magnitude, Mat& direction);
		static Mat GetNormalizedGrayImage(Mat& image);
		static Mat InvertMask(Mat& maskImage);
		static void GetInOutHistograms(Mat& grayImage, Mat& mask, Mat& innerHist, Mat& outerHist);
		static int GetPixelCount(Mat& histogram);
		static int GetPixelCount(Mat& image, const Scalar& color);
		static int Hist2PD(Mat& histogram, Mat& distribution);
		static Mat AutoCanny(Mat& image, float sigma = 0.33);
	private:
		static double GetMedian(Mat& matrix);	
	};
}
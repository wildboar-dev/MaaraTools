//--------------------------------------------------
// Implementation code for DisplayUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "DisplayUtils.h"
using namespace NVLib;

//--------------------------------------------------
// ShowImage
//--------------------------------------------------

/**
 * @brief Show an image on the screen
 * @param caption The caption of the image that we are showing
 * @param image The image that we are showing
 * @param width The width of the image that we are showing
 */
void DisplayUtils::ShowImage(const string& caption, Mat& image, int width) 
{
	if (image.empty()) return;
	auto maxDimension = max(image.rows, image.cols);
	if (maxDimension <= 0) return;
	auto factor = (double)width / maxDimension;
	Mat smallImage; resize(image, smallImage, Size(), factor, factor);
	imshow(caption, smallImage);
}

//--------------------------------------------------
// ShowFloatMap
//--------------------------------------------------

/**
 * @brief Show a float map on the screen
 * @param caption The caption of the map that we are showing
 * @param map The map that we are showing
 * @param width The width of the image that we are showing
 */
void DisplayUtils::ShowFloatMap(const string& caption, Mat& map, int width) 
{
	auto maxDimension = max(map.rows, map.cols);
	auto factor = (double)width / maxDimension;
	Mat smallImage; resize(map, smallImage, Size(), factor, factor);

	double min=0, max=1000; minMaxIdx(smallImage, &min, &max);
	Mat d(smallImage.size(), CV_8UC1); smallImage.convertTo(d, CV_8UC1, 255.0 / (max - min), -min);
	Mat colorDepth; applyColorMap(d, colorDepth, COLORMAP_JET);

	imshow(caption, colorDepth);
}

//--------------------------------------------------
// ShowFeatures
//--------------------------------------------------

/**
 * @brief Show the detected features on the screen
 * @param caption The caption of the image being shown
 * @param image The image that we are rendering
 * @param corners The corners that are being rendered
 * @param circleRadius The radius of the circle that makes the corners
 * @param color The color of the associated corners
 * @param width The width of the image that we are showing
 */
void DisplayUtils::ShowFeatures(const string& caption, Mat& image, vector<Point2d>& features, int circleRadius, const Vec3i& color, int width)
{
	Mat displayImage = image.clone(); auto renderColor = Scalar(color[0], color[1], color[2]);

	for (auto& corner : features) 
	{
		circle(displayImage, corner, circleRadius, renderColor, -1);
		circle(displayImage, corner, circleRadius, Scalar(), 1);
	}

	ShowImage(caption, displayImage, width);
}

//--------------------------------------------------
// ShowStereoFeatures
//--------------------------------------------------

/**
 * @brief Show a given set of stereo features on the screen
 * @param caption The caption associated wiht the given window
 * @param frame The frame that we are showing the features for
 * @param width The width that we want the visualization to be
 * @param matches The list of left points that we are displaying
} * @param color The color that we want to display the points as
 */
void DisplayUtils::ShowStereoFeatures(const string& caption, StereoFrame* frame, int width, vector<FeatureMatch>& matches, const Vec3i& color) 
{
	int actualWidth = frame->GetLeft().cols + frame->GetRight().cols;
	auto factor = (double)width / (double)actualWidth;

	Mat result = Mat::zeros(frame->GetLeft().rows, actualWidth, CV_8UC3);
	int height1 = frame->GetLeft().rows; int width1 = frame->GetLeft().cols;
	frame->GetLeft().copyTo(result(Range(0, height1), Range(0, width1)));

	int height2 = frame->GetRight().rows; int width2 = frame->GetRight().cols;
	frame->GetRight().copyTo(result(Range(0, height2), Range(width1, width1 + width2)));

	Mat smallImage; resize(result, smallImage, Size(), factor, factor);

	auto displayColor = Scalar((double)color[0], (double)color[1], (double)color[2]);

	for (auto& match : matches) 
	{
		auto point = match.GetPoint1();
		auto x = (int)round(point.x * factor);
		auto y = (int)round(point.y * factor);
		circle(smallImage, Point(x, y), 3, displayColor, FILLED);
		circle(smallImage, Point(x, y), 4, Scalar(), 1);
	}

	for (auto& match : matches)
	{
		auto point = match.GetPoint2();
		auto x = (int)round(((double)point.x + width1) * factor);
		auto y = (int)round(point.y * factor);
		circle(smallImage, Point(x, y), 3, displayColor, -1);
		circle(smallImage, Point(x, y), 4, Scalar(), 1);
	}

	imshow(caption, smallImage);
}


//--------------------------------------------------
// ShowToggleImages
//--------------------------------------------------

/**
 * @brief Show the result on the screen
 * @param caption The caption of the display window
 * @param image1 The first image that we are showing
 * @param image2 The second image that we are showing
 * @param width The width of the image we are processing
 */
void DisplayUtils::ShowToggleImages(string caption, Mat& image1, Mat& image2, int width)
{
	int actualWidth = image1.cols;
	auto scaleFactor = (double)width / (double)actualWidth;

	Mat small1; resize(image1, small1, Size(), scaleFactor, scaleFactor);
	Mat small2; resize(image2, small2, Size(), scaleFactor, scaleFactor);

	Mat* currentImage = &small1;

	while (true)
	{
		imshow(caption, *currentImage);
		int key = waitKey(1000);
		if (key == 27) break;
		currentImage = currentImage == &small1 ? &small2 : &small1;
	}
}

//--------------------------------------------------
// ShowStereoFrame
//--------------------------------------------------

/**
 * @brief Display a stereo frame frame on the screen
 * @param caption The caption associated with the frame
 * @param frame The frame that we are displaying
 * @param width The width of the frame that we are displaying
 */
void DisplayUtils::ShowStereoFrame(const string& caption, StereoFrame& frame, int width)
{
	int actualWidth = frame.GetLeft().cols + frame.GetRight().cols;
	auto factor = (double)width / (double)actualWidth;

	Mat result = Mat::zeros(frame.GetLeft().rows, actualWidth, CV_8UC3);
	int height1 = frame.GetLeft().rows; int width1 = frame.GetRight().cols;
	frame.GetLeft().copyTo(result( Range(0,height1), Range(0, width1) ));

	int height2 = frame.GetRight().rows; int width2 = frame.GetRight().cols;
	frame.GetRight().copyTo(result(Range(0, height2), Range(width1, width1 + width2)));

	Mat smallImage; resize(result, smallImage, Size(), factor, factor);

	imshow(caption, smallImage);
}

//--------------------------------------------------
// ShowDepthFrame
//--------------------------------------------------

/**
 * @brief Display a depth frame on the screen
 * @param caption The caption associated with the frame
 * @param frame The frame that we are displaying
 * @param width The width of the frame that we are displaying
 */
void DisplayUtils::ShowDepthFrame(const string& caption, DepthFrame* frame, int width) 
{
	int actualWidth = frame->GetColor().cols + frame->GetDepth().cols;
	auto factor = (double)width / (double)actualWidth;

	Mat result = Mat::zeros(frame->GetColor().rows, actualWidth, CV_8UC3);
	int colorHeight = frame->GetColor().rows; int colorWidth = frame->GetColor().cols;
	frame->GetColor().copyTo(result( Range(0,colorHeight), Range(0, colorWidth) ));

	double minDepth = 0, maxDepth = 1000; minMaxIdx(frame->GetDepth(), &minDepth, &maxDepth);
	Mat d(frame->GetDepth().size(), CV_8UC1); frame->GetDepth().convertTo(d, CV_8UC1, 255.0 / (maxDepth - minDepth), -minDepth);
	Mat colorDepth; applyColorMap(d, colorDepth, COLORMAP_JET);
	int depthHeight = frame->GetDepth().rows; int depthWidth = frame->GetDepth().cols;
	colorDepth.copyTo(result(Range(0, depthHeight), Range(colorWidth, colorWidth + depthWidth)));

	Mat smallImage; resize(result, smallImage, Size(), factor, factor);

	imshow(caption, smallImage);
}


/**
 * @brief Display a depth frame on the screen
 * @param caption The caption associated with the frame
 * @param frame The frame that we are displaying
 * @param minDepth The minimum depth level
 * @param maxDepth The maximum depth level
 * @param width The width of the frame that we are displaying
 */
void DisplayUtils::ShowDepthFrame(const string& caption, DepthFrame* frame, double minDepth, double maxDepth, int width) 
{
	int actualWidth = frame->GetColor().cols + frame->GetDepth().cols;
	auto factor = (double)width / (double)actualWidth;

	Mat result = Mat::zeros(frame->GetColor().rows, actualWidth, CV_8UC3);
	int colorHeight = frame->GetColor().rows; int colorWidth = frame->GetColor().cols;
	frame->GetColor().copyTo(result( Range(0,colorHeight), Range(0, colorWidth) ));

	Mat d(frame->GetDepth().size(), CV_8UC1); frame->GetDepth().convertTo(d, CV_8UC1, 255.0 / (maxDepth - minDepth), -minDepth);
	Mat colorDepth; applyColorMap(d, colorDepth, COLORMAP_JET);
	int depthHeight = frame->GetDepth().rows; int depthWidth = frame->GetDepth().cols;
	colorDepth.copyTo(result(Range(0, depthHeight), Range(colorWidth, colorWidth + depthWidth)));

	Mat smallImage; resize(result, smallImage, Size(), factor, factor);

	imshow(caption, smallImage);
}

//--------------------------------------------------
// ShowStereoFeatures
//--------------------------------------------------

/**
 * @brief Show a given set of stereo features on the screen
 * @param caption The caption associated with the given window
 * @param frame The frame that we are showing the features for
 * @param width The width that we want the visualization to be
 * @param leftPoints The list of left points that we are displaying
 * @param rightPoints The list of right points that we are displaying
 * @param color The color that we want to display the points as
 */
void DisplayUtils::ShowStereoFeatures(const string& caption, StereoFrame* frame, int width, vector<Point>& leftPoints, vector<Point>& rightPoints, const Vec3b& color) 
{
	int actualWidth = frame->GetLeft().cols + frame->GetRight().cols;
	auto factor = (double)width / (double)actualWidth;

	Mat result = Mat::zeros(frame->GetLeft().rows, actualWidth, CV_8UC3);
	int height1 = frame->GetLeft().rows; int width1 = frame->GetLeft().cols;
	frame->GetLeft().copyTo(result(Range(0, height1), Range(0, width1)));

	int height2 = frame->GetRight().rows; int width2 = frame->GetRight().cols;
	frame->GetRight().copyTo(result(Range(0, height2), Range(width1, width1 + width2)));

	Mat smallImage; resize(result, smallImage, Size(), factor, factor);

	auto displayColor = Scalar((double)color[0], (double)color[1], (double)color[2]);

	for (auto point : leftPoints) 
	{
		auto x = (int)round(point.x * factor);
		auto y = (int)round(point.y * factor);
		circle(smallImage, Point(x, y), 3, displayColor, -1);
	}

	for (auto point : rightPoints)
	{
		auto x = (int)round(((double)point.x + width1) * factor);
		auto y = (int)round(point.y * factor);
		circle(smallImage, Point(x, y), 3, displayColor, -1);
	}

	imshow(caption, smallImage);
}

//--------------------------------------------------
// ShowStereoCalibration
//--------------------------------------------------

/**
 * @brief Show stereo calibration details on the screen 
 * @param calibration The calibration that we are showing
 */
void DisplayUtils::ShowStereoCalibration(StereoCalibration * calibration) 
{
	cout << "==== Stereo Calibration ====" << endl;

	cout << "Camera 1: " << endl;
	cout << " K1: " << endl << calibration->GetCamera1() << endl << endl;
	cout << " D1: " << endl << calibration->GetDistortion1().t() << endl;
	cout << " Image Size: [" << calibration->GetImageSize1().width << ", " << calibration->GetImageSize1().height << "]" << endl;

	cout << endl;

	cout << "Camera 2: " << endl;
	cout << " K2: " << endl << calibration->GetCamera2() << endl << endl;
	cout << " D2: " << endl << calibration->GetDistortion2().t() << endl;
	cout << " Image Size: [" << calibration->GetImageSize2().width << ", " << calibration->GetImageSize2().height << "]" << endl << endl;

	cout << endl;

	cout << "Extrinsics: " << endl << calibration->GetPose() << endl;

	cout << "==== END Stereo Calibration ====" << endl;
}

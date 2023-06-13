//--------------------------------------------------
// Implementation code for LoadUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "LoadUtils.h"
using namespace NVLib;

//--------------------------------------------------
// LoadImage
//--------------------------------------------------

/**
 * Load an image from disk
 * @param path The path that we are loading the image from
 * @return The image that was loaded
 */
Mat LoadUtils::LoadImage(const std::string& path)
{
	cv::Mat image = imread(path, IMREAD_COLOR);
	if (image.empty()) throw runtime_error("Unable to load image: " + path);
 	return image;
}

//--------------------------------------------------
// LoadMask
//--------------------------------------------------

/**
 * @brief Load a mask from disk
 * @param path The path that we are loading from
 * @return The mask that was loaded
 */
Mat LoadUtils::LoadMask(const string& path) 
{
	cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
	if (image.empty()) throw runtime_error("Unable to load image: " + path);
	return image;
}

//--------------------------------------------------
// LoadGray
//--------------------------------------------------

/**
 * @brief Load a grayscale image
 * @param path The path to the image
 * @return The image that was loaded
 */
Mat LoadUtils::LoadGray(const std::string& path) 
{
	cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
	if (image.empty()) throw runtime_error("Unable to load image: " + path);
	return image;
}

//--------------------------------------------------
// Load Utils
//--------------------------------------------------

/**
 * @brief Load the stereo frame from images on disk
 * @param left The path to the left frame
 * @param right The path to the right frame
 * @return StereoFrame The stereo frame that we have loaded
 */
StereoFrame * LoadUtils::LoadStereoFrame(const string& left, const string& right) 
{
	auto leftImage = LoadImage(left); auto rightImage = LoadImage(right);
	if (leftImage.rows != rightImage.rows || rightImage.cols != leftImage.cols) throw runtime_error("The left image and the right image needs to be the same size");
	return new StereoFrame(leftImage, rightImage);
}

//--------------------------------------------------
// Load Stereo Calibration
//--------------------------------------------------

/**
 * @brief Load calibration details from disk
 * @param path The path to the calibration details
 * @return StereoCalibration The calibration that has been loaded
 */
StereoCalibration * LoadUtils::LoadStereoCalibration(const string& path) 
{
	auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);

	Mat camera1; reader["camera_1"] >> camera1;
	Mat distortion1; reader["distortion_1"] >> distortion1;
	Mat camera2; reader["camera_2"] >> camera2;
	Mat distortion2; reader["distortion_2"] >> distortion2;
	Mat pose; reader["pose"] >> pose;
	Size size1; reader["image_size_1"] >> size1;
	Size size2; reader["image_size_2"] >> size2;

	return new StereoCalibration(camera1, distortion1, camera2, distortion2, pose, size1, size2);
}

//--------------------------------------------------
// Load calibration
//--------------------------------------------------

/**
 * @brief Add the functionality to load calibration details from disk
 * @param path The path to the calibration file 
 * @return MonoCalibration The calibration details
 */
MonoCalibration * LoadUtils::LoadCalibration(const string& path) 
{
	auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
	if (!reader.isOpened()) throw runtime_error("Unable to open file: " + path);

	Mat camera; reader["camera"] >> camera;
	Mat distortion; reader["distortion"] >> distortion;
	Size size; reader["size"] >> size;

	return new MonoCalibration(camera, distortion, size);
}

//--------------------------------------------------
// Load Depth Frame
//--------------------------------------------------

/**
 * @brief Add the logic to load a given depth frame
|* @param color The color image to load
 * @param depth The depth image to load
 * @return DepthFrame* The resultant depth frame
 */
DepthFrame * LoadUtils::LoadDepthFrame(const string& colorPath, const string& depthPath) 
{
	Mat color = imread(colorPath); Mat depth = imread(depthPath, IMREAD_UNCHANGED);
	return new DepthFrame(color, depth);
}

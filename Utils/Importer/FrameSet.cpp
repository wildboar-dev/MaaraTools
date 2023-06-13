//--------------------------------------------------
// Implementation of class FrameSet
//
// @author: Wild Boar
//
// @date: 2023-03-09
//--------------------------------------------------

#include "FrameSet.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param folder The folder that we are dealing with
 */
FrameSet::FrameSet(const string& folder) : _folder(folder)
{
	_frameCount = InitFrameCount(folder);
}

//--------------------------------------------------
// Getter
//--------------------------------------------------

/**
 * @brief Retrieve the frame with a given image
 * @param index The index of the frame that we want
 * @return Frame * Returns a Frame *
 */
Frame * FrameSet::GetFrame(int index)
{
	// Get the color image - if this is not found, then I will assume that the index is not valid!
	Mat color = GetColor(index); if (color.empty()) return nullptr;

	// Now get the other attributes, and throw an exception if not found!
	Mat depth = GetDepth(index); if (depth.empty()) throw runtime_error("The depth map appears to be missing");
	Mat camera = GetK(index); if (camera.empty()) throw runtime_error("The camera matrix appears to be missing");
	Mat pose = GetPose(index); if (pose.empty()) throw runtime_error("The pose map appears to be missing");

	// Return the result
	return new Frame(index, camera, pose, color, depth);
}

/**
 * @brief Retrieve the next frame within the series
 * @return Frame * Returns a Frame *
 */
Frame * FrameSet::GetNext()
{
	// Get the next frame
	auto frame = GetFrame(_currentIndex);

	// Update the index
	_currentIndex++; if (_currentIndex - _startIndex >= _frameCount) _currentIndex = _startIndex;

	// Return the result;
	return frame;
}

/**
 * @brief Retrieve the pose of the frame only
 * @param index The index of the frame that we want
 * @return Mat Returns a Mat
 */
Mat FrameSet::GetPose(int index)
{
	// Build the path
	auto fileName = stringstream(); 

	// Special case - a negative index indicates that we want the "world pose"
	if (index >= 0) 
	{
		fileName << index << "_transform.yaml";
	}
	else 
	{
		fileName << "world_marker_transform.yaml";
	}

	auto path = NVLib::FileUtils::PathCombine(_folder, fileName.str());
	
	// Load the file
	auto rootNode = YAML::LoadFile(path);

	// Get the Rotation
	auto qw = rootNode["rotation"]["w"].as<double>();
	auto qx = rootNode["rotation"]["x"].as<double>();
	auto qy = rootNode["rotation"]["y"].as<double>();
	auto qz = rootNode["rotation"]["z"].as<double>();
	Mat rotation = NVLib::PoseUtils::Quaternion2Matrix(Vec4d(qw, qx, qy, qz));

	// Get the translation
	auto tx = rootNode["translation"]["x"].as<double>();
	auto ty = rootNode["translation"]["y"].as<double>();
	auto tz = rootNode["translation"]["z"].as<double>();
	auto translation = Vec3d(tx, ty, tz);

	// Return the pose
	return NVLib::PoseUtils::GetPose(rotation, translation);
}

/**
 * @brief Retrieve the camera matrix of the frame only
 * @param index The index of the frame that we want
 * @return Mat Returns a Mat
 */
Mat FrameSet::GetK(int index)
{
	// Build the path
	auto fileName = stringstream(); fileName << index << "_camera_info.yaml";
	auto path = NVLib::FileUtils::PathCombine(_folder, fileName.str());

	// Load the file
	auto rootNode = YAML::LoadFile(path);

	// Load the camera matrix
	auto params = rootNode["K"].as<vector<double>>();

	// Copy the values into the camera matrix
	Mat result = Mat_<double>(3,3); auto resultLink = (double *) result.data;
	for (auto i = 0; i < 9; i++) resultLink[i] = params[i];

	// Return the result
	return result;
}

/**
 * @brief Retrieve the depth map of the frame
 * @param index The index of the frame that we want
 * @return Mat Returns a Matrix
 */
Mat FrameSet::GetDepth(int index)
{
	// Build the path
	auto fileName = stringstream(); fileName << index << "_depth.tif";
	auto path = NVLib::FileUtils::PathCombine(_folder, fileName.str());

	// Load the depth value
	Mat depth = imread(path, IMREAD_UNCHANGED);

	// Return the result
	return depth;
}

/**
 * @brief Retrieve the color image of my frame
 * @param index The index of the frame we are using
 * @return Mat Returns a matrix
 */
Mat FrameSet::GetColor(int index) 
{
	// Build the path
	auto fileName = stringstream(); fileName << index << "_image_color.png";
	auto path = NVLib::FileUtils::PathCombine(_folder, fileName.str());

	// Load the depth value
	Mat color = imread(path);

	// Return the result
	return color;
}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * @brief Find the given file count
 * @param folder The folder that we are updating
 * @return int The number of files that were found
 */
int FrameSet::InitFrameCount(const string& folder) 
{
	// Get the list of file names
	auto fileNames = vector<string>(); NVLib::FileUtils::GetFileList(folder, fileNames);

	// Create a variable to hold the count
	auto counter = 0; _startIndex = INT_MAX;	

	// Count the number of "image" files
	for (auto& fileName : fileNames) 
	{
		auto justFile = NVLib::FileUtils::GetFileName(fileName);
		auto parts = vector<string>(); NVLib::StringUtils::Split(justFile, '_', parts);
		if (parts[1] == "image" && parts[2] == "color.png") 
		{
			if (!NVLib::StringUtils::IsNumeric(parts[0])) continue;

			auto index = NVLib::StringUtils::String2Int(parts[0]);
			if (index < _startIndex) _startIndex = index;
			counter++;
		}
	}

	// Set the last index to start
	_currentIndex = _startIndex;

	// Handle the case that no files were found
	if (_startIndex == INT_MAX) throw runtime_error("No files found!");

	// return the counter
	return counter;
}

//--------------------------------------------------
// Implementation of class PathHelper
//
// @author: Wild Boar
//
// @date: 2023-03-28
//--------------------------------------------------

#include "PathHelper.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param parameters Input parameters into the application
 */
PathHelper::PathHelper(NVLib::Parameters* parameters)
{
	_database = NVL_Utils::ArgReader::ReadString(parameters, "database");
	_dataset = NVL_Utils::ArgReader::ReadString(parameters, "dataset");
}

//--------------------------------------------------
// Folder Generator
//--------------------------------------------------

/**
 * @brief Generate the path to the input folder
 * @return The requested path
 */
string PathHelper::GetFrameFolder()
{
	return GetPath("raw");
}

/**
 * @brief Generate the path to the output folder
 * @return The requested path
 */
string PathHelper::GetMetaFolder()
{
	return GetPath("meta");
}

/**
 * @brief Retrieves the path to the model folder
 * @return The requested path
 */
string PathHelper::GetModelFolder() 
{
	return GetPath("model");
}

/**
 * @brief Retrieves the path to the pose folder
 * @return The requested path
 */
string PathHelper::GetPoseFolder() 
{
	return GetPath("pose");
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * @brief Creates a path within the system
 * @param folder Generates a path to a new folder within the system
 * @return string Returns a string
 */
string PathHelper::GetPath(const string& folder)
{
	auto basePath = NVLib::FileUtils::PathCombine(_database, _dataset);
	return NVLib::FileUtils::PathCombine(basePath, folder);
}
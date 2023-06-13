//--------------------------------------------------
// Implementation of class BackupUtils
//
// @author: Wild Boar
//
// @date: 2021-09-05
//--------------------------------------------------

#include "BackupUtils.h"
using namespace NVLib;

//--------------------------------------------------
// BackupFile
//--------------------------------------------------

/**
 * @brief Makes a copy of a file in a given folder
 * @param folder The path to the folder that we want to back up file to
 * @param filePath The path to the file that we want to make a copy of
 */
void BackupUtils::Backup(const string& folder, const string& filePath)
{
	if (!FileUtils::Exists(filePath)) throw runtime_error("File not found: " + filePath);
	if (!FileUtils::Exists(folder)) FileUtils::AddFolders(folder);

	auto fileName = FileUtils::GetFileName(filePath);
	auto newPath = FileUtils::PathCombine(folder, fileName);

	FileUtils::CopyFile(filePath, newPath);
}

//--------------------------------------------------
// Implementation of class ZipUtils
//
// @author: Wild Boar
//
// @date: 2022-01-06
//--------------------------------------------------

#include "ZipUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Zip
//--------------------------------------------------

/**
 * @brief Perform the zipping of a file
 * @param zipFile The file that we are zipping to
 * @param folder The path to the folder that we are zipping
 */
void ZipUtils::Zip(const string& zipFile, const string& folder)
{
	int error; auto handle = zip_open(zipFile.c_str(), ZIP_CREATE | ZIP_EXCL, &error);
	if (handle == nullptr) 
	{
		zip_error_t ziperror; zip_error_init_with_code(&ziperror, error);
		throw runtime_error("Failed to open output file: " + zipFile + ": " + zip_error_strerror(&ziperror));
	}

	InsertFolder(handle, folder);

	zip_close(handle);
}

/**
 * @brief Add the logic to insert the folder into the zip file
 * @param handle The handle to the zip file
 * @param folder the path to the folder that we are inserting
 */
void ZipUtils::InsertFolder(zip_t * handle, const string& folder) 
{
	auto files = vector<string>(); NVLib::FileUtils::GetFileList(folder, files);
	for (const auto& file : files) 
	{
		if (NVLib::FileUtils::IsFolder(file)) continue; // NOT doink the recursive thing for the mo
		auto source = zip_source_file(handle, file.c_str(), 0, 0);
		if (source == nullptr) throw runtime_error("Failed to add file to zip: " + std::string(zip_strerror(handle)));
		auto fileName = NVLib::FileUtils::GetFileName(file);
		zip_file_add(handle, fileName.c_str(), source, ZIP_FL_UNCHANGED);
	}
}

//--------------------------------------------------
// UnZip
//--------------------------------------------------

/**
 * @brief Extracts a zip file into a folder
 * @param zipFile The file that we want to extract
 * @param folder The folder that we want to extract into
 */
void ZipUtils::UnZip(const string& zipFile, const string& folder)
{
	int error; auto handle = zip_open(zipFile.c_str(), ZIP_RDONLY, &error);
	if (handle == nullptr) 
	{
		zip_error_t ziperror; zip_error_init_with_code(&ziperror, error);
		throw runtime_error("Failed to open output file: " + zipFile + ": " + zip_error_strerror(&ziperror));
	}

	ExtractFolder(handle, folder);

	zip_close(handle);
}

/**
 * @brief Extracts the zip to a given folder
 * @param handle The handle to the zip file
 * @param folder The folder we are extracting to
 */
void ZipUtils::ExtractFolder(zip_t * handle, const string& folder) 
{
	auto entryCount = zip_get_num_entries(handle, 0);
	struct zip_stat stats;

	for (auto i = 0; i < entryCount; i++) 
	{
		if (zip_stat_index(handle, i, 0, &stats) != 0) continue;
		if (NVLib::StringUtils::EndsWith(stats.name, "/")) continue; // Not handling subfolders; 
		auto file = zip_fopen_index(handle, i, 0);
		if (file == nullptr) throw runtime_error("Unable to insert file into zip: " + string(stats.name));
		auto outPath = NVLib::FileUtils::PathCombine(folder, stats.name);
		auto writer = ofstream(outPath);

		auto sum = 0;
		while (sum < stats.size) 
		{
			char buffer[100]; auto readCount = zip_fread(file, buffer, 100);
			writer.write(buffer, readCount);
			sum += readCount;
		}
	
		writer.close(); zip_fclose(file);
	}
}

//--------------------------------------------------
// A set of utilities for performing file handling stuff
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <uuid/uuid.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
using namespace std;

#include "StringUtils.h"

namespace NVLib
{
	class FileUtils
	{
	public:
		static string PathCombine(const string& baseFolder, const string& fileName);
		static string BuildFileName(const string& baseName, int index, const string& extension);
		static string BuildPath(const string& folder, const string& fileTemplate, int index);
		static string GetExtension(const string& fileName);
		static string GetFileName(const string& path);
		static string GetNameWithoutExtension(const string& fileName);
		
		static bool IsFolder(const string& path);
		static bool Exists(const string& fileName);
		static void AddFolder(const string& path);
		static void AddFolders(const string& path);
		static void Remove(const string& path);
		static void RemoveAll(const string& path);
		static void GetFileList(const string& path, vector<string>& fileNames);
		static void WriteFile(const string& fileName, const string& data);
		static string ReadFile(const string& fileName);
		static void CopyFile(const string& source, const string& destination);
		static void MoveFile(const string& source, const string& distination);
		static int GetFileCount(const string& folder);

		static string MakeUniqueName();
		static string MakeTempFolder(const string& basePath);
		static string MakeTempFile(const string& folder, const string& extension);

		static string GetFileHash(const string& path);

	private:
		static string PrepareFolder(const string& folder);
		static string PrepareFile(const string& file);
		static string MessageDigest(const string& s, const EVP_MD* evp_md, size_t digest_length);
	};
}

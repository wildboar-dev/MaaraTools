//--------------------------------------------------
// Implementation of class ParameterLoader
//
// @author: Wild Boar
//
// @date: 2022-02-19
//--------------------------------------------------

#include "ParameterLoader.h"
using namespace NVLib;

//--------------------------------------------------
// Load
//--------------------------------------------------

/**
 * @brief Load a set of parameters from disk
 * @param path The path were the parameter live
 * @return Parameters * Returns a Parameters *
 */
Parameters * ParameterLoader::Load(const string& path)
{
	auto result = new Parameters();

	auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
	auto root = reader.root();

	for (auto node : root) 
	{
		auto name = node.name();
		auto value = node.string();
		result->Add(name, value);
	}

	reader.release();

	return result;
}
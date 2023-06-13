//--------------------------------------------------
// Implementation of class ARFFReader
//
// @author: Wild Boar
//
// @date: 2022-12-23
//--------------------------------------------------

#include "ARFFReader.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param path The path to the file that we are loading
 */
ARFFReader::ARFFReader(const string& path)
{
	// Create a reader for the file
	_reader = new ifstream(path);
	if (!_reader->is_open()) throw runtime_error("Unable to open file: " + path);
	
	// Load up the header
	_header = new ARFFHeader(*_reader);

	// Determine the expected column count
	_columns = _header->GetFields().size();
	if (_header->GetHasOutput()) _columns++;
	if (_header->GetClassLabels().size() > 0) _columns++;

	// Load in class labels for later verification
	for (auto label : _header->GetClassLabels()) _classes.insert(label);
}

/**
 * @brief Main Terminator
 */
ARFFReader::~ARFFReader()
{
	if (_reader->is_open()) _reader->close(); delete _reader;
	delete _header;
}

//--------------------------------------------------
// Read
//--------------------------------------------------

/**
 * @brief Read all the data from a given file
 * @return Mat Returns a Mat
 */
Mat ARFFReader::ReadAll()
{
	Mat result = Read();

	while (true) 
	{
		Mat next = Read(); if (next.empty()) break;
		result.push_back(next);
	}

	return result;
}

/**
 * @brief Read a single record from the given file
 * @return Mat Returns a Mat
 */
Mat ARFFReader::Read()
{
	// Verify the reader
	if (!_reader->is_open()) throw runtime_error("The file is not open");

	// Read in the next line
	auto line = string(); getline(*_reader, line);

	// If empty then we are at the end I guess
	if (line == string()) return Mat();

	// Get the parts and verify the count
	auto parts = vector<string>(); StringUtils::Split(line,',', parts);
	if (parts.size() != _columns) throw runtime_error("The number of numbers dont seem to match the column count");

	// Verify the classes
	if (_classes.size() != 0) 
	{
		auto classValue = StringUtils::String2Int(parts[parts.size() - 1]);
		auto valid = _classes.find(classValue) != _classes.end();
		if (!valid) throw runtime_error("The field lables do not seem to match the header");
	}

	// Build the result
	Mat result = Mat_<double>(1, parts.size()); auto data = (double *) result.data;
	for (auto i = 0; i < result.cols; i++) data[i] = StringUtils::String2Double(parts[i]); 

	// Return the result
	return result;
}

//--------------------------------------------------
// Close
//--------------------------------------------------

/**
 * @brief The logic to close the given file
 */
void ARFFReader::Close()
{
	_reader->close();
}

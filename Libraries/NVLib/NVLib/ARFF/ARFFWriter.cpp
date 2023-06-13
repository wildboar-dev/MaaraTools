//--------------------------------------------------
// Implementation of class ARFFWriter
//
// @author: Wild Boar
//
// @date: 2022-12-23
//--------------------------------------------------

#include "ARFFWriter.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param path The path that we are writing to
 */
ARFFWriter::ARFFWriter(const string& path)
{
	_writer = new ofstream(path);
	if (!_writer->is_open()) throw runtime_error("Unable to open location: " + path);

	_columns = 0; _headerWritten = false;
}

/**
 * @brief Main Terminator
 */
ARFFWriter::~ARFFWriter()
{
	if (_writer->is_open()) _writer->close(); delete _writer;
}

//--------------------------------------------------
// Write
//--------------------------------------------------

/**
 * @brief Add logic to write file header to disk
 * @param header The file header that we are writing
 */
void ARFFWriter::WriteHeader(ARFFHeader& header)
{
	// Verify that this is the first time we are attempting to write the header to disk
	if (_headerWritten) throw runtime_error("The header may only be written once");
	
	// Verify that we have an effective writer
	if (!_writer->is_open()) throw runtime_error("Unable to write to a file that is not open");

	// Validate the writer
	if (header.GetDataName() == string()) throw runtime_error("Relation name is a required field");
	if (header.GetFields().size() == 0) throw runtime_error("At least one input field is required");

	// Determine the expected column count
	_columns = header.GetFields().size();
	if (header.GetHasOutput()) _columns++;
	if (header.GetClassLabels().size() > 0) _columns++;

	// Load in class labels for later verification
	for (auto label : header.GetClassLabels()) _classes.insert(label);

	// Perform the actual header writing
	header.Save(*_writer); _headerWritten = true;
}

/**
 * @brief Write somne data to the file
 * @param data The data that we are writing to disk
 */
void ARFFWriter::Write(const Mat& data)
{
	// Confirm that the header has been written before we try any writing
	if (!_headerWritten) throw runtime_error("Please write the file header first");

	// Confirm that the given file is open
	if (!_writer->is_open()) throw runtime_error("Unable to write to a file that is not open");

	// Confirm that we have the corect column count
	if (data.cols != _columns) throw runtime_error("The provided data does not appear to have the correct column count wrt the header");

	// Loop thru the data and process it
	auto input = (double *) data.data;
	for (auto row = 0; row < data.rows; row++) 
	{
		auto dataVector = vector<double>();
		for (auto column = 0; column < data.cols; column++) 
		{
			auto index = column + row * data.cols;
			dataVector.push_back(input[index]);
		}

		if (_classes.size() > 0 ) 
		{
			auto classValue = (int)dataVector[dataVector.size() - 1];
			if (_classes.find(classValue) == _classes.end()) throw runtime_error("The provided data does not appear to have valid class labels");
		}

		for (auto i = 0; i < dataVector.size(); i++) 
		{
			if (i != 0) FILE() << ",";
			FILE() << fixed << setprecision(12) << dataVector[i];
		}
		FILE() << endl;
	}

}

//--------------------------------------------------
// Close
//--------------------------------------------------

/**
 * @brief Add the logic to close the file
 */
void ARFFWriter::Close()
{
	_writer->close();
}
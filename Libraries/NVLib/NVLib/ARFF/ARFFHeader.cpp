//--------------------------------------------------
// Implementation of class ARFFHeader
//
// @author: Wild Boar
//
// @date: 2022-12-22
//--------------------------------------------------

#include "ARFFHeader.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param reader The reader that we are loading the file from
 */
ARFFHeader::ARFFHeader(istream& reader)
{
	auto line = string();

	_dataName = string();
	_description = string();
	_author = string();
	_created = string();
	_hasOutput = false;
	_recordCount = -1;

	for (auto i = 0; i < 1000; i++) 
	{
		if (reader.eof()) break;
		getline(reader, line);
		ProcessLine(line);
		if (NVLib::StringUtils::StartsWith(line, "@DATA")) break;
	}
}

/**
 * @brief Custom Constructor
 * @param dataName The name of the associated dataset
 * @param author The name of the author of the file
 * @param recordCount The expected record count (zero if unknown)
 */
ARFFHeader::ARFFHeader(const string& dataName, const string& author, int recordCount)
{
	_dataName = dataName; _author = author; _recordCount = recordCount; 
	_description = string();
	_created = StringUtils::GetDateTimeString();
	_hasOutput = false;
}

//--------------------------------------------------
// Save
//--------------------------------------------------

/**
 * @brief Save the header to disk
 * @param writer The stream that we are writing the output to
 */
void ARFFHeader::Save(ostream& writer)
{
	writer << "%----------------------------------------------------" << endl;
	if (_description != string()) writer << "% @ABOUT " << _description << endl;
	writer << "%" << endl;
	if (_author != string()) writer << "% @AUTHOR " << _author << endl;
	if (_created != string()) writer << "% @CREATED " << _created << endl;
	if (_recordCount > 0) writer << "% @ROW_COUNT 22" << endl;
	writer << "%----------------------------------------------------" << endl;

	writer << "@RELATION " << _dataName << endl << endl;

	for (auto& attribute : _fields) 
	{
		writer << "@ATTRIBUTE " << attribute << " NUMERIC" << endl;
	}

	if (_hasOutput) writer << "@ATTRIBUTE output NUMERIC" << endl;
	
	if (_classLabels.size() > 0) 
	{
		writer << "@ATTRIBUTE class {";
		for (auto i = 0; i < _classLabels.size(); i++) 
		{
			if (i > 0) writer << ",";
			writer << _classLabels[i];
		}
		writer << "}" << endl;
	}

	writer << endl << "@DATA" << endl;
}

//--------------------------------------------------
// Loader Process Logic
//--------------------------------------------------

/**
 * @brief Entry point for the line processing logic
 * @param line The line that we are processing 
 */
void ARFFHeader::ProcessLine(const string& line) 
{
	if (StringUtils::StartsWith(line, PREFIX_DESCRIPTION)) ReadDescription(line);
	else if (StringUtils::StartsWith(line, PREFIX_AUTHOR)) ReadAuthor(line);
	else if (StringUtils::StartsWith(line, PREFIX_CREATED)) ReadCreated(line);
	else if (StringUtils::StartsWith(line, PREFIX_ROW_COUNT)) ReadRecordCount(line);
	else if (StringUtils::StartsWith(line, PREFIX_RELATION)) ReadDataName(line);
	else if (StringUtils::StartsWith(line, PREFIX_OUTPUT)) ReadOutputFound(line);
	else if (StringUtils::StartsWith(line, PREFIX_CLASS)) ReadClassEntry(line);
	else if (StringUtils::StartsWith(line, PREFIX_ATTRIBUTE)) ReadAttribute(line);
}

/**
 * @brief Read the name of the data name
 * @param line The line that we are processing
 */
void ARFFHeader::ReadDataName(const string& line) 
{
	_dataName = line.substr(PREFIX_RELATION.size());
}

/**
 * @brief Read the description from the file
 * @param line The line that we are processing
 */
void ARFFHeader::ReadDescription(const string& line) 
{
	_description = line.substr(PREFIX_DESCRIPTION.size());
}

/**
 * @brief Read the name of the author that we are processing
 * @param line The line that we are processing
 */
void ARFFHeader::ReadAuthor(const string& line) 
{
	_author = line.substr(PREFIX_AUTHOR.size());
}

/**
 * @brief Read the creation date
 * @param line The line that we are processing
 */
void ARFFHeader::ReadCreated(const string& line) 
{
	_created = line.substr(PREFIX_CREATED.size());
}

/**
 * @brief Read the number of rows that we are dealing with
 * @param line The line that we are processing
 */
void ARFFHeader::ReadRecordCount(const string& line) 
{
	auto countValue = line.substr(PREFIX_ROW_COUNT.size());
	_recordCount = StringUtils::String2Int(countValue);
}

/**
 * @brief Read the attribute that we should be processing
 * @param line The line that we are processing
 */
void ARFFHeader::ReadAttribute(const string& line) 
{
	auto parts = vector<string>(); StringUtils::Split(line, ' ', parts);
	if (parts.size() != 3) throw runtime_error("Attribute syntax appears to be incorrect within the file");
	_fields.push_back(parts[1]);
}

/**
 * @brief Read the output value that we have from the system
 * @param line The line that we are processing
 */
void ARFFHeader::ReadOutputFound(const string& line) 
{
	_hasOutput = true;
}

/**
 * @brief Read a line describing the discrete classes that we are classifying rows into
 * @param line The line that we are processing
 */
void ARFFHeader::ReadClassEntry(const string& line)
{
	// Create a variable for holding the class list
	auto classList = stringstream();

	// Extract the class list from the line
	bool startFound = false;
	for (auto& character : line) 
	{
		if (startFound && character == '}') break;
		else if (!startFound && character == '{') startFound = true;
		else if (startFound) classList << character;
	}

	// Break the classList into parts
	auto parts = vector<string>(); StringUtils::Split(classList.str(), ',', parts);

	// Add the parts to the class
	for (auto& part : parts) 
	{
		auto label = NVLib::StringUtils::String2Int(part);
		_classLabels.push_back(label);
	}
}
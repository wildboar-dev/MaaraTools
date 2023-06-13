//--------------------------------------------------
// Defines the header for an ARFF file
//
// @author: Wild Boar
//
// @date: 2022-12-22
//--------------------------------------------------

#pragma once

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

#include "../StringUtils.h"

namespace NVLib
{
	class ARFFHeader
	{
	private:

		/* Constant */
		inline static const string PREFIX_DESCRIPTION = "% @ABOUT ";
		inline static const string PREFIX_AUTHOR = "% @AUTHOR ";
		inline static const string PREFIX_CREATED = "% @CREATED ";
		inline static const string PREFIX_ROW_COUNT = "% @ROW_COUNT";
		inline static const string PREFIX_RELATION = "@RELATION ";
		inline static const string PREFIX_CLASS = "@ATTRIBUTE class ";
		inline static const string PREFIX_OUTPUT = "@ATTRIBUTE output ";
		inline static const string PREFIX_ATTRIBUTE = "@ATTRIBUTE";
	
	private:
		string _dataName;
		string _description;
		string _author;
		string _created;
		vector<string> _fields;
		bool _hasOutput;
		vector<int> _classLabels;
		int _recordCount;
	public:
		ARFFHeader(istream& reader);
		ARFFHeader(const string& dataName, const string& author, int recordCount = -1);

		void Save(ostream& writer);

		inline string& GetDescription() { return _description; }
		inline string& GetDataName() { return _dataName; }
		inline string& GetAuthor() { return _author; }
		inline string& GetCreated() { return _created; }
		inline vector<string>& GetFields() { return _fields; }
		inline bool GetHasOutput() { return _hasOutput; }
		inline vector<int>& GetClassLabels() { return _classLabels; }
		inline int& GetRecordCount() { return _recordCount; }

		inline void SetDescription(const string& value) { _description = value; }
		inline void SetDataName(const string& value) { _dataName = value; }
		inline void SetAuthor(const string& value) { _author = value; }
		inline void SetHasOutput(bool value) { _hasOutput = value; }
	private:
		void ProcessLine(const string& line);
		void ReadDataName(const string& line);
		void ReadDescription(const string& line);
		void ReadAuthor(const string& line);
		void ReadCreated(const string& line);
		void ReadRecordCount(const string& line);
		void ReadAttribute(const string& line);
		void ReadOutputFound(const string& line);
		void ReadClassEntry(const string& line);
	};
}

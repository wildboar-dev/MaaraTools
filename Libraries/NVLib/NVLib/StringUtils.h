//--------------------------------------------------
// Defines a set of utilities for the processing of strings
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <algorithm>
#include <chrono>
#include <locale>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

#include "DateTimeUtils.h"

namespace NVLib
{
	class StringUtils
	{
	public:
		static void Split(const string & value, char delimiter, vector<string> & output);
		static void Trim(const string & value, string & output);
		static int Replace(string& value, const string& token, const string& newToken);
		static string ZeroBufferInt(int value, int bufferSize);
		static string BufferString(const string& value, int length, const string& token = " ");
		static string GetTimeString();
		static string GetDateTimeString();
		static string GetDateString();
		static int String2Int(const string& value);
		static double String2Double(const string& value);
		static string Int2String(int value);
		static string Double2String(double value);
		static string ToUpper(const string& value);
		static bool StartsWith(const string& value, const string& token);
		static bool EndsWith(const string& value, const string& token);
		static bool Contains(const string& value, const string& token);
		static bool String2Bool(const string& value);
		static string Bool2String(bool value);
		static string Clone(const string& value);
		static string ToLower(const string& value);
		static string Combine(const string& firstValue, const string& secondValue);
		static string GetBracketText(const string& value, char startBracket, char endBracket);
		static int FindLocations(const string& value, const string& token, vector<int>& locations);
		static string Stream2Text(istream& stream);
		static string AssembleLines(vector<string>& lines);
		static string BuildFieldName(const string& field);
		static void FieldSplit(const string & value, vector<string>&parts);
		static string Indent(int tabCount);
		static bool IsNumeric(const string& value);
	};
}

//--------------------------------------------------
// A set of utilities for dealing with datetime and time_t elements.
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <sstream>
#include <ctime>
#include <iostream>
using namespace std;

#include "StringUtils.h"

namespace NVLib
{
	class DateTimeUtils
	{
	public:
		static time_t GetDateTimeNow();
		static time_t GetDateTime(int day, int month, int year, int hour, int minute, int second);
		static time_t GetDate(int day, int month, int year);
		static time_t GetTime(int hour, int minute, int  second);
		static int ExtractHours(const time_t& value);
		static int ExtractMinutes(const time_t& value);
		static int ExtractSeconds(const time_t value);
		static int ExtractYear(const time_t& value);
		static int ExtractMonth(const time_t& value);
		static int ExtractDay(const time_t& value);

		static string DateTime2String(const time_t& value, const string& format = "%Y-%m-%d %H:%M:%S");
		static string Date2String(const time_t& value, const string& format = "%Y-%m-%d");
		static time_t String2DateTime(const string& value, const string& format = "%Y-%m-%d %H:%M:%S");

		static time_t AddDays(const time_t& value, int days);
	};
}

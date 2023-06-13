//--------------------------------------------------
// Implementation code for DateTimeUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "DateTimeUtils.h"
using namespace NVLib;

//--------------------------------------------------
// GetDateTimeNow
//--------------------------------------------------

/**
 * @brief Get the date time at this moment
 * @return The time at this moment
*/
time_t DateTimeUtils::GetDateTimeNow()
{
	auto timenow = std::chrono::system_clock::now();
	return std::chrono::system_clock::to_time_t(timenow);
}

//--------------------------------------------------
// GetDateTime
//--------------------------------------------------

/**
 * @brief Create a date time
 * @param day The day that we are setting
 * @param month The month that we are setting
 * @param year The year that we are setting
 * @param hour The hour that we are setting
 * @param minute The minute that we are setting
 * @param second The second that we are setting
 * @return Return a time_t
 */
time_t DateTimeUtils::GetDateTime(int day, int month, int year, int hour, int minute, int second)
{
	std::tm timeData; 

	timeData.tm_mday = day == -1 ? 0 : day;
	timeData.tm_mon = month == -1 ? 0 : (month - 1);
	timeData.tm_year = year == -1 ? 1900 : (year - 1900);
	timeData.tm_hour =  hour == -1  ? 0 : hour;
	timeData.tm_min = minute == -1 ? 0 : minute;
	timeData.tm_sec = second == -1 ? 0 : second;
	timeData.tm_isdst = -1;

	return mktime(&timeData);
}

//--------------------------------------------------
// GetDate
//--------------------------------------------------

/**
 * @brief Create a data element
 * @param day The day element
 * @param month The month element
 * @param year The year element
 * @return Return a time_t
 */
time_t DateTimeUtils::GetDate(int day, int month, int year)
{
	return GetDateTime(day, month, year, -1, -1, -1);
}

//--------------------------------------------------
// GetTime
//--------------------------------------------------

/**
 * @brief Create a time element
 * @param hour The hour element
 * @param minute The minute element
 * @param second The second element
 * @return Return a time_t
 */
time_t DateTimeUtils::GetTime(int hour, int minute, int second)
{
	return GetDateTime(-1, -1, -1, hour, minute, second);
}

//--------------------------------------------------
// ExtractHours
//--------------------------------------------------

/**
 * @brief Extract the hours from a time_t
 * @param value Extract the hours from time_t
 * @return Return a int
 */
int DateTimeUtils::ExtractHours(const time_t& value)
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_hour;
}

//--------------------------------------------------
// ExtractMinutes
//--------------------------------------------------

/**
 * @brief Extract minutes from a time_t
 * @param value The time that we are extracting from
 * @return Return a int
 */
int DateTimeUtils::ExtractMinutes(const time_t& value)
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_min;
}

//--------------------------------------------------
// ExtractSeconds
//--------------------------------------------------

/**
 * @brief Extract seconds from a time_t
 * @param value The value that we are extracting from
 * @return Return a int
 */
int DateTimeUtils::ExtractSeconds(const time_t value)
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_sec;
}

//--------------------------------------------------
// ExtractYear
//--------------------------------------------------

/**
 * @brief Extract a year from a time_t
 * @param value The value that we are extracting from
 * @return Return a int
 */
int DateTimeUtils::ExtractYear(const time_t& value)
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_year + 1900;
}

//--------------------------------------------------
// ExtractMonth
//--------------------------------------------------

/**
 * @brief Extract the month field from a time_t
 * param value The time_t we are extracting from
 * @return Return an int
 */
int DateTimeUtils::ExtractMonth(const time_t& value)
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_mon + 1;
}

//--------------------------------------------------
// ExtractDay
//--------------------------------------------------

/**
 * @brief Extract the day field from a time_t 
 * @param value The time_t we are extracting from
 * @return Return an int
 */
int DateTimeUtils::ExtractDay(const time_t& value) 
{
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif

	return timeData.tm_mday;
}

//--------------------------------------------------
// DateTime2String
//--------------------------------------------------

/**
 * @brief converts a time_t into a string with the given format
 * @param value The time_t that we are converting
 * @param format The expected format
 * @return The resultant string 
 */
string DateTimeUtils::DateTime2String(const time_t& value, const string& format) 
{
	char buffer[100]; 
	
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif
	
	strftime(buffer, 100, format.c_str(), &timeData);
	return string(buffer);
}

//--------------------------------------------------
// Date2String
//--------------------------------------------------

/**
 * @brief Convert only the date part to a string
 * @param format The format that we want the date to be
 * @return string The resultant string
 */
string DateTimeUtils::Date2String(const time_t& value, const string& format) 
{
	char buffer[100]; 
	
	time_t tempTime = value;
	std::tm timeData; 

 	#ifdef __unix__   
	 	struct tm * info;
	    info = localtime(&tempTime);
		timeData = *info;
    #else
        auto bt = std::tm();
        localtime_s(&timeData, &tempTime);
    #endif
	
	strftime(buffer, 100, format.c_str(), &timeData);
	return string(buffer);
}

//--------------------------------------------------
// String2DateTime
//--------------------------------------------------

/**
 * @brief converts a string into a time_t
 * @param value The string that we are converting
 * @param format The expected format of the string
 * @return The resultant time_t component
 */
time_t DateTimeUtils::String2DateTime(const string& value, const string& format) 
{
	std::tm tm = {};
	std::stringstream ss(value);
	ss >> get_time(&tm, format.c_str()); tm.tm_isdst = -1;
	if (ss.fail()) throw runtime_error("Date conversion failed: " + value);
	return mktime(&tm);
}

//--------------------------------------------------
// String2DateTime
//--------------------------------------------------

/**
 * @brief Add days to the current time element
 * @param value The value that we are updating
 * @param days The number of days we are add (or subtracting if negative)
 * @return time_t The resultant time
 */
time_t DateTimeUtils::AddDays(const time_t& value, int days) 
{
	const time_t ONE_DAY = 24 * 60 * 60;
	return value + ONE_DAY * days;
}

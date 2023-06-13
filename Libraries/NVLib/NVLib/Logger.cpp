//--------------------------------------------------
// Implementation code for the Logger
//
// @author: Wild Boar
//
// @date: 2021-08-24
//--------------------------------------------------

#include "Logger.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors
//--------------------------------------------------

/**
 * Main Constructor
 * @param consoleLevel The level of messages to log to the console
 * @param fileLevel The level of messages to log to file
 */
Logger::Logger(int consoleLevel, int fileLevel)
{
	_file = nullptr; _startTime = 0; _functionStartTime = 0; _currentFunction = "";
	_consoleLevel = consoleLevel;
	_fileLevel = fileLevel;

#ifdef __unix__
	if (_fileLevel > 0) _file = fopen("log.txt", "w");
#else
	if (_fileLevel > 0) fopen_s(&_file, "log.txt", "w");
#endif
}

/**
 * Terminator for the logger
 */
Logger::~Logger()
{
	if (_file != nullptr) fclose(_file);
}

//--------------------------------------------------
// Log Functionality
//--------------------------------------------------

/**
 * Log a message to the system
 * @param level The level associated with the message
 * @param message The message that we are logging
 */
void Logger::Log(int level, const char * message, ...)
{
	va_list argumentPointer; va_start(argumentPointer, message);
	auto fullMessage = GetTimeStamp() + message + "\n";

#ifdef __unix__
	if (level <= _consoleLevel) vfprintf(stdout, fullMessage.c_str(), argumentPointer);
	if (level <= _fileLevel) vfprintf(_file, fullMessage.c_str(), argumentPointer);
#else
	if (level <= _consoleLevel) vfprintf_s(stdout, fullMessage.c_str(), argumentPointer);
	if (level <= _fileLevel) vfprintf_s(_file, fullMessage.c_str(), argumentPointer);
#endif

	va_end(argumentPointer);
}

/**
 * Log an error to the system  
 */
void Logger::LogError(int level, const char* message, ...) 
{
	char exceptionString[500];
	va_list argumentPointer; va_start(argumentPointer, message);
	auto fullMessage = GetTimeStamp() + message + "\n";

#ifdef __unix__
	if (level <= _consoleLevel) vfprintf(stdout, fullMessage.c_str(), argumentPointer);
	if (level <= _fileLevel) vfprintf(_file, fullMessage.c_str(), argumentPointer);
#else
	if (level <= _consoleLevel) vfprintf_s(stdout, fullMessage.c_str(), argumentPointer);
	if (level <= _fileLevel) vfprintf_s(_file, fullMessage.c_str(), argumentPointer);
#endif
	
	snprintf(exceptionString, 500, fullMessage.c_str(), argumentPointer);
	va_end(argumentPointer);
	throw string(fullMessage);
}


//--------------------------------------------------
// Stop Watch
//--------------------------------------------------

/**
 * Start a counter for recording time taken
 */
void Logger::StartApplication()
{
	if (_consoleLevel < 0) return;
	_startTime = getTickCount();
	cout << GetTimeStamp() << "Starting Application" << endl;
}

/**
 * Shows a time at the end of the application
 */
void Logger::StopApplication()
{
	if (_consoleLevel < 0) return;
	auto endTime = getTickCount();
	auto elapsed = (endTime - _startTime) / getTickFrequency();
	cout << GetTimeStamp() << "Stopping Application - Time Elapsed: " << elapsed << " seconds" << endl;
}

/**
 * Indicates that a function is starting
 * @param functionName The function that we are starting
 */
void Logger::StartFunction(const string& functionName)
{
	if (_consoleLevel < 0) return;
	_currentFunction = functionName;
	_functionStartTime = getTickCount();
	cout << GetTimeStamp() << "Starting function: " << functionName << endl;
}

/**
 * Indicates that a function has come to an end
 */
void Logger::StopFunction()
{
	if (_consoleLevel < 0) return;
	auto endTime = getTickCount();
	auto elapsed = (endTime - _functionStartTime) / getTickFrequency();
	cout << GetTimeStamp() << "Stopping " << _currentFunction << " - Time Elapsed: " << elapsed << " seconds" << endl;
}

//--------------------------------------------------
// Add a time stamp function to the logger
//--------------------------------------------------

/**
 * Add a time stamp to the logger
 * @return The time stamp as a string
 */
string Logger::GetTimeStamp() 
{
	time_t rawtime;
	char buffer[80];

	time(&rawtime); 
	
#ifdef __unix__
	struct tm * timeinfo;
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "[%d-%m-%Y %H:%M:%S] ", timeinfo);
#else
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, sizeof(buffer), "[%d-%m-%Y %H:%M:%S] ", &timeinfo);
#endif

	return string(buffer);
}

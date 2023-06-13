//--------------------------------------------------
// A tool for Logging program execution.
//
// @author: Wild Boar
//
// @date: 2021-08-24
//--------------------------------------------------

#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVLib
{
	class Logger
	{
	private:
		int64 _startTime;
		int64 _functionStartTime;
		string _currentFunction;
		FILE * _file;
		int _consoleLevel;
		int _fileLevel;
	public:
		Logger(int  consoleLevel, int fileLevel = 0);
		~Logger();

		void Log(int level, const char * message, ...);
		void LogError(int level, const char* message, ...);
		void StartApplication();
		void StopApplication();
		void StartFunction(const string & functionName);
		void StopFunction();
	private:
		string GetTimeStamp();
	};
}

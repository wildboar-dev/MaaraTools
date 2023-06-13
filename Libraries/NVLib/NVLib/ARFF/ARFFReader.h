//--------------------------------------------------
// A restricted version of an ARFF file reader
//
// @author: Wild Boar
//
// @date: 2022-12-23
//--------------------------------------------------

#pragma once

#include <unordered_set>
#include <fstream>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "../StringUtils.h"
#include "ARFFHeader.h"

namespace NVLib
{
	class ARFFReader
	{
	private:
		ifstream * _reader;
		ARFFHeader * _header;
		int _columns;
		unordered_set<int> _classes;
	public:
		ARFFReader(const string& path);
		~ARFFReader();

		Mat ReadAll();
		Mat Read();

		void Close();

		inline ARFFHeader * GetHeader() { return _header; }

	protected:
		inline istream& FILE() { return *_reader; }
	};
}

//--------------------------------------------------
// Define a writer for the given system
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
	class ARFFWriter
	{
	private:
		ofstream * _writer;
		int _columns;
		unordered_set<int> _classes;
		bool _headerWritten;
	public:
		ARFFWriter(const string& path);
		~ARFFWriter();

		void WriteHeader(ARFFHeader& header);
		void Write(const Mat& data);

		void Close();
	protected:
		inline ofstream& FILE() { return *_writer; }
	};
}

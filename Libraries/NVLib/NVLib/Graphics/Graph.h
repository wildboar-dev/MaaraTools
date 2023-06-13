//--------------------------------------------------
// Utility: A utility module for generating a graph
//
// @author: Wild Boar
//
// @date: 2022-02-07
//--------------------------------------------------

#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include "GraphSeries.h"

namespace NVLib
{
	class Graph
	{
	private:
		string _title;
		string _vlegend;
		string _hlegend;
		vector<GraphSeries> _series;
		vector<string> _labels;

		int _widthBuffer;
		int _heightBuffer;
		int _hAxisCount;
		int _vAxisCount;

	public:
		Graph(const string& title, const string& vlegend, const string& hlegend);

		void AddSeries(GraphSeries& series);

		Mat Render(const Size& size);

		inline string& GetTitle() { return _title; }
		inline string& GetVlegend() { return _vlegend; }
		inline string& GetHlegend() { return _hlegend; }
		inline vector<GraphSeries>& GetSeries() { return _series; }
		inline vector<string>& GetLabels() { return _labels; }
	private:
		void RenderText(Mat& image, const Point& position, const string& value, double scale = 1, const Scalar& color = Scalar::all(0), int thickness = 1);
		void RenderHText(Mat& image, const Point& position, const string& value, double scale = 1, const Scalar& color = Scalar::all(0), int thickness = 1);
		void RenderGrid(Mat& image, int widthbuffer, int heightBuffer, int rowCount, int columnCount);
		void RenderHAxis(Mat& image, int widthBuffer, int heightBuffer, vector<string>& values);
		void RenderVAxis(Mat& image, int widthBuffer, int heightBuffer, vector<string>& values);
		void RenderKey(Mat& image, vector<GraphSeries>& series);
		void RenderGraph(Mat& image, vector<GraphSeries>& series);

		int GetCenter(int length, int itemLength);
		Size GetTextSize(const string& value, double scale = 1, int thickness = 1);
		void RenderImage(Mat& baseImage, const Point& position, Mat& image);
	};
}

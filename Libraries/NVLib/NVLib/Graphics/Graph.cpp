//--------------------------------------------------
// Implementation of class Graph
//
// @author: Wild Boar
//
// @date: 2022-02-07
//--------------------------------------------------

#include "Graph.h"
using namespace NVLib;

//--------------------------------------------------
// Constructor
//--------------------------------------------------

/**
 * @brief Main cosntructor
 * @param title The title of the graph
 * @param vlegend The vertical legend of the graph
 * @param hlegend The horizontal legend of the graph
 */
Graph::Graph(const string& title, const string& vlegend, const string& hlegend) : _title(title), _vlegend(vlegend), _hlegend(hlegend)
{
	_widthBuffer = 40; _heightBuffer = 50; _vAxisCount = 10; _hAxisCount = 10;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

/**
 * @brief Add a series to the graph
 * @param series Add a series to the graph
 */
void Graph::AddSeries(GraphSeries& series)
{
	_series.push_back(series);
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief Render the graph
 * @param size The size that we want to render the graph as
 * @return Mat Returns a Mat
 */
Mat Graph::Render(const Size& size)
{
	// Create the render image
	Mat result = Mat_<Vec3b>(size); result.setTo(255);

	// Render the title
	auto titleSize = GetTextSize(_title, 1, 2);
	auto titleXCenter = GetCenter(size.width, titleSize.width);
	RenderText(result, Point(titleXCenter, 35), _title, 1, Scalar::all(0), 2);

	// Render the grid
	RenderGrid(result, _widthBuffer, _heightBuffer, _vAxisCount, _hAxisCount);

	// Render the bottom Legend
	auto hlegendSize = GetTextSize(_hlegend, 1);
	auto hlegendXCenter = GetCenter(size.width, hlegendSize.width);
	RenderText(result, Point(hlegendXCenter, size.height - 20), _hlegend, 0.5);

	// Render the Side Legend
	auto vlegendSize = GetTextSize(_vlegend, 1);
	auto vlegendYCenter = GetCenter(size.height, vlegendSize.width);
	RenderHText(result, Point(10, vlegendYCenter), _vlegend, 0.5);

	// Render the key
	RenderKey(result, _series);

	// Render the graph
	RenderGraph(result, _series);

	// Return the result
	return result;
}

/**
 * @brief Render a test string at a given location
 * @param image The image that we are rendering upon
 * @param position The position that we are rendering
 * @param value The string value that we are rendering
 * @param scale The scale that we are rendering at
 * @param color The color that we are rendering
 * @param thickness The thickness of the given font
 */
void Graph::RenderText(Mat& image, const Point& position, const string& value, double scale, const Scalar& color, int thickness) 
{
	putText(image, value, position, FONT_HERSHEY_SIMPLEX, scale, color, thickness, LINE_8);
}

/**
 * @brief Render horizontal text
 * @param image The image that we are rendering upon
 * @param position The position to render the text
 * @param value The sting value that we are rendering
 * @param scale The scale that we are rendering at
 * @param color The color that we are rendering
 * @param thickness The thickness of the given font
 */
void Graph::RenderHText(Mat& image, const Point& position, const string& value, double scale, const Scalar& color, int thickness) 
{
	auto textSize = GetTextSize(value, scale, thickness);
	Mat textImage = Mat_<Vec3b>(textSize); textImage.setTo(255);
	putText(textImage, value, Point(0,9), FONT_HERSHEY_SIMPLEX, scale, color, thickness, LINE_8);
	Mat output; rotate(textImage, output, RotateFlags::ROTATE_90_COUNTERCLOCKWISE);
	RenderImage(image, position, output);
}

/**
 * @brief Render the grid on the screen
 * @param image The image upon which we are rendering
 * @param widthbuffer The gap between the end of the screen and the grid (horizontal)
 * @param heightBuffer The gap between the end of the screen and the grid (vertical)
 * @param rowCount The numer of rows in the grid
 * @param columnCount The number of columns associated with the grid
 */
void Graph::RenderGrid(Mat& image, int widthbuffer, int heightBuffer, int rowCount, int columnCount) 
{
	// Calculate parameters
	auto startX = widthbuffer; auto endX = image.cols - widthbuffer - 100; // Add in an extra for the key
	auto startY = heightBuffer; auto endY = image.rows - heightBuffer;
	auto gridWidth = endX - startX; auto gridHeight = endY - startY;
	auto widthStep = gridWidth / (double)columnCount; auto heightStep = gridHeight / (double)rowCount;

	// Render Vertical Lines
	for (auto i = 0; i < (rowCount + 1); i++) 
	{
		auto y = (int)round(startY + i * heightStep);
		line(image, Point(startX, y), Point(endX, y), Scalar::all(0), 1);
	}

	// Render Horizontal Lines
	for (auto i = 0; i < (columnCount + 1); i++) 
	{
		auto x = (int)round(startX + i * widthStep);
		line(image, Point(x, startY), Point(x, endY), Scalar::all(0), 1);
	}
}

/**
 * @brief Render the key that we want to see on the image
 * @param image The image upon which we are rendering
 * @param position The position to render the key
 * @param series The series to remember
 */
void Graph::RenderKey(Mat& image, vector<GraphSeries>& series) 
{
	// Determine the height
	auto height = 10; 
	for (auto& entry : series) 
	{ 
		auto size = GetTextSize(entry.GetName(), 0.4, 1); 
		height += (size.height + 10);
	}

	// Render the box
	auto boxy = GetCenter(image.rows, height);
	rectangle(image, Rect(image.cols - 120, boxy, 100, height), Scalar::all(0));

	// Render rows
	height = boxy + 10;
	for (auto& entry : series) 
	{ 
		auto size = GetTextSize(entry.GetName(), 0.4, 1); 

		// Render an example line
		auto rowHeight = height + size.height; auto color = Scalar(entry.GetColor()[0], entry.GetColor()[1], entry.GetColor()[2]);
		line(image, Point( image.cols - 115, height + (size.height / 2 + 2)), Point(image.cols-110, height + (size.height / 2 + 2)), color);
		RenderText(image, Point(image.cols-100, rowHeight), entry.GetName(), 0.4, 1);
		height += (size.height + 10);

	}
}

/**
 * @brief Render the actual graph
 * @param image The image that we are rendering upon
 * @param series The series that we are rendering
 */
void Graph::RenderGraph(Mat& image, vector<GraphSeries>& series) 
{
	// Find the range
	auto xmin = series[0].GetValues()[0].x; auto xmax = series[0].GetValues()[0].x;
	auto ymin = series[0].GetValues()[0].y; auto ymax = series[0].GetValues()[0].y;
	for (auto entry : series) 
	{
		for (auto value : entry.GetValues()) 
		{
			xmin = min(xmin, value.x); xmax = max(xmax, value.x);
			ymin = min(ymin, value.y); ymax = max(ymax, value.y);
		}
	}

	// Find
	auto startX = _widthBuffer; auto endX = image.cols - _widthBuffer - 100;
	auto startY = _heightBuffer; auto endY = image.rows - _heightBuffer;

	// Render the line
	for (auto entry : series) 
	{
		auto values = vector<Point2d>();
		for (auto& point : entry.GetValues()) 
		{
			auto x = startX + (point.x - xmin) * (endX - startX) / (xmax - xmin);
			auto y = endY - (point.y - ymin) * (endY - startY) / (ymax - ymin);
			values.push_back(Point2d(x, y));
		}

		auto color = Scalar(entry.GetColor()[0], entry.GetColor()[1], entry.GetColor()[2]);
		for (auto i = 1; i < values.size(); i++) line(image, values[i-1], values[i], color, 2);
	}

}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * @brief Helper method for determining the center
 * @param length The lenght of a side
 * @param itemLength The length of an item
 * @return int The (rounded) coordinate of the center
 */
int Graph::GetCenter(int length, int itemLength) 
{
	auto itemHalf = itemLength * 0.5;
	auto half = length * 0.5;
	auto floatResult = half - itemHalf;
	return (int)round(floatResult);
}

/**
 * @brief Retrieve the size of the associated text string
 * @param value The value of the string that we want the size of
 * @param scale The scale of the given string
 * @param thickness The thickness of the given font
 * @return Size The resultant size that we are calculating
 */
Size Graph::GetTextSize(const string& value, double scale, int thickness) 
{
	int baseline; 
	return getTextSize(value, FONT_HERSHEY_SIMPLEX, scale, thickness, &baseline);
}

/**
 * @brief Defines the functionality to render an image at a given location
 * @param baseImage The base image that we are rendering
 * @param position The position we are rendering the image
 * @param image The image that is being rendered
 */
void Graph::RenderImage(Mat& baseImage, const Point& position, Mat& image) 
{
	auto renderArea = Rect(position.x, position.y, image.cols, image.rows);
	auto subImage = baseImage(renderArea);
	image.copyTo(subImage);
}
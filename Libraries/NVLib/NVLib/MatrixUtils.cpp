//--------------------------------------------------
// Implementation code for MatrixUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "MatrixUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Matrix2Vector
//--------------------------------------------------

/**
 * @brief Convert a matrix into a vector
 * @param matrix The matrix that we are converting
 * @param output The vector that we are converting the Mat to
 */
void MatrixUtils::Matrix2Vector(Mat& matrix, vector<double>& output)
{
	auto data = (double*)matrix.data;

	for (auto row = 0; row < matrix.rows; row++) 
	{
		for (auto column = 0; column < matrix.cols; column++) 
		{
			output.push_back(data[column + row * matrix.cols]);
		}
	}

}

//--------------------------------------------------
// Vector2Matrix
//--------------------------------------------------

/**
 * @brief Convert a vector into a matrix
 * @param rows The number of rows for the matrix
 * @param columns The number of columns for the matrix
 * @param data The vector as a data set
 * @return Return a Mat
 */
Mat MatrixUtils::Vector2Matrix(int rows, int columns, vector<double>& data)
{
	auto size = rows * columns; 
	if (size != data.size()) throw runtime_error("Matrix size does not match the vector");

	Mat result = Mat_<double>(rows, columns);
	auto output = (double*)result.data;

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			auto index = column + row * result.cols;
			output[index] = data[index];
		}	
	}

	return result;
}
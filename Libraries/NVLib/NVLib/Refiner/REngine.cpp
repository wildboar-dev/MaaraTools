//--------------------------------------------------
// Implementation of class REngine
//
// @author: Wild Boar
//
// @date: 2022-04-07
//--------------------------------------------------

#include "REngine.h"
using namespace NVLib;

//--------------------------------------------------
// Find Jacobian
//--------------------------------------------------

/**
 * @brief Retrieve the jacobian
 * @param parameters The parameters that we have the jacobian for
 * @param baseError The base error we are calculating the Jacobian from
 * @param problemId The problem we are getting the Jacobian from
 * @return Mat Returns a Mat
 */
Mat REngine::GetJacobian(Mat& parameters, double baseError, int problemId)
{
	Mat result = Mat_<double>(1, parameters.rows);
	auto pdata = (double *) parameters.data;
	auto output = (double *) result.data;

	for (auto i=0; i < parameters.rows; i++) 
	{
		auto original = pdata[i];
		pdata[i] = pdata[i] + _epsilon;
		auto error = _problem->GetError(parameters, problemId);
		pdata[i] = original;

		output[i] = (error - baseError) / _epsilon;
	}

	return result;
}

//--------------------------------------------------
// Get Error
//--------------------------------------------------

/**
 * @brief The error that we are getting
 * @param parameters The parameters that we are calculate variables
 * @return Mat Returns a Mat
 */
Mat REngine::GetErrors(Mat& parameters)
{
	auto pcount = parameters.rows; 
	Mat result = Mat_<double>(_problem->GetTrainingSize(), 1);
	auto output = (double *) result.data;

	for (auto i = 0; i < result.rows; i++) 
	{
		auto error = _problem->GetError(parameters, i);
		output[i] = error;
	}

	return result;
}

/**
 * @brief Calculate the average error 
 * @param errors The errors that we are calculating
 * @return double The result
 */
Vec2d REngine::GetAveError(Mat& errors) 
{
	assert(errors.rows > 0 && errors.cols == 1);
	auto mean = Scalar(); auto stddev = Scalar();
	meanStdDev(errors, mean, stddev);
	return Vec2d(mean[0], stddev[0]);
}

//--------------------------------------------------
// Iterate
//--------------------------------------------------

/**
 * @brief Iterate a round of the refiner
 * @param parameters The current set of pixels
 * @param errors The associated error vectors
 * @return Mat Returns a Mat
 */
Mat REngine::Iterate(Mat& parameters, Mat& errors)
{
	// Calculate the error vector
	auto rdata = (double *)errors.data;

	// Build up the jacobian
	Mat J; for (auto i = 0; i < _problem->GetTrainingSize(); i++) 
	{
		Mat entry = GetJacobian(parameters, rdata[i], i);
		J.push_back(entry);
	}

	// Determine the update
	Mat u; solve(J, errors, u, DECOMP_SVD);

	// Return the result
	return parameters - u;
}

//--------------------------------------------------
// Minimize
//--------------------------------------------------

/**
 * @brief The fully minimization loop
 * @param parameters The initial parameters to start minimizing from
 * @param maxIterations The list of maximum iterations
 * @param minError The minimum error that indicates we are good enough
 * @return double Returns a double
 */
Vec2d REngine::Minimize(Mat& parameters, int maxIterations, double minError)
{
	auto error = Vec2d(1e6, 1e6);

	for (auto i = 0; i < maxIterations; i++) 
	{
		Mat R = GetErrors(parameters);
		error = GetAveError(R);	
		if (error[0] < minError) break;
		parameters = Iterate(parameters, R);
	}

	return error;
}


//--------------------------------------------------
// Implementation code for CloudUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "CloudUtils.h"
using namespace NVLib;

//--------------------------------------------------
// BuildColorCloud
//--------------------------------------------------

/**
 * Add the functionality to build a color cloud
 * @param camera The camera matrix that we are working with
 * @param color The texture associated with the cloud
 * @param depth The depth associated with the cloud
 * @return Return a Mat
 */
Mat CloudUtils::BuildColorCloud(Mat& camera, Mat& color, Mat& depth)
{
	Mat result = Mat::zeros(color.size(), CV_64FC(6));
	
	auto depthData = (double *)depth.data;
	auto resultData = (double*)result.data;

	auto k = (double *)camera.data;
	auto fx = k[0];
	auto fy = k[4];
	auto cx = k[2];
	auto cy = k[5];

	for (auto row = 0; row < color.rows; row++) 
	{
		for (auto column = 0; column < color.cols; column++) 
		{
			auto index = column + row * color.cols;
			auto Z = depthData[index];
			if (Z == 0) continue;
			
			resultData[index * 6 + 0] = (column - cx) * (Z / fx);
			resultData[index * 6 + 1] = (row - cy) * (Z / fy);
			resultData[index * 6 + 2] = Z;
			resultData[index * 6 + 3] = color.data[index * 3 + 0];
			resultData[index * 6 + 4] = color.data[index * 3 + 1];
			resultData[index * 6 + 5] = color.data[index * 3 + 2];
		}	
	}

	return result;
}

//--------------------------------------------------
// SampleCloud
//--------------------------------------------------

/**
 * Add the functionality to subsample a cloud
 * @param colorCloud The cloud that we are sampling
 * @param step The size of the sample steps
 * @return Return a Mat
 */
Mat CloudUtils::SampleCloud(Mat& colorCloud, int step)
{
	auto expectedWidth = (int)ceil(colorCloud.cols / (double)step);
	auto expectedHeight = (int)ceil(colorCloud.rows / (double)step);

	Mat result = Mat::zeros(expectedHeight, expectedWidth, CV_64FC(6));

	auto input = (double*)result.data;
	auto output = (double*)colorCloud.data;

	for (auto row = 0; row < colorCloud.rows; row += step) 
	{
		for (auto column = 0; column < colorCloud.cols; column += step) 
		{
			auto index1 = column + row * colorCloud.cols;
			auto index2 = (column / step) + (row / step) * expectedWidth;

			for (auto i = 0; i < 6; i++) input[index2 * 6 + i] = output[index1 * 6 + i];		
		}	
	}

	return result;
}

//--------------------------------------------------
// RenderImage
//--------------------------------------------------

/**
 * Render an image of the cloud
 * @param colorCloud The cloud that we are sampling
 * @param camera The camera matrix that we are connecting to
 * @param pose The pose that we want to sample at
 * @param step The associated step scaling
 * @return Return a Mat
 */
Mat CloudUtils::RenderImage(Mat& colorCloud, Mat & camera, Mat& pose, int step)
{
	Mat image = Mat::zeros(colorCloud.size(), CV_8UC3);
	Mat depth = Mat_<double>::zeros(colorCloud.size());

	auto input = (double*)colorCloud.data;
	auto depthData = (double*)depth.data;

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			// Index of the row
			auto index = column + row * image.cols;

			// Retrieve the 3D points
			auto X = input[index * 6 + 0];
			auto Y = input[index * 6 + 1];
			auto Z = input[index * 6 + 2];
			if (Z == 0) continue;

			// Retrieve the Color points
			auto R = (int)input[index * 6 + 3];
			auto G = (int)input[index * 6 + 4];
			auto B = (int)input[index * 6 + 5];

			// Determine the image location
			auto tscene = Math3D::TransformPoint(pose, Point3d(X, Y, Z));
			auto imagePoint = Math3D::Project(camera, tscene);

			// Get the index
			auto u = (int)round(imagePoint.x / step); auto v = (int)round(imagePoint.y / step);
			if (u < 0 || u >= colorCloud.cols || v < 0 || v >= colorCloud.rows) continue;
			auto index2 = u + v * colorCloud.cols;

			// Determine whether we perform an update
			auto currentZ = depthData[index2];
			if (currentZ > 0 && currentZ < Z) continue;

			// Perform updates
			image.data[index2 * 3 + 0] = R;
			image.data[index2 * 3 + 1] = G;
			image.data[index2 * 3 + 2] = B;
			depthData[index2] = Z;
		}
	}

	return image;
}

//--------------------------------------------------
// Project image points
//--------------------------------------------------

/**
 * @brief Calculate the project images points of the point cloud
 * @param camera The given camera matrix
 * @param cloud The cloud that we are getting the image points for
 * @return Mat The resultant point matrix
 */
Mat CloudUtils::ProjectImagePoints(Mat& camera, Mat& cloud) 
{
	Mat result = Mat_<Vec2d>(cloud.size()); result.setTo(Vec2f(-1,-1));

	auto input = (double*)cloud.data;
	auto output = (double *) result.data;

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			// Index of the row
			auto index = column + row * result.cols;

			// Retrieve the 3D points
			auto X = input[index * 6 + 0];
			auto Y = input[index * 6 + 1];
			auto Z = input[index * 6 + 2];
			if (Z <= 0) continue;

			auto imagePoint = Math3D::Project(camera, Point3d(X, Y, Z));

			// Perform updates
			output[index * 2 + 0] =  imagePoint.x;
			output[index * 2 + 1] =  imagePoint.y;
		}
	}

	return result;
}

//--------------------------------------------------
// TransformCloud
//--------------------------------------------------

/**
 * Add the functionality to tranform the location of a point cloud
 * @param colorCloud The cloud that we are transforming
 * @param pose The pose that we are transforming the cloud to
 */
Mat CloudUtils::TransformCloud(Mat& colorCloud, Mat& pose) 
{
	Mat result = Mat::zeros(colorCloud.size(), CV_64FC(6));

	auto cloudData = (double*)colorCloud.data;
	auto poseData = (double*)pose.data;
	auto output = (double*)result.data;

	for (auto row = 0; row < colorCloud.rows; row++)
	{
		for (auto column = 0; column < colorCloud.cols; column++)
		{
			// Index of the row
			auto index = column + row * colorCloud.cols;

			// Retrieve the 3D points
			auto X = cloudData[index * 6 + 0];
			auto Y = cloudData[index * 6 + 1];
			auto Z = cloudData[index * 6 + 2];
			if (Z == 0) continue;

			// Retrieve the Color points
			auto R = (int)cloudData[index * 6 + 3];
			auto G = (int)cloudData[index * 6 + 4];
			auto B = (int)cloudData[index * 6 + 5];

			auto newX = poseData[0] * X + poseData[1] * Y + poseData[2] * Z + poseData[3];
			auto newY = poseData[4] * X + poseData[5] * Y + poseData[6] * Z + poseData[7];
			auto newZ = poseData[8] * X + poseData[9] * Y + poseData[10] * Z + poseData[11];

			output[index * 6 + 0] = newX;
			output[index * 6 + 1] = newY;
			output[index * 6 + 2] = newZ;
			output[index * 6 + 3] = R;
			output[index * 6 + 4] = G;
			output[index * 6 + 5] = B;
		}
	}

	return result;
}

//--------------------------------------------------
// GetVertexCount
//--------------------------------------------------

/**
 * Count the number of vertices in the colorcloud
 * @param colorCloud The color cloud that we are examining
 * @return The number of vertices in the color cloud
 */
int CloudUtils::GetVertexCount(Mat& colorCloud) 
{
	auto cloudData = (double*)colorCloud.data;
	auto counter = 0;

	for (auto row = 0; row < colorCloud.rows; row++)
	{
		for (auto column = 0; column < colorCloud.cols; column++)
		{
			// Index of the row
			auto index = column + row * colorCloud.cols;

			// Retrieve the 3D points
			auto Z = cloudData[index * 6 + 2];
			if (Z != 0) counter++;
		}
	}

	return counter;
}

//--------------------------------------------------
// Save
//--------------------------------------------------

/**
 * Save the given color cloud to disk
 * @param path The path that we are saving to
 * @param colorCloud The color cloud that we are saving
 */
void CloudUtils::Save(const string& path, Mat& colorCloud) 
{
	auto writer = ofstream(path);

	auto vertexCount = GetVertexCount(colorCloud);

	// Write the header
	writer << "ply" << endl;
	writer << "format ascii 1.0" << endl;
	writer << "comment Generated by Neural Vision Ltd" << endl;
	writer << "element vertex " << vertexCount << endl;
	writer << "property float x" << endl;
	writer << "property float y" << endl;
	writer << "property float z" << endl;
	writer << "property uchar red" << endl;
	writer << "property uchar green" << endl;
	writer << "property uchar blue" << endl;
	writer << "end_header" << endl;

	// Write the data
	auto cloudData = (double*)colorCloud.data;
	for (auto row = 0; row < colorCloud.rows; row++)
	{
		for (auto column = 0; column < colorCloud.cols; column++)
		{
			// Index of the row
			auto index = column + row * colorCloud.cols;

			// Retrieve the 3D points
			auto X = cloudData[index * 6 + 0];
			auto Y = cloudData[index * 6 + 1];
			auto Z = cloudData[index * 6 + 2];
			if (Z == 0) continue;

			// Retrieve the Color points
			auto R = (int)cloudData[index * 6 + 3];
			auto G = (int)cloudData[index * 6 + 4];
			auto B = (int)cloudData[index * 6 + 5];

			// Write the PLY entry
			char buffer[100];
			sprintf(buffer, "%f %f %f %i %i %i\n", X, Y, Z, B, G, R);
			writer.write(buffer, strlen(buffer));
		}
	}

	writer.close();
}

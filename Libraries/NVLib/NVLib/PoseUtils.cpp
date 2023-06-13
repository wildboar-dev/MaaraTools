//--------------------------------------------------
// Implementation code for PoseUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "PoseUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Vectors2Pose
//--------------------------------------------------

/**
 * Convert feature vectors into a pose
 * @param rvec The associated rotation vector
 * @param tvec The associated translation vector
 * @return The resultant matrix
 */
Mat PoseUtils::Vectors2Pose(const Vec3d & rvec, const Vec3d & tvec) 
{
	Mat result = Mat::eye(4, 4, CV_64FC1); auto data = (double*)result.data;
	Mat rotation; Rodrigues(rvec, rotation);

	auto rData = (double*)rotation.data;
	for (auto row = 0; row < 3; row++)
	{
		for (auto column = 0; column < 3; column++)
		{
			data[column + row * 4] = rData[column + row * 3];
		}
	}

	data[3] = tvec[0]; data[7] = tvec[1]; data[11] = tvec[2];

	return result;
}

//--------------------------------------------------
// Pose2Vectors
//--------------------------------------------------

/**
 * Convert the associated pose vectors into a rotation vector and a translation vector
 * @param pose The associaed pose
 * @param rvec The rvec that we are updating
 * @param tvec The tvec that we are updating
 */
void PoseUtils::Pose2Vectors(Mat& pose, Vec3d & rvec, Vec3d & tvec) 
{
	Mat rotation = Mat(3, 3, CV_64FC1);
	auto pdata = (double*)pose.data; auto rdata = (double*)rotation.data;

	for (auto row = 0; row < 3; row++) 
	{
		for (auto column = 0; column < 4; column++) 
		{
			auto index = column + row * 4;
			if (column == 3) tvec[row] = pdata[index];
			else rdata[column + row * 3] = pdata[index];		
		}	
	}

	Rodrigues(rotation, rvec);
}

//--------------------------------------------------
// GetPoseTranslation
//--------------------------------------------------

/**
 * Extract the translation component from a rotation matrix
 * @param pose The pose matrix that we are extracting from
 * @return The translation component
 */
Vec3d PoseUtils::GetPoseTranslation(Mat& pose)
{
	auto input = (double*)pose.data;
	return Vec3d(input[3], input[7], input[11]);
}

//--------------------------------------------------
// GetPoseRotation
//--------------------------------------------------

/**
 * Extract the rotation matrix component from a pose matrix
 * @param pose The pose matrix that we are extracting from
 * @return The rotation component
 */
Mat PoseUtils::GetPoseRotation(Mat& pose)
{
	Mat result = Mat(3, 3, CV_64FC1);

	auto input = (double*)pose.data;
	auto output = (double*)result.data;

	for (auto row = 0; row < 3; row++)
	{
		for (auto column = 0; column < 3; column++)
		{
			output[column + row * 3] = input[column + row * 4];
		}
	}

	return result;
}

//--------------------------------------------------
// Quaternion
//--------------------------------------------------

/**
 * @brief The logic to normalize quaternion
 * @param quaternion The quaternion that we are dealing 
 * @return Vec4d The network quaternion
 */
Vec4d PoseUtils::NormalizeQuaternion(const Vec4d& quaternion) 
{
	auto q = quaternion; // alias
	auto scale = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q = q * (1.0 / scale);
	return q;
}

/**
*Find a rotation matrix from a Quaternion
* @param quaternion that we are finding the matrix for[w x y z]
* @return The matrix that has been found
*/
Mat PoseUtils::Quaternion2Matrix(const Vec4d & quaternion)
{
	auto q = quaternion; // alias

	Mat result = Mat_<double>::eye(3, 3);
	auto R = (double*)result.data;

	R[0] = 1 - 2 * q[2] * q[2] - 2 * q[3] * q[3]; R[1] = 2 * q[1] * q[2] - 2 * q[0] * q[3];     R[2] = 2 * q[1] * q[3] + 2 * q[0] * q[2];
	R[3] = 2 * q[1] * q[2] + 2 * q[0] * q[3];     R[4] = 1 - 2 * q[1] * q[1] - 2 * q[3] * q[3]; R[5] = 2 * q[2] * q[3] - 2 * q[0] * q[1];
	R[6] = 2 * q[1] * q[3] - 2 * q[0] * q[2];     R[7] = 2 * q[2] * q[3] + 2 * q[0] * q[1];     R[8] = 1 - 2 * q[1] * q[1] - 2 * q[2] * q[2];

	return result;
}

/**
 * @brief Converts a matrxi into a quaternion
 * @param R The rotation matrix that we are converting
 * @return Return the Quaternion
 */
Vec4d PoseUtils::Matrix2Quaternion(Mat& R)
{
    double trace = R.at<double>(0,0) + R.at<double>(1,1) + R.at<double>(2,2);
	auto Q = Vec4d();

    if (trace > 0.0) 
    {
        double s = sqrt(trace + 1.0);
        Q[3] = (s * 0.5);
        s = 0.5 / s;
        Q[0] = ((R.at<double>(2,1) - R.at<double>(1,2)) * s);
        Q[1] = ((R.at<double>(0,2) - R.at<double>(2,0)) * s);
        Q[2] = ((R.at<double>(1,0) - R.at<double>(0,1)) * s);
    } 
    
    else 
    {
        int i = R.at<double>(0,0) < R.at<double>(1,1) ? (R.at<double>(1,1) < R.at<double>(2,2) ? 2 : 1) : (R.at<double>(0,0) < R.at<double>(2,2) ? 2 : 0); 
        int j = (i + 1) % 3;  
        int k = (i + 2) % 3;

        double s = sqrt(R.at<double>(i, i) - R.at<double>(j,j) - R.at<double>(k,k) + 1.0);
        Q[i] = s * 0.5;
        s = 0.5 / s;

        Q[3] = (R.at<double>(k,j) - R.at<double>(j,k)) * s;
        Q[j] = (R.at<double>(j,i) + R.at<double>(i,j)) * s;
        Q[k] = (R.at<double>(k,i) + R.at<double>(i,k)) * s;
    }

	// Return the Q
	return Vec4d(Q[3], Q[0], Q[1], Q[2]);
}

//--------------------------------------------------
// Euler2Matrix
//--------------------------------------------------

/**
 * Convert a set of Euler angles into a rotation matrix 
 */
Mat PoseUtils::Euler2Matrix(const Vec3d & angles) 
{
	auto theta = Vec3d(Degree2Radian(angles[0]), Degree2Radian(angles[1]), Degree2Radian(angles[2]));

	// Calculate rotation about x axis
    Mat R_x = (Mat_<double>(3,3) <<
               1,       0,              0,
               0,       cos(theta[0]),   -sin(theta[0]),
               0,       sin(theta[0]),   cos(theta[0])
               );

    // Calculate rotation about y axis
    Mat R_y = (Mat_<double>(3,3) <<
               cos(theta[1]),    0,      sin(theta[1]),
               0,               1,      0,
               -sin(theta[1]),   0,      cos(theta[1])
               );

    // Calculate rotation about z axis
    Mat R_z = (Mat_<double>(3,3) <<
               cos(theta[2]),    -sin(theta[2]),      0,
               sin(theta[2]),    cos(theta[2]),       0,
               0,               0,                  1);

    // Combined rotation matrix
    Mat R = R_z * R_y * R_x;

    return R;
}

/**
 * @brief Get Euler angles from a rotation matrix
 * @param R The rotation matrix
 * @return Vec3d The resultant angles
 */
Vec3d PoseUtils::Matrix2Euler(Mat& R) 
{
    auto sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );

    bool singular = sy < 1e-6; // If

    double x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vec3d( Radian2Degree(x), Radian2Degree(y), Radian2Degree(z));
}

//--------------------------------------------------
// GetPose
//--------------------------------------------------

/**
 * Calculate a pose matrix from a seperate rotation and translation
 * @param rotation The rotation component
 * @param translation The translation component
 */
Mat PoseUtils::GetPose(Mat& rotation, Vec3d& translation) 
{
	Mat result = Mat::eye(4, 4, CV_64FC1); auto data = (double*)result.data;

	auto rData = (double*)rotation.data;
	for (auto row = 0; row < 3; row++)
	{
		for (auto column = 0; column < 3; column++)
		{
			data[column + row * 4] = rData[column + row * 3];
		}
	}

	data[3] = translation[0]; data[7] = translation[1]; data[11] = translation[2];

	return result;
}

//--------------------------------------------------
// Radian and Degree conversions
//--------------------------------------------------

/**
 * @brief Converts a degree value into radians
 * @param degrees The degrees
 * @return double The resultant radians
 */
double PoseUtils::Degree2Radian(double degrees) 
{
	return (degrees / 180.0) * M_PI;
}

/**
 * @brief Converts a radian value into a degree
 * @param radians The radians
 * @return double The resultant degrees
 */

double PoseUtils::Radian2Degree(double radians) 
{
	return (radians / M_PI) * 180.0;
}

//--------------------------------------------------
// Simple Pose Generation helpers
//--------------------------------------------------

/**
 * @brief Create a pose from just a translation
 * @param translation The translation that we are creating the pose from
 * @return Mat The resultant pose matrix
 */
Mat PoseUtils::CreateTPose(const Vec3d& translation) 
{
	return (Mat_<double>(4,4) << 1, 0, 0, translation[0], /**/ 0, 1, 0, translation[1], /**/ 0, 0, 1, translation[2], /**/ 0, 0, 0, 1);
}

/**
 * @brief Create a pose from a rotation around the x-axis
 * @param radians The number of radians that we are translating
 * @return Mat The resultant pose matrix
 */
Mat PoseUtils::CreateRX(double radians) 
{
	return (Mat_<double>(4,4) << 1, 0, 0, 0, /**/ 0, cos(radians), -sin(radians), 0, /**/ 0, sin(radians), cos(radians), 0, /**/ 0, 0, 0, 1);
}

/**
 * @brief Create a pose from a rotation around the y-axis
 * @param radians The number of radians that we are translating
 * @return Mat The resultant pose matrix
 */
Mat PoseUtils::CreateRY(double radians) 
{
	return (Mat_<double>(4,4) << cos(radians), 0, sin(radians), 0, /**/ 0, 1, 0, 0, /**/ -sin(radians), 0, cos(radians), 0, /**/ 0, 0, 0, 1);
}

/**
 * @brief Create a pose from a rotation around the z-axis
 * @param radians The number of radians that we are translating
 * @return Mat The resultant pose matrix
 */
Mat PoseUtils::CreateRZ(double radians) 
{
	return (Mat_<double>(4,4) << cos(radians), -sin(radians), 0, 0, /**/ sin(radians), cos(radians), 0, 0, /**/ 0, 0, 1, 0, /**/ 0, 0, 0, 1);
}
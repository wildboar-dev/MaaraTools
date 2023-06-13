//--------------------------------------------------
// Implementation of the utility methods for stereo matching
//
// @author: Wild Boar
//--------------------------------------------------

#include "StereoUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Fundamental Matrix Calculations
//--------------------------------------------------

/**
 * Retrieve the Fundamental matrix from the given set of feature matches
 * @param matches The matches that we are getting the fundamental matrix fro
 * @return The fundamental matrix
 */
Mat StereoUtils::GetFMatrix(const vector<FeatureMatch>& matches)
{
    vector<Point2d> pointSet1; vector<Point2d> pointSet2;
    for (auto match : matches)
    {
        pointSet1.push_back(match.GetPoint1());
        pointSet2.push_back(match.GetPoint2());
    }

    return findFundamentalMat(pointSet1, pointSet2, FM_8POINT);
}

/**
 * Calculate the error associated with the fundamental matrix
 * @param F The fundamental matrix that we are getting the error for
 * @param matches The matches that we are getting the error for
 * @param mask Indicates whether the error is valid or not
 */
Vec2d StereoUtils::FindFError(Mat& F, vector<FeatureMatch>& matches, vector<uchar>& mask)
{
    vector<double> errors;
    for (auto i = 0; i < matches.size(); i++)
    {
        auto valid = mask[i];
        if (valid == 0) continue;
        auto error = StereoUtils::GetSampsonError(F, matches[i]);
        errors.push_back(error);
    }

    vector<double> mean, stddev;
    meanStdDev(errors, mean, stddev);

    return Vec2d(mean[0], stddev[0]);
}

/**
 * Calculate the associated sampson error
 * @param F The current fundamental matrix
 * @param match The match that we are finding the score for
 * @return The error that was calculated
 */
double StereoUtils::GetSampsonError(const Mat& F, FeatureMatch& match)
{
    Mat pointL = (Mat_<double>(3, 1) << match.GetPoint1().x, match.GetPoint1().y, 1);
    Mat pointR = (Mat_<double>(3, 1) << match.GetPoint2().x, match.GetPoint2().y, 1);

    Mat fml = F * pointL; Mat mrTF = pointR.t() * F;

    Mat top = pointR.t() * F * pointL;
    double topSquared = GetSquared(top, 0, 0);

    auto weight = GetSquared(fml, 0, 0) + GetSquared(fml, 1, 0) + GetSquared(mrTF, 0, 0) + GetSquared(mrTF, 0, 1);

    return weight != 0 ? topSquared / weight : 0;
}


/**
 * Retrieve an element from the matrix and square the value
 * @param matrix The matrix that we are getting the value for
 * @param row The row coordinate
 * @param column The column coordinate
 * @return The value squared within the matrix
 */
double StereoUtils::GetSquared(const Mat& matrix, int row, int column)
{
    auto index = column + row * matrix.cols;
    double value = ((double*)matrix.data)[index];
    return  value * value;
}

/**
 * Apply a homography to the given points
 * @param H The current homography
 * @param point The point that we are applying
 */
Point2d StereoUtils::ApplyH(const Mat& H, const Point2f& point)
{
    auto hdata = (double*)H.data;

    auto X = hdata[0] * point.x + hdata[1] * point.y + hdata[2];
    auto Y = hdata[3] * point.x + hdata[4] * point.y + hdata[5];
    auto Z = hdata[6] * point.x + hdata[7] * point.y + hdata[8];

    return Point2d(X / Z, Y / Z);
}

//--------------------------------------------------
// Corresponding points
//--------------------------------------------------

/**
 * @brief Corresponding point calculations
 * @param camera The camera matrix that we are using
 * @param pose The relative pose between images
 * @param point The location of the point in the first image
 * @param Z The depth of the point
 * @return Point2d The location of the point in the next image
 */
Point2d StereoUtils::GetMatchPoint(Mat& camera, Mat& pose, const Point2d& point, double Z) 
{
    // Unproject the point
    auto scenePoint = Math3D::UnProject(camera, point, Z);

    // Transform the point
    auto tscenePoint = Math3D::TransformPoint(pose, scenePoint);

    // project the point back
    return Math3D::Project(camera, tscenePoint);
}
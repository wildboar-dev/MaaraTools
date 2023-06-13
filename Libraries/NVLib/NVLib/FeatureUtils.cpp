//--------------------------------------------------
// Implementation of the utility methods for Feature Points
//
// @author: Wild Boar
//--------------------------------------------------

#include "FeatureUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Find
//--------------------------------------------------

/**
 * Find feature points
 * @param image The image that we are finding features for
 * @param blockSize The size of the blocks that we looking for within the image
 * @param result The resultant feature points that were found.
 */
void FeatureUtils::Find(Mat& image, int blockSize, vector<Point2d>& result)
{
    Ptr<FeatureDetector> detector = FastFeatureDetector::create();
    vector<KeyPoint> keypoints; detector->detect(image, keypoints);

    auto duplicates = unordered_map<int, KeyPoint>();
    for (const KeyPoint & keypoint : keypoints)
    {
        auto key = GetIndex(keypoint.pt, blockSize, image.cols);
        auto match = duplicates.find(key);

        if (match == duplicates.end()) duplicates[key] = keypoint;
        else
        {
            if (duplicates[key].response < keypoint.response) duplicates[key] = keypoint;
        }
    }

    for (const pair<int, KeyPoint>& pair : duplicates) result.push_back(pair.second.pt);
}

/**
 * Find the for the point
 * @param point The point that we are getting index of
 * @param blockSize The size of the block
 * @param width The width of the image
 * @return Return a int
 */
int FeatureUtils::GetIndex(const Point2d& point, int blockSize, int width)
{
    int x = (int)floor(point.x / blockSize); int y = (int)floor(point.y / blockSize);
    return x + y * width;
}

//--------------------------------------------------
// Matching Entry Point
//--------------------------------------------------

/**
 * Fills a vector of feature matches
 * @param frame The frame that we are finding matching points for
 * @param points The initial set of features that we finding matches for
 * @param result The resultant set of matches
 * @return Return a static void
 */
void FeatureUtils::Match(StereoFrame* frame, vector<Point2d>& points, vector<FeatureMatch>& result)
{
    auto inpoints = vector<Point2f>(); for (auto point : points) inpoints.push_back(Point2f(float(point.x), float(point.y)));
    auto matchPoints = vector<Point2f>(); auto status = vector<uchar>(); auto errors = vector<float>();

    calcOpticalFlowPyrLK(frame->GetLeft(), frame->GetRight(), inpoints, matchPoints, status, errors, Size(21, 21), 3, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 9000, 1e-8));

    auto matches = vector<FeatureMatch>();
    for (auto i = 0; i < (int)points.size(); i++)
    {
        if (status[i] != 0 && errors[i] < 9) matches.push_back(FeatureMatch(inpoints[i], matchPoints[i]));
    }

    result.clear(); if (matches.size() < 4) return;
    EpipolarFilter(matches, result);
}

/**
 * Filter matches
 * @param input The set of feature matchs that we are filering
 * @param output The output list of matches
 */
void FeatureUtils::EpipolarFilter(vector<FeatureMatch>& input, vector<FeatureMatch>& output)
{
    vector<Point2f> points_1; vector<Point2f> points_2;

    for (auto match : input) { points_1.push_back(match.GetPoint1()); points_2.push_back(match.GetPoint2()); }

    auto outliers = vector<uchar>();
    auto F = findFundamentalMat(points_1, points_2, FM_LMEDS, 1.0, 0.8, outliers);

    for (auto i = 0; i < outliers.size(); i++) if (outliers[i]) output.push_back(input[i]);
}

//--------------------------------------------------
// GetScenePoints
//--------------------------------------------------

/**
 * @brief Add the logic to get the associated scene points
 * @param camera The camera matrix
 * @param frame The frame we are working with
 * @param features The list of features
 * @param output The output scene points
 */
void FeatureUtils::GetScenePoints(Mat& camera, DepthFrame * frame, vector<FeatureMatch>& matches, vector<Point3d>& outScene, vector<Point2d>& outImage) 
{
    outScene.clear(); outImage.clear();

    for (auto& match : matches) 
    {
        auto Z = Math3D::ExtractDepth(frame->GetDepth(), match.GetPoint1());
        if (Z <= 10) continue;

        auto scenePoint = Math3D::UnProject(camera, match.GetPoint1(), Z);

        outScene.push_back(scenePoint); outImage.push_back(match.GetPoint2());
    }
}

//--------------------------------------------------
// FindPose
//--------------------------------------------------

/**
 * @brief Defines the logic to find the given pose
 * @param camera The camera matrix
 * @param scenePoints The list of scene points
 * @param imagePoints The list of image points
 * @return Mat The resultant pose matrix
 */
Mat FeatureUtils::FindPose(Mat& camera, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    // find the pose        
    Vec3d rvec, tvec; solvePnPRansac(scenePoints, imagePoints, camera, Mat(), rvec, tvec, false, 1000, 3, 0.95, noArray(), SOLVEPNP_ITERATIVE);

    // return the result
    return PoseUtils::Vectors2Pose(rvec, tvec);
}

//--------------------------------------------------
// FindPoseError
//--------------------------------------------------

/**
 * @brief Defines how accurate the pose we are working with are
 * @param camera The camera matrix
 * @param pose The pose matrix
 * @param scenePoints The list of scene points
 * @param imagePoints The list of image points
 * @return double The error that we are working with
 */
double FeatureUtils::FindPoseError(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    auto estimated = vector<Point2d>(); 
    for (auto i = 0; i < scenePoints.size(); i++) 
    {
        auto& scenePoint = scenePoints[i];
        auto tpoint = Math3D::TransformPoint(pose, scenePoint);
        auto imagePoint = Math3D::Project(camera, tpoint);
        estimated.push_back(imagePoint);
    }

    auto total = 0.0;
    for (auto i = 0; i < estimated.size(); i++) 
    {
        auto xDiff = imagePoints[i].x - estimated[i].x;
        auto yDiff = imagePoints[i].y - estimated[i].y;
        auto length = sqrt(xDiff * xDiff + yDiff * yDiff);
        total += length;
    }
 
    return total / (int)estimated.size();
}
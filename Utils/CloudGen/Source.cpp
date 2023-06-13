//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2023-04-03
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Math3D.h>
#include <NVLib/SaveUtils.h>
#include <NVLib/PoseUtils.h>
#include <NVLib/Model/Model.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ArgReader.h"
#include "PathHelper.h"
#include "Frame.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);
Mat LoadCameraMatrix(const string& folder); 
unique_ptr<NVL_App::Frame> LoadFrame(const string& folder, int index);
Mat LoadPose(const string& folder, int index);
void SaveModel(const string& folder, Mat& camera, Mat& pose, NVL_App::Frame * frame);

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 * @param parameters The input parameters
 */
void Run(NVLib::Parameters * parameters) 
{
    // Verify that we have some input parameters
    if (parameters == nullptr) return; auto logger = NVLib::Logger(1);

    logger.StartApplication();
    
    logger.Log(1, "Determining path locations");
    auto pathHelper = NVL_App::PathHelper(parameters);
    auto frameFolder = pathHelper.GetFrameFolder();
    auto metaFolder = pathHelper.GetMetaFolder();
    auto modelFolder = pathHelper.GetModelFolder();
    auto poseFolder = pathHelper.GetPoseFolder();

    logger.Log(1, "Determining the camera matrix");
    Mat camera = LoadCameraMatrix(metaFolder);
    logger.Log(1, "Focal length: %f", ((double *) camera.data)[0]);

    logger.Log(1, "Determining the index of the file to process");
    auto fileId = NVL_Utils::ArgReader::ReadInteger(parameters, "file_id");

    logger.Log(1, "Create a model folder if there is none");
    if (NVLib::FileUtils::Exists(modelFolder)) NVLib::FileUtils::RemoveAll(modelFolder);
    NVLib::FileUtils::AddFolder(modelFolder);

    logger.Log(1, "Processing Frame: %i", fileId);

    logger.Log(1, "Loading the world pose");
    Mat worldPose = LoadPose(poseFolder, -1);
    if (worldPose.empty()) 
    {
        logger.Log(1, "No world pose found! Assuming identity matrix");
        worldPose = Mat_<double>::eye(4,4);
    }

    logger.Log(1, "Loading frame pose");
    Mat pose = LoadPose(poseFolder, fileId);
    if (pose.empty()) throw runtime_error("Pose not found for this frame");
    pose = worldPose * pose;

    auto frame = LoadFrame(frameFolder, fileId);
    SaveModel(modelFolder, camera, pose, frame.get());

    logger.StopApplication();
}

//--------------------------------------------------
// Loaders
//--------------------------------------------------

/**
 * @brief Load the given camera matrix
 * @param folder The "frames" folder
 * @return Mat The camera matrix that has been loaded
 */
Mat LoadCameraMatrix(const string& folder) 
{
    // The path to the calibration file
    auto path = NVLib::FileUtils::PathCombine(folder, "calibration.xml");

    // Open the given reader
    auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);

    // Load the given camera matrix
    Mat camera; reader["camera"] >> camera;
    if (camera.empty()) throw runtime_error("Failed to load the camera matrix from: " + path);

    // Release the reader
    reader.release();

    // Return the result
    return camera;
}

/**
 * @brief Add the functionality to load a pose from disk
 * @param folder The folder that we are loading from
 * @param index the index of the pose that we want
 * @return Mat The pose matrix
 */
Mat LoadPose(const string& folder, int index) 
{
    // Create the file name
    auto filename = stringstream(); 
    
    // If the pose is negative - then load the world pose
    if (index >= 0) 
    {
        filename << "pose_" << setw(4) << setfill('0') << index << ".xml";
    }
    else 
    {
        filename << "pose_world.xml";
    }

    // Create the path
    auto path = NVLib::FileUtils::PathCombine(folder, filename.str());

    // Open a reader
    auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
    if (!reader.isOpened()) return Mat();

    // Read in the pose
    Mat pose; reader["pose"] >> pose;

    // Close the reader
    reader.release();

    // Return the result
    return pose;
}

/**
 * @brief Load a frame from disk
 * @param path The path were the files are located
 * @param index The index that we are loading
 * @return Frame * Returns a Frame *
 */
unique_ptr<NVL_App::Frame> LoadFrame(const string& folder, int index)
{
	// Generate the file names
	auto colorFile = stringstream(); colorFile << "color_" << setw(4) << setfill('0') << index << ".png";
	auto depthFile = stringstream(); depthFile << "depth_" << setw(4) << setfill('0') << index << ".tiff";
	
	// Create the associated paths
	auto colorPath = NVLib::FileUtils::PathCombine(folder, colorFile.str());
	auto depthPath = NVLib::FileUtils::PathCombine(folder, depthFile.str());
	
	// Load the files
	Mat color = imread(colorPath); if (color.empty()) throw runtime_error("Unable to load: " + colorPath);
	Mat depth = imread(depthPath, IMREAD_UNCHANGED); if (depth.empty()) throw runtime_error("Unable to load: " + depthPath);

	// Return the result
	return unique_ptr<NVL_App::Frame>(new NVL_App::Frame(index, color, depth));
}

//--------------------------------------------------
// Save Logic
//--------------------------------------------------

/**
 * @brief The save logic 
 * @param folder The folder that we are saving in
 * @param camera The camera matrix
 * @param rotateInfo The rotation information
 * @param frame The frame that we are saving
 */
void SaveModel(const string& folder, Mat& camera, Mat& pose, NVL_App::Frame * frame) 
{
    // Create the associated links
    auto dlink = (float *) frame->GetDepth().data;

    // Fix the naming convention
    auto filename = stringstream(); filename << "model_" << setw(4) << setfill('0') << frame->GetIndex() << ".ply";
    auto path = NVLib::FileUtils::PathCombine(folder, filename.str());

    // Get the image size
    auto size = frame->GetColor().size();

    // Create a model to hold the result
    auto model = NVLib::Model();

    // Add the points to the model
    for (auto row = 0; row < size.height; row++) 
    {
        for (auto column = 0; column < size.width; column++) 
        {
            auto index = column + row * size.width;

            auto Z = dlink[index]; if (Z <= 0 || Z > 1) continue;

            auto scenePoint = NVLib::Math3D::UnProject(camera, Point2d(column, row), Z);
            auto tscenePoint = NVLib::Math3D::TransformPoint(pose, scenePoint);

            auto c1 = frame->GetColor().data[index * 3 + 0];
            auto c2 = frame->GetColor().data[index * 3 + 1];
            auto c3 = frame->GetColor().data[index * 3 + 2];

            model.AddVertex(tscenePoint, Vec3i(c1, c2, c3));
        }
    }

    // Save the model
    NVLib::SaveUtils::SaveModel(path, &model);
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incoming arguments
 * @param argv The number of incoming arguments
 * @return SUCCESS and FAILURE
 */
int main(int argc, char ** argv) 
{
    NVLib::Parameters * parameters = nullptr;

    try
    {
        parameters = NVL_Utils::ArgReader::GetParameters(argc, argv);
        Run(parameters);
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    if (parameters != nullptr) delete parameters;

    return EXIT_SUCCESS;
}

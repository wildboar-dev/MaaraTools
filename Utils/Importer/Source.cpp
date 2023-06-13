//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2023-04-05
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/Logger.h>
#include <NVLib/FileUtils.h>

#include "ArgReader.h"
#include "FrameSet.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);
string CreateFolders(const string& database, const string& folder);
string BuildInputPath(const string& database, const string& folder);
void SaveCameraMatrix(const string& folder, Mat& camera);
void SaveFrame(const string& folder, NVL_App::Frame * frame);
void SavePose(const string & folder, NVL_App::Frame * frame);
void SaveWorldPose(const string& folder, Mat& pose);

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

    logger.Log(1, "Load in the input parameters");
    auto database = NVL_Utils::ArgReader::ReadString(parameters, "database");
    auto folder = NVL_Utils::ArgReader::ReadString(parameters, "mfolder");
    auto count = NVL_Utils::ArgReader::ReadInteger(parameters, "file_count");

    logger.Log(1, "Generating the folder locations");
    auto outputFolder = CreateFolders(database, folder);

    logger.Log(1, "Creating a frameset element");
    auto inputFolder = BuildInputPath(database, folder);
    auto frameset = NVL_App::FrameSet(inputFolder);

    logger.Log(1, "Preparing to perform loop thru frames");
    auto firstFrame = true;

    logger.Log(1, "Processing Frames");
    for (auto i = 0; i < count; i++) 
    {
        logger.Log(1, "Processing Frame: %i", i);

        auto frame = frameset.GetNext();
        if (frame == nullptr) 
        {
            logger.Log(1, "Frame missing: %i", i);
            continue;
        }

        if (firstFrame) 
        {
            SaveCameraMatrix(outputFolder, frame->GetCamera());
            firstFrame = false;
        }
        
        SaveFrame(outputFolder, frame);
        SavePose(outputFolder, frame);
    }

    logger.Log(1, "Saving the world transform");
    Mat worldPose = frameset.GetPose(-1);
    SaveWorldPose(outputFolder, worldPose);

    logger.StopApplication();
}

//--------------------------------------------------
// Folder handler methods
//--------------------------------------------------

/**
 * @brief Add the logic to create the associated folders
 * @param database The database location
 * @param folder The folder that I want to create
 * @return Return the base path
 */
string CreateFolders(const string& database, const string& folder) 
{
    auto path = NVLib::FileUtils::PathCombine(database, folder);
    if (NVLib::FileUtils::Exists(path)) throw runtime_error("Cannot proceed, the folder already exists");
    NVLib::FileUtils::AddFolders(path);

    auto rawFolder = NVLib::FileUtils::PathCombine(path, "raw");
    NVLib::FileUtils::AddFolder(rawFolder);
    
    auto metaFolder = NVLib::FileUtils::PathCombine(path, "meta");
    NVLib::FileUtils::AddFolder(metaFolder);

    auto poseFolder = NVLib::FileUtils::PathCombine(path, "pose");
    NVLib::FileUtils::AddFolder(poseFolder);

    return path;
}

/**
 * @brief Add the functionality to deal with folders
 * @param database The path to the database
 * @param folder The input folder that we care about
 * @return string The string representing the input folder
 */
string BuildInputPath(const string& database, const string& folder) 
{
    auto inputFolder = NVLib::FileUtils::PathCombine(database, "00_import");
    return NVLib::FileUtils::PathCombine(inputFolder, folder);
}

//--------------------------------------------------
// Save Helpers
//--------------------------------------------------

/**
 * @brief Add the logic to save the frame to disk
 * @param folder The folder that I am saving to
 * @param camera The camera that I am saving
 */
void SaveCameraMatrix(const string& folder, Mat& camera) 
{
    // Defines the path that I am saving to
    auto metaFolder = NVLib::FileUtils::PathCombine(folder, "meta");
    auto path = NVLib::FileUtils::PathCombine(metaFolder, "calibration.xml");

    // Create a writer
    auto writer = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::WRITE);

    // Write the output camera matrix
    writer << "camera" << camera;

    // Close the writer
    writer.release();
}

/**
 * @brief Save a given frame to disk
 * @param folder The folder that we are saving to
 * @param frame The frame that we are saving
 */
void SaveFrame(const string& folder, NVL_App::Frame * frame) 
{
    // Defines the folder that we are saving to
    auto rawFolder = NVLib::FileUtils::PathCombine(folder, "raw");

    // Create the file names
    auto colorFile = stringstream(); colorFile << "color_" << setw(4) << setfill('0') << frame->GetIndex() << ".png";
    auto depthFile = stringstream(); depthFile << "depth_" << setw(4) << setfill('0') << frame->GetIndex() << ".tiff";

    // Create the save paths
    auto colorPath = NVLib::FileUtils::PathCombine(rawFolder, colorFile.str());
    auto depthPath = NVLib::FileUtils::PathCombine(rawFolder, depthFile.str());

    // Write the results
    imwrite(colorPath, frame->GetColor()); imwrite(depthPath, frame->GetDepth());
}

/**
 * @brief Save the associated pose to disk
 * @param folder The folder that we are saving to
 * @param frame The frame that we are saving
 */
void SavePose(const string & folder, NVL_App::Frame * frame) 
{
    // Defines the path that I am saving to
    auto poseFolder = NVLib::FileUtils::PathCombine(folder, "pose");
    auto fileName = stringstream(); fileName << "pose_" << setw(4) << setfill('0') << frame->GetIndex() << ".xml";
    auto path = NVLib::FileUtils::PathCombine(poseFolder, fileName.str());

    // Create a writer
    auto writer = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::WRITE);

    // Write the output camera matrix
    writer << "pose" << frame->GetPose();

    // Close the writer
    writer.release();
}

/**
 * @brief Add the logic to save the world pose to disk
 * @param folder The folder that we are writing to
 * @param pose The pose that we are saving
 */
void SaveWorldPose(const string& folder, Mat& pose) 
{
    // Defines the path that I am saving to
    auto poseFolder = NVLib::FileUtils::PathCombine(folder, "pose");
    auto fileName = stringstream(); fileName << "pose_world.xml";
    auto path = NVLib::FileUtils::PathCombine(poseFolder, fileName.str());

    // Create a writer
    auto writer = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::WRITE);

    // Write the output camera matrix
    writer << "pose" << pose;

    // Close the writer
    writer.release();
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

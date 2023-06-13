#--------------------------------------------------------
# CMake for generating the NVLib
#
# @author: Wild Boar
#
# Date Created: 2021-08-24
#--------------------------------------------------------

# Add OpenCV to the solution
find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(OpenSSL REQUIRED)

# Create Library
add_library (NVLib STATIC
	ARFF/ARFFHeader.cpp
	ARFF/ARFFReader.cpp
	ARFF/ARFFWriter.cpp
	Graphics/Graph.cpp
	Parameters/Parameters.cpp
	Parameters/ParameterLoader.cpp
	Model/Model.cpp
	Refiner/REngine.cpp
	Odometry/FastDetector.cpp
	Odometry/FastTracker.cpp
	DateTimeUtils.cpp
	Math2D.cpp
	Math3D.cpp
	MatrixUtils.cpp
	StringUtils.cpp
	Logger.cpp
	PoseUtils.cpp
	RandomUtils.cpp
	DrawUtils.cpp
	FileUtils.cpp
	FeatureUtils.cpp
	LoadUtils.cpp
	DisplayUtils.cpp
	RectangleUtils.cpp
	ImageUtils.cpp
	ConvertUtils.cpp
	CmdLineUtils.cpp
	BackupUtils.cpp
	EscapeUtils.cpp
	Colors.cpp
	ZipUtils.cpp
	PlaneUtils.cpp
	SaveUtils.cpp
	CloudUtils.cpp
	StereoUtils.cpp
	PlyLoader.cpp
	Email.cpp
	GeneralUtils.cpp
)

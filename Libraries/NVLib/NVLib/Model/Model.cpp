//--------------------------------------------------
// Implementation of class Model
//
// @author: Wild Boar
//
// @date: 2022-02-15
//--------------------------------------------------

#include "Model.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 */
Model::Model()
{
	// Additional implementation can go here
}

//--------------------------------------------------
// Update
//--------------------------------------------------

/**
 * @brief Add a vertex to the system
 * @param vertex The vertex that we are adding
 */
void Model::AddVertex(ColorPoint& vertex)
{
	_vertices.push_back(vertex);
}

/**
 * @brief Add a vertex to the model
 * @param vertex The vertex that we are adding
 * @param color The color the vertex
 */
void Model::AddVertex(const Point3d& vertex, const Vec3i& color) 
{
	_vertices.push_back(ColorPoint(vertex, color));
}

/**
 * @brief Add a set of vertices to the system
 * @param points The points that we are adding
 * @param color The color of the points that are being added
 */
void Model::AddVertices(vector<Point3d>& points, const Vec3i& color) 
{
	for (auto& point : points) AddVertex(point, color);
}

//--------------------------------------------------
// Transform
//--------------------------------------------------

/**
 * @brief Apply a transformation to the point locations 
 * @param transform The transformation that we are applying
 */
void Model::Transform(Mat& transform) 
{
	auto t = (double *) transform.data;

	for (auto& vertex : _vertices) 
	{
		auto p = vertex.GetLocation();

		auto X = p.x * t[0] + p.y * t[1] + p.z * t[2] + t[3];
		auto Y = p.x * t[4] + p.y * t[5] + p.z * t[6] + t[7];
		auto Z = p.x * t[8] + p.y * t[9] + p.z * t[10] + t[11];

		vertex.GetLocation().x = X;  vertex.GetLocation().y = Y; vertex.GetLocation().z = Z;
	}
}

//--------------------------------------------------
// Retrieve
//--------------------------------------------------

/**
 * @brief Retrieve the number of vertices that have been added
 * @return int Returns a int
 */
int Model::VertexCount()
{
	return (int)_vertices.size();
}
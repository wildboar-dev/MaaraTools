//--------------------------------------------------
// Implementation of class Scene
//
// @author: Wild Boar
//
// @date: 2022-02-15
//--------------------------------------------------

#include "Scene.h"
using namespace NVLib;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 */
Scene::Scene()
{
	// Extra implementation can go here
}

/**
 * @brief Main Terminator
 */
Scene::~Scene()
{
	for (auto& model : _models) delete model;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

/**
 * @brief Add a new model to the scene
 * @param model The 3D model that we are adding
 * @param pose The pose of the model given the coordinate space of the scene
 */
void Scene::AddModel(Model * model, Mat& pose)
{
	model->Transform(pose); _models.push_back(model);
}

//--------------------------------------------------
// Retrieve
//--------------------------------------------------

/**
 * @brief Retrieve the number of vertices that have been added
 * @return int Returns a int
 */
int Scene::VertexCount()
{
	auto result = 0;
	for (auto& model : _models) result += model->VertexCount();
	return result;
}

/**
 * @brief The number of models that have been added to the scene
 * @return int Returns a int
 */
int Scene::ModelCount()
{
	return (int) _models.size();
}
//--------------------------------------------------
// Model: A scene consisting of a set of 3D models
//
// @author: Wild Boar
//
// @date: 2022-02-15
//--------------------------------------------------

#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include "Model.h"

namespace NVLib
{
	class Scene
	{
		private:
			vector<Model *> _models;
		public:
			Scene();
			~Scene();

			void AddModel(Model * model, Mat& pose);

			int VertexCount();
			int ModelCount();

			inline vector<Model *>& GetModels() { return _models; }
	};
}

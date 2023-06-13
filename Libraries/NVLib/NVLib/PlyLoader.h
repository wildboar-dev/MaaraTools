//--------------------------------------------------
// A set of utilities for working with PLY 3D model files
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

#include "Model/ColorPoint.h"

#include "StringUtils.h"

namespace NVLib
{
	class PlyLoader
	{
	public:
		static void Load(const string& path, vector<ColorPoint *>& vertices, vector< vector<int> >& indices);
	private:
		static string ExtractHeaderText(istream& reader);
		static int GetVertexCount(const string& headerText);
		static int GetIndexCount(const string& headerText);
		static void FillVertices(istream& reader, int vertexCount, vector<ColorPoint *>& vertices);
		static void FillIndices(istream& reader, int indexCount, vector< vector<int> >& indices);
	};
}

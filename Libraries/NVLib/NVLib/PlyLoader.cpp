//--------------------------------------------------
// Implementation code for PlyLoader
//
// @author: Wild Boar
//--------------------------------------------------

#include "PlyLoader.h"
using namespace NVLib;

//--------------------------------------------------
// Load
//--------------------------------------------------

/**
 * @brief Load a ply file from disk and return vertices and indices
 * @param path The path to the ply file that we want to load
 * @param vertices The vertices of the PLY file that we are loading
 * @param indices The indices of the faces that make up the ply file
 */
void PlyLoader::Load(const string& path, vector<ColorPoint *>& vertices, vector< vector<int> >& indices)
{
	auto reader = ifstream(path);
	if (!reader.is_open()) throw runtime_error("Unable to open: " + path);

	auto headerText = ExtractHeaderText(reader);
	auto vertexCount = GetVertexCount(headerText);
	auto indexCount = GetIndexCount(headerText);

	FillVertices(reader, vertexCount, vertices);
	FillIndices(reader, indexCount, indices);

	reader.close();
}

/**
 * @brief Extract the header text from a stream
 * @param reader The reader of the stream
 * @return The header text that we have extracted
*/
string PlyLoader::ExtractHeaderText(istream& reader) 
{
	auto result = stringstream(); auto line = string();

	while (getline(reader, line)) 
	{
		result << line << endl;
		if (line == "end_header") break;
	}
	
	return result.str();
}

/**
 * @brief Get the vertex count from the header text
 * @param headerText The header text that we are getting the vertex count for
 * @return The value associated with the vertex count tag
*/
int PlyLoader::GetVertexCount(const string& headerText) 
{
	auto reader = stringstream(headerText);
	auto line = string();

	while (getline(reader, line)) 
	{
		if (StringUtils::StartsWith(line, "element vertex")) 
		{
			auto parts = vector<string>();
			StringUtils::Split(line, ' ', parts);
			if (parts.size() != 3) throw runtime_error("element vertex clause does not appear valid");
			return StringUtils::String2Int(parts[2]);
		}
	}

	return 0;
}

/**
 * @brief Get the index count from the header text
 * @param headerText The header text that we are extracting from
 * @return The value associated with the index count tag
*/
int PlyLoader::GetIndexCount(const string& headerText) 
{
	auto reader = stringstream(headerText);
	auto line = string();

	while (getline(reader, line))
	{
		if (StringUtils::StartsWith(line, "element face"))
		{
			auto parts = vector<string>();
			StringUtils::Split(line, ' ', parts);
			if (parts.size() != 3) throw runtime_error("element vertex clause does not appear valid");
			return StringUtils::String2Int(parts[2]);
		}
	}

	return 0;
}

/**
 * @brief Get the vertices and place them into the vector
 * @param reader The reader that we are getting the values for
 * @param vertexCount The number of vertices that we have
 * @param vertices The vertices that we are filling
*/
void PlyLoader::FillVertices(istream& reader, int vertexCount, vector<ColorPoint *>& vertices) 
{
	for (auto i = 0; i < vertexCount; i++) 
	{
		auto line = string(); getline(reader, line);
		auto tline = string(); StringUtils::Trim(line, tline);
		auto parts = vector<string>();
		StringUtils::Split(tline, ' ', parts);
		if (parts.size() < 6) throw runtime_error("Invalid vertex line!");

		auto x = StringUtils::String2Double(parts[0]);
		auto y = StringUtils::String2Double(parts[1]);
		auto z = StringUtils::String2Double(parts[2]);

		auto red = (unsigned char)StringUtils::String2Int(parts[3]);
		auto green = (unsigned char)StringUtils::String2Int(parts[4]);
		auto blue = (unsigned char)StringUtils::String2Int(parts[5]);

		vertices.push_back(new ColorPoint(x, y, z, red, green, blue));
	}
}

/**
 * @brief Get the indices and place them into the vector
 * @param reader The reader that we are getting the values for
 * @param indexCount The number of indices that we have
 * @param indices The indices that we are filling
*/
void PlyLoader::FillIndices(istream& reader, int indexCount, vector< vector<int> >& indices) 
{
	for (auto i = 0; i < indexCount; i++)
	{
		auto line = string(); getline(reader, line);
		auto tline = string();  StringUtils::Trim(line, tline);
		auto parts = vector<string>();
		StringUtils::Split(tline, ' ', parts);
		if (parts.size() <= 1) throw runtime_error("Invalid index line!");

		indices.push_back(vector<int>());

		for (auto j = 1; j < parts.size(); j++) 
		{
			indices[i].push_back(StringUtils::String2Int(parts[j]));		
		}
	}
}
//--------------------------------------------------
// Implementation of class Parameters
//
// @author: Wild Boar
//
// @date: 2022-02-19
//--------------------------------------------------

#include "Parameters.h"
using namespace NVLib;

//--------------------------------------------------
// Insert
//--------------------------------------------------

/**
 * @brief Insert parameters into the collection
 * @param key The key that we are adding
 * @param value The value that we are adding
 */
void Parameters::Add(const string& key, const string& value)
{
	_parameters[key] = value;
}

//--------------------------------------------------
// Retrieval
//--------------------------------------------------

/**
 * @brief Retrieve a parameter from the collection
 * @param key The key that we are adding
 * @return string Returns a string
 */
string Parameters::Get(const string& key)
{
	if (!Contains(key)) throw runtime_error("Unable to find the key: " + key);
	return _parameters[key];
}

/**
 * @brief Retrieve the number of parameters that are in the collection
 * @return int Returns a int
 */
int Parameters::Count()
{
	return (int) _parameters.size();
}

/**
 * @brief Check to see if a given parameter in the collection
 * @param key The key that we are adding
 * @return bool Returns a bool
 */
bool Parameters::Contains(const string& key)
{
	return _parameters.find(key) != _parameters.end();
}

/**
 * @brief Retrieves the set of keys that make up the parameters 
 * @param keys The output keys list that we are updating
 */
void Parameters::GetKeys(vector<string>& keys) 
{
	keys.clear(); // make sure that the keys list is empty
	for (auto pair : _parameters) keys.push_back(pair.first);
}
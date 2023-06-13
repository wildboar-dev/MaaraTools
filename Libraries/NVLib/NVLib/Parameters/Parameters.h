//--------------------------------------------------
// Model: A collection of incomming parameters
//
// @author: Wild Boar
//
// @date: 2022-02-19
//--------------------------------------------------

#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

namespace NVLib
{
	class Parameters
	{
	private:
		unordered_map<string, string> _parameters;
	public:
		Parameters() {}

		void Add(const string& key, const string& value);

		string Get(const string& key);
		int Count();
		bool Contains(const string& key);

		void GetKeys(vector<string>& keys);
	};
}

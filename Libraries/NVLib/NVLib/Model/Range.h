//--------------------------------------------------
// A generic container for holding ranges
//
// @author: Wild Boar
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

namespace NVLib
{
	template <typename T>
	class Range
	{
	private:
		T _min;
		T _max;
	public:
		Range(T min, T max) : _min(min), _max(max) {}

		inline T GetMin() const { return _min; }
		inline T GetMax() const { return _max; }
	};
}

//--------------------------------------------------
// Implementation of class GeneralUtils
//
// @author: Wild Boar
//
// @date: 2023-03-09
//--------------------------------------------------

#include "GeneralUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Swap
//--------------------------------------------------

/**
 * @brief Perform a swap of two variables
 * @param value1 The first value that we are swapping
 * @param value2 The second value that we are swapping
 */
void GeneralUtils::Swap(float& value1, float& value2)
{
	auto temp = value1; value1 = value2; value2 = temp;
}

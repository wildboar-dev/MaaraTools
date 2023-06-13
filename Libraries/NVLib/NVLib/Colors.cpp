//--------------------------------------------------
// Color Implementations
//
// @author: Wild Boar
//
// @date: 2021-08-27
//--------------------------------------------------

#include "Colors.h"
using namespace NVLib;

//--------------------------------------------------
// Constant initializations
//--------------------------------------------------

const Vec3i Colors::RED = Vec3i(0,0,255);
const Vec3i Colors::GREEN = Vec3i(0,255,0);
const Vec3i Colors::BLUE = Vec3i(255,0,0);
const Vec3i Colors::BLACK = Vec3i(0,0,0);
const Vec3i Colors::WHITE = Vec3i(255,255,255);

//--------------------------------------------------
// Method Declaration
//--------------------------------------------------

/**
 * @brief Defines the convert method
 * @param color The incomming color
 * @return Scalar The outgoing scalar
 */
Scalar Colors::Convert(const Vec3i& color) 
{
    return Scalar(color[0], color[1], color[2]);
}
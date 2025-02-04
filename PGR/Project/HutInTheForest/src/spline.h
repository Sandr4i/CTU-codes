/**
 * \file	spline.h
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#ifndef SPLINE_H
#define SPLINE_H

#include "pgr.h"

/**
* \ brief Control points of the curve animation
*/
extern glm::vec3 controlPoints[];
/**
* \ brief Number of control points
*/
extern const size_t controlPointsCount;

/**
* \ brief Check if vectror has length of 0
*/
bool isVectorNull(glm::vec3& vector);

/**
* \ brief Transform (rotate/move) current coordinates to the given parameters
*/
glm::mat4 transformObject(glm::vec3& posiition, glm::vec3& front, glm::vec3& up);

/**
* \ brief Evaluate position on Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegment(glm::vec3 &P0, glm::vec3 &P1, glm::vec3 &P2, glm::vec3 &P3, float t);

/**
* \ brief Evaluate 1st Derivative of the Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegment_1stDerivative(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, float t);

/**
* \ brief Evaluate position of the Catmull_rom closed curve segment
*/
glm::vec3 evaluateClosedCurve(size_t count, glm::vec3 points[], float t);

/**
* \ brief Evaluate 1st Derivative of the Catmull-Rom closed curve segment
*/
glm::vec3 evaluateClosedCurve_1stDerivative(size_t count, glm::vec3 points[], float t);


#endif



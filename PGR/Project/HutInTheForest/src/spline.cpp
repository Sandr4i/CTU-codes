/**
 * \file	spline.cpp
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#include "spline.h"
/**
* \ brief Control points of the curve animation
*/
glm::vec3 controlPoints[] = {
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(4.0, -1.5, 0.0),
	glm::vec3(4.0, 1.0, -1.0),
	glm::vec3(0.0, 0.0, -1.0),
	glm::vec3(-4.0, -1.5, -1.0),
	glm::vec3(2.0, 1.5, 1.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(-4.0, 1.5, 0.0),
	glm::vec3(-4.0, -1.0, 1.0),
	glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(4.0, 1.5, 1.0),
	glm::vec3(-2.0, -1.5, -1.0),
};
/**
* \ brief Number of control points
*/
const size_t controlPointsCount = 12;

/**
* \ brief Check if vectror has length of 0
*/
bool isVectorNull(glm::vec3& vector) {
	return !vector.x && !vector.y && !vector.z;
}

/**
* \ brief Transform (rotate/move) current coordinates to the given parameters
*/
glm::mat4 transformObject(glm::vec3& position, glm::vec3& front, glm::vec3& up) {
	glm::vec3 vectorZ = -glm::normalize(front);
	if(isVectorNull(vectorZ)) vectorZ = glm::vec3(0.0, 0.0, 1.0);

	glm::vec3 vectorX = glm::normalize(glm::cross(up, vectorZ));
	if (isVectorNull(vectorX)) vectorX = glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 vectorY = glm::cross(vectorZ, vectorX);
	glm::mat4 matrix = glm::mat4(
		vectorX.x, vectorX.y, vectorX.z, 0.0,
		vectorY.x, vectorY.y, vectorY.z, 0.0,
		vectorZ.x, vectorZ.y, vectorZ.z, 0.0,
		position.x, position.y, position.z, 1.0
	);

	return matrix;
}


/**
* \ brief Evaluate position on Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegment(glm::vec3 &P0, glm::vec3 &P1, glm::vec3 &P2, glm::vec3 &P3, float t) {
	glm::vec3 ret = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 matrix(
		-1, 3, -3, 1,
		2, -5, 4, -1,
		-1, 0, 1, 0,
		0, 2, 0, 0
	);

	glm::vec4 tvec = glm::vec4(t * t * t, t * t, t, 1) * 0.5f;
	glm::vec4 vec = tvec * glm::transpose(matrix);
	ret += (P0 * vec.x) + (P1 * vec.y) + (P2 * vec.z) + (P3 * vec.a);
	
	return ret;
}

/**
* \ brief Evaluate 1st Derivative of the Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegment_1stDerivative(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, float t) {
	glm::vec3 ret = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 matrix(
		-1, 3, -3, 1,
		2, -5, 4, -1,
		-1, 0, 1, 0,
		0, 2, 0, 0
	);

	glm::vec4 tvec = glm::vec4(t * t * 3, t * 2, 1, 0) * 0.5f;
	glm::vec4 vec = tvec * glm::transpose(matrix);
	ret += (P0 * vec.x) + (P1 * vec.y) + (P2 * vec.z) + (P3 * vec.a);

	return ret;
}

/**
* \ brief Evaluate position of the Catmull_rom closed curve segment
*/
glm::vec3 evaluateClosedCurve(size_t count, glm::vec3 points[], float t) {
	glm::vec3 ret = glm::vec3(0.0, 0.0, 0.0);
	size_t s = floor(t);
	ret = evaluateCurveSegment(points[(count + s - 1) % count], points[s % count], points[(s + 1) % count], points[(s + 2) % count], fmod(t, 1));
	
	return ret;
}

/**
* \ brief Evaluate 1st Derivative of the Catmull-Rom closed curve segment
*/
glm::vec3 evaluateClosedCurve_1stDerivative(size_t count, glm::vec3 points[], float t) {
	glm::vec3 ret = glm::vec3(1.0, 0.0, 0.0);
	size_t s = floor(t);
	ret = evaluateCurveSegment_1stDerivative(points[(count + s - 1) % count], points[s % count], points[(s + 1) % count], points[(s + 2) % count], fmod(t, 1));

	return ret;
}
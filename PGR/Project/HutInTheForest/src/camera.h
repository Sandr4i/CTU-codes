/**
 * \file	camera.h
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "pgr.h"
#include "render_stuff.h"
#include "spline.h"

#include <vector>
#include <iostream>

/**
* \brief collision box structure
*/
typedef struct {
	glm::vec3 position;
	glm::vec3 maxCoord;
	glm::vec3 minCoord;

	bool colliderShape; //True for square; false for sphere
	float size;

} Box;

const float cameraSpeed = 1.85f;

enum cameraMovement {
	FORWARD, BACKWARD, RIGHT,  LEFT
};

/**
* \brief camera data and functions
*/
class Camera {
public:
	//camera data
	glm::vec3 position;
	glm::vec3 centerPosition;
	glm::vec3 horizontalPosition;
	glm::vec3 worldUp;
	glm::vec3 up;

	//camera angles
	float yawAngle;
	float pitchAngle;

	float startTime;
	float currentTime;
	std::vector<Box*> objects; //Array of objects that collide with camera

	/**
	* \brief Camera constructor
	*/
	Camera(glm::vec3 position) {
		this->position = position;
		this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->objects = std::vector<Box*>();
		this->yawAngle = 0.0f;
		this->pitchAngle = 0.0f;

		update();
	}

	/**
	* \brief Returns view matrix
	*/
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, position + centerPosition, up);
	}

	void CameraMouseInputManager(float x, float y) {
		x *= CAM_SENSIVITY;
		y *= CAM_SENSIVITY;

		yawAngle += x;
		pitchAngle += y;

		if (yawAngle > 360.0f) yawAngle -= 360.0f;
		if (yawAngle < 0.0f) yawAngle += 360.0f;
		if (pitchAngle > 90.0f) pitchAngle = 90.0f;
		if (pitchAngle < -90.0f) pitchAngle = -90.0f;

		update();
	}

	/**
	* \brief Camera keyboard input management 
	*/
	void cameraKeyboardInputManager(cameraMovement direction, float deltaTime) {
		float speed = cameraSpeed * deltaTime;
		glm::vec3 previousPosition = position;

		switch (direction) {
			case FORWARD:
				position += centerPosition * speed;
				break;
			case BACKWARD:
				position -= centerPosition * speed;
				break;
			case LEFT:
				position -= horizontalPosition * speed;
				break;
			case RIGHT:
				position += horizontalPosition * speed;
				break;
		}
		for (Box* object : objects) {
			if (checkCollision(object, position)) position = previousPosition;
		}

		if (position.x < -3.0f) position.x = -3.0f;
		else if (position.x > 10.0f) position.x = 10.0f;

		if (position.y < -0.7f) position.y = -0.7f;
		else if (position.y > 7.0f) position.y = 7.0f;

		if (position.z < -9.0f) position.z = -9.0f;
		else if (position.z > 8.0f) position.z = 8.0f;

		update();
	}

	/**
	* \brief Function to check if there is a collision between camera and object
	*/
	bool checkCollision(Box* box, glm::vec3 position) {
		bool ret = false;
		if (box->colliderShape) {
			glm::vec3 coord1 = box->position + box->size * box->maxCoord;
			glm::vec3 coord2 = box->position + box->size * box->minCoord;
			ret = (position.x - 0.1 < coord1.x && position.x + 0.1 > coord2.x)
				&& (position.y - 0.1 < coord1.y && position.y + 0.1 > coord2.y)
				&& (position.z - 0.1 < coord1.z && position.z + 0.1 > coord2.z);
		}
		else {
			ret = (pow((position.x - box->position.x), 2)
				+ pow((position.y - box->position.y), 2)
				+ pow((position.z - box->position.z), 2) < pow((0.1 + box->size), 2));
		}
		return ret;
	}

private:
	/**
	* \brief Function to calculate a new vectors
	*/
	void update() {
		glm::vec3 front;
		front.x = cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		front.y = sin(glm::radians(pitchAngle));
		front.z = sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));

		centerPosition = glm::normalize(front);
		horizontalPosition = glm::normalize(glm::cross(centerPosition, worldUp));
		up = glm::normalize(glm::cross(horizontalPosition, centerPosition));
	}
};

#endif
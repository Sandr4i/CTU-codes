/**
 * \file	main.cpp
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#include <time.h>
#include <list>

#include "data.h"
#include "pgr.h"

#include "render_stuff.h"
#include "spline.h"
#include "camera.h"
#include "trees_positions.h"

#include <fstream>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;
/**
* \brief Shader programs for objects
*/
extern Shader shaderProgram;
extern Shader crowShaderProgram;
extern Shader flameShaderProgram;
extern Shader boatShaderProgram;
extern Shader frogShaderProgram;
extern SkyboxShaderProgram skyboxShaderProgram;
extern WaterShaderProgram waterShaderProgram;
extern WaterBlendShaderProgram waterBlendShaderProgram;

extern bool useLight;

typedef list<void*> Trees;
int maxTrees = 69;

float win_width = WIN_WIDTH;
float win_height = WIN_HEIGHT;
float crowHeight = 2.0f;
float crowAlpha = 0.5f;

/**
* \brief Structure that contains information about scene objects
*/
struct SceneObjects {
	BaseObject* terrain;
	BaseObject* hut;
	BaseObject* boat;
	BaseObject* campfire;
	BaseObject* flame;
	BaseObject* bench;
	BaseObject* wood;
	BaseObject* largeWood;
	BaseObject* frog;
	BaseObject* crow;
	BaseObject* chunk;
	RainBannerObject* rainBanner;
	WaterObject* waterAnimation;
	WaterObject* waterBlend;
	Trees trees;
} sceneObjects;

/** 
* \brief Structure that contain s information about current scene state
*/
struct SceneState {
	int winWidth;
	int winHeight;
	float time; //Elapsed time

	bool keys[KEYS_COUNT];

	Camera* camera;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	int camView;
	bool cameraFree;

	//Light 
	bool dayLight;
	bool refelctor;
	bool campfireLight;
	bool fog = false;

	bool chunkPicked;
	bool campfireLit;
	bool rain;

	glm::quat crowRotation;
	float crowSpeed;

} sceneState;

/**
* \brief Function to switch between different camera's positions
*/
void switchCamera() {
	glm::vec3 eye, dist, up;
	switch (sceneState.camView) {
	/**
	* \brief Static camera
	*/
	case 0:
		dist = glm::vec3(1, 0.8, 4);
		eye = glm::vec3(3, -0.5, -1.4);
		up = glm::vec3(0, 1, 0);

		sceneState.viewMatrix = glm::lookAt(eye, eye + dist, up);
		sceneState.camera->position = eye;
		sceneState.camera->centerPosition = dist;
		sceneState.camera->up = up;
		sceneState.camera->horizontalPosition = glm::normalize(glm::cross(dist, up));
		break;
	
	/**
	* \brief Dynamic camera from the crow point of view
	*/
	case 1:
		sceneState.camera->up = glm::vec3(4, glm::tan(20) * 2 + 2.0f, 0) - sceneObjects.crow->position;
		break;
	/**
	* \brief Dynamic camera from a spline curve
	*/
	case 2:
		sceneState.camera->startTime = sceneState.time;
		break;
	/**
	* \brief Static camera
	*/
	default:
		dist = glm::vec3(-3, -3, 4);
		eye = glm::vec3(6, 4, -3.8);
		up = glm::vec3(0, 1, 0);

		sceneState.viewMatrix = glm::lookAt(eye, eye + dist, up);
		sceneState.camera->position = eye;
		sceneState.camera->centerPosition = dist;
		sceneState.camera->up = up;
		sceneState.camera->horizontalPosition = glm::normalize(glm::cross(dist, up));
		break;
	}
}

/**
* \brief Function that plant trees in scene
*/
void setTrees() {
	while (!sceneObjects.trees.empty()) {

		delete sceneObjects.trees.back();
		sceneObjects.trees.pop_back();
	}
	for (int i = 0; i < maxTrees; i++) {

		BaseObject* tree = new BaseObject();
		tree->position = trees_positions[i];
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dis(0.7f, 1.5f);
		float value = dis(gen);

		tree->size = TREE_SIZE * value;
		tree->x = 0.0f;
		tree->y = 0.0f;
		tree->z = 0.0f;

		sceneObjects.trees.push_back(tree);
	}
}

/**
* \brief Function that create a random color for frog object
*/
void newFrogColor() {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);

	float r = dis(gen);
	float g = dis(gen);
	float b = dis(gen);
	glm::vec3 newColor = glm::vec3(r, g, b);

	glUseProgram(frogShaderProgram.program);
	GLint frogColor = glGetUniformLocation(frogShaderProgram.program, "frogColor");
	if (frogColor != -1) {
		glUniform3fv(frogColor, 1, &newColor[0]);
	}
}

/**
* \brief Function that creates a water object
*/
WaterObject* createWaterAnimation(const glm::vec3& position) {
	WaterObject* water = new WaterObject;

	water->position = position;
	water->speed = 0.0f; 
	water->startTime = sceneState.time;
	water->elapsedTime = water->startTime;
	water->direction = glm::vec3(0.0f, 1.0f, 0.0f);
	water->frameDuration = 0.2f;
	water->testureFrames = 16;
	water->size = WATER_SIZE;

	return water;
}

/**
* \brief Function that creates a water blend object
*/
WaterObject* createWaterBlend(const glm::vec3& position) {
	WaterObject* water = new WaterObject;

	water->position = position;
	water->speed = 0.0f;
	water->startTime = sceneState.time;
	water->elapsedTime = water->startTime;
	water->direction = glm::vec3(0.0f, 1.0f, 0.0f);
	water->frameDuration = 0.05f;
	water->testureFrames = 16;
	water->size = WATER_SIZE;

	return water;
}

/**
* \brief Function that delete objects in the end of scene
*/
void deleteObjects() {
	if (sceneObjects.terrain) {
		delete sceneObjects.terrain;
		sceneObjects.terrain = NULL;
	}

	if (sceneObjects.hut) {
		delete sceneObjects.hut;
		sceneObjects.hut = NULL;
	}

	while (!sceneObjects.trees.empty()) {
		delete sceneObjects.trees.back();
		sceneObjects.trees.pop_back();
	}
}

/**
* \brief Function that (re)start the program
*/
void restart() {
	deleteObjects();
	glutReshapeWindow(win_width, win_height);
	std::vector<BaseObject*> cameraObjects;

	sceneState.time = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);

	/**
	* \brief initialize objects
	*/
	if (sceneObjects.terrain == NULL) sceneObjects.terrain = new BaseObject;
	sceneObjects.terrain->position = glm::vec3(0.0f, 0.91f, 0.0f);
	sceneObjects.terrain->size = TERRAIN_SIZE;
	sceneObjects.terrain->x = 0.0f;
	sceneObjects.terrain->y = 0.0f;
	sceneObjects.terrain->z = 0.0f;
	
	if (sceneObjects.hut == NULL) sceneObjects.hut = new BaseObject;
	sceneObjects.hut->position = glm::vec3(-1.2f, -0.02f, 2.1f); 
	sceneObjects.hut->size = HUT_SIZE;
	sceneObjects.hut->x = 0.0f;
	sceneObjects.hut->y = 120.0f;		
	sceneObjects.hut->z = 0.0f;

	if (sceneObjects.campfire == NULL) sceneObjects.campfire = new BaseObject;
	sceneObjects.campfire->position = glm::vec3(4.4f, -0.73f, 1.9f);  
	sceneObjects.campfire->size = CAMPFIRE_SIZE;
	sceneObjects.campfire->x = 0.0f;
	sceneObjects.campfire->y = 0.0f;			
	sceneObjects.campfire->z = 0.0f;

	if (sceneObjects.flame == NULL) sceneObjects.flame = new BaseObject;
	sceneObjects.flame->position = glm::vec3(4.4f, -0.60f, 1.9f); 
	sceneObjects.flame->size = FLAME_SIZE;
	sceneObjects.flame->x = 0.0f;
	sceneObjects.flame->y = 0.0f;			
	sceneObjects.flame->z = 0.0f;

	if (sceneObjects.bench == NULL) sceneObjects.bench = new BaseObject;
	sceneObjects.bench->position = glm::vec3(5.4f, -0.57f, 3.1f); 
	sceneObjects.bench->size = BENCH_SIZE;
	sceneObjects.bench->x = 0.0f;
	sceneObjects.bench->y = -57.0f;			
	sceneObjects.bench->z = 0.0f;

	if (sceneObjects.boat == NULL) sceneObjects.boat = new BaseObject;
	sceneObjects.boat->position = glm::vec3(4.0f, -0.94f, -2.1f);  
	sceneObjects.boat->size = BOAT_SIZE;
	sceneObjects.boat->x = 0.0f;
	sceneObjects.boat->y = 0.0f;			
	sceneObjects.boat->z = 5.0f;

	if (sceneObjects.wood == NULL) sceneObjects.wood = new BaseObject;
	sceneObjects.wood->position = glm::vec3(-0.8f, -0.87f, 0.7f); 
	sceneObjects.wood->size = WOOD_SIZE;
	sceneObjects.wood->x = 0.0f;
	sceneObjects.wood->y = 0.0f;			
	sceneObjects.wood->z = 0.0f;

	if (sceneObjects.largeWood == NULL) sceneObjects.largeWood = new BaseObject;
	sceneObjects.largeWood->position = glm::vec3(-0.85f, -0.8f, 0.25f); 
	sceneObjects.largeWood->size = LARGE_WOOD_SIZE;
	sceneObjects.largeWood->x = 0.0f;
	sceneObjects.largeWood->y = 0.0f;			
	sceneObjects.largeWood->z = 0.0f;

	if (sceneObjects.crow == NULL) sceneObjects.crow = new BaseObject;
	sceneObjects.crow->size = CROW_SIZE;
	sceneObjects.crow->x = 0.0f;
	sceneObjects.crow->y = 0.0f;			
	sceneObjects.crow->z = 0.0f;

	if (sceneObjects.frog == NULL) sceneObjects.frog = new BaseObject;
	sceneObjects.frog->position = glm::vec3(-1.0f, -0.84f, -2.0f);  
	sceneObjects.frog->size = FROG_SIZE;
	sceneObjects.frog->x = 0.0f;
	sceneObjects.frog->y = 85.0f;			
	sceneObjects.frog->z = 0.0f;

	if (sceneObjects.rainBanner == NULL) sceneObjects.rainBanner = new RainBannerObject;
	sceneObjects.rainBanner->position = glm::vec3(0.0f, 0.0f, 0.0f);  
	sceneObjects.rainBanner->direction = glm::vec3(0.0f, 1.0f, 0.0f);
	sceneObjects.rainBanner->size = 1.0f;
	sceneObjects.rainBanner->speed = 20.4f;
	sceneObjects.rainBanner->startTime = sceneState.time ;
	sceneObjects.rainBanner->elapsedTime = sceneObjects.rainBanner->startTime;

	/**
	* \brief Initialize chunk object
	*/
	if (sceneObjects.chunk == NULL) sceneObjects.chunk = new BaseObject;
	setupWoodChunk(sceneObjects.chunk);

	/**
	* \brief Initialize water
	*/
	if (sceneObjects.waterAnimation == NULL) {
		if (!sceneState.dayLight) {
			sceneObjects.waterAnimation = createWaterAnimation(glm::vec3(4.0f, -1.0f, -2.1f));
		}
		else sceneObjects.waterAnimation = createWaterAnimation(glm::vec3(4.0f, -0.99f, -2.1f));
	}

	/**
	* \brief Initialize water blending
	*/
	if (sceneObjects.waterBlend == NULL) {
		if (!sceneState.dayLight) {
			sceneObjects.waterBlend = createWaterBlend(glm::vec3(4.0f, -0.99f, -2.1f));
		}
		else sceneObjects.waterBlend = createWaterBlend(glm::vec3(4.0f, -1.0f, -2.1f));
	}

	/**
	* \brief Initialize trees
	*/
	setTrees();

	/**
	* \brief Initialize camera
	*/
	if (sceneState.camera == NULL) sceneState.camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	/**
	* \brief Push back objects that can collide with camera to the queue
	*/
	cameraObjects.push_back(sceneObjects.hut);
	cameraObjects.push_back(sceneObjects.boat);
	cameraObjects.push_back(sceneObjects.wood);
	cameraObjects.push_back(sceneObjects.largeWood);
	cameraObjects.push_back(sceneObjects.bench);

	/**
	* \brief Initialize camera collision
	*/
	cameraCollision(sceneState.camera, cameraObjects);

	
	sceneState.cameraFree = false;
	sceneState.dayLight = true;
	sceneState.fog = false;
	sceneState.campfireLight = false;
	sceneState.campfireLit = false;
	sceneState.refelctor = false;
	sceneState.rain = false;
	sceneState.crowSpeed = 1;

	switchCamera();

	/**
	* \brief Reset keymap
	*/
	for (int i = 0; i < KEYS_COUNT; i++) sceneState.keys[i] = false;
	
	
}

/**
* \brief Draw components in the window
*/
void drawWindowContents() {
	/**
	* \brief Projection and view matrices for rain banner
	*/
	glm::mat4 orthoProjMatrix = glm::ortho(-SCENE_WIDTH, SCENE_WIDTH,
		-SCENE_HEIGHT, SCENE_HEIGHT,
		-10.0f * SCENE_DEPTH, 10.0f * SCENE_DEPTH				
	);

	glm::mat4 otrhoViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	
	sceneState.projectionMatrix = glm::perspective(glm::radians(60.0f),
		sceneState.winWidth / (float)sceneState.winHeight,
		0.1f, 20.0f);

	/**
	* \brief Set free camera position
	*/
	if (sceneState.cameraFree) sceneState.viewMatrix = sceneState.camera->GetViewMatrix();

	/**
	* \brief Set crow camera position
	*/
	if (sceneState.camView == 1) {
		sceneState.camera->position = glm::vec3(sceneObjects.crow->position.x,
			sceneObjects.crow->position.y + 0.2,
			sceneObjects.crow->position.z);
		if (!sceneState.cameraFree) {
			sceneState.camera->centerPosition = glm::vec3(-sceneObjects.crow->position.z / 2,
				-0.6, (sceneObjects.crow->position.x - 4) / 2);
			sceneState.camera->up = glm::vec3(4, glm::tan(20) * 2 + 2.0f, 0) - sceneObjects.crow->position;
		}
		sceneState.viewMatrix = sceneState.camera->GetViewMatrix();
	}
	/**
	* \brief Set dynamic curve camera position
	*/
	else if (sceneState.camView == 2) {
		sceneState.camera->currentTime = sceneState.time;

		float t = (sceneState.camera->currentTime - sceneState.camera->startTime) * 0.5f;
		sceneState.camera->position = glm::vec3(3.0f, 2, 0.0f) + evaluateClosedCurve(controlPointsCount, controlPoints, t);

		if (!sceneState.cameraFree) {
			sceneState.camera->centerPosition = evaluateClosedCurve_1stDerivative(controlPointsCount, controlPoints, t);
			sceneState.camera->horizontalPosition = glm::normalize(glm::cross(sceneState.camera->centerPosition, sceneState.camera->worldUp));
			sceneState.camera->up = glm::normalize(glm::cross(sceneState.camera->horizontalPosition, sceneState.camera->centerPosition));
		}
		sceneState.viewMatrix = sceneState.camera->GetViewMatrix();
	}

	/**
	* \brief Set up daylight
	*/
	glUseProgram(shaderProgram.program);
	glUniform1i(shaderProgram.dayLight, sceneState.dayLight);
	glUniform1f(shaderProgram.time, sceneState.time);
	CHECK_GL_ERROR();

	/**
	* \brief Set up reflector
	*/
	glUniform1f(shaderProgram.reflectorSwitch, sceneState.refelctor);
	CHECK_GL_ERROR();
	glUniform3fv(shaderProgram.reflectorPosition, 1, glm::value_ptr(sceneState.camera->position));
	glUniform3fv(shaderProgram.reflectorDirection, 1, glm::value_ptr(sceneState.camera->centerPosition));

	/**
	* \brief Set up sun movement and ambience
	*/
	glm::vec4 sunPosition = sceneState.viewMatrix * glm::vec4(0.0f, cos(sceneState.time * 0.5), sin(sceneState.time * 0.5), 0.0);
	glUniform4fv(shaderProgram.lightPosition, 1, glm::value_ptr(sunPosition));
	glm::vec3 sunAmbience = glm::vec3(0.8f, 0.8f, 0.8f); 
	glUniform3fv(shaderProgram.lightAmbient, 1, glm::value_ptr(sunAmbience));
	glUniform1i(shaderProgram.fogSwitch, sceneState.fog);
	CHECK_GL_ERROR();

	/**
	* \brief Set up campfire light
	*/
	glUniform1i(shaderProgram.campfireLightSwitch, sceneState.campfireLight);
	CHECK_GL_ERROR();
	glUseProgram(0);

	/**
	* \brief Set up flame animation
	*/
	glUseProgram(flameShaderProgram.program);
	glUniform1f(flameShaderProgram.time, sceneState.time);
	glUseProgram(0);
	
	/**
	* \brief Set up boat animation
	*/
	glUseProgram(boatShaderProgram.program);
	glUniform1f(boatShaderProgram.time, sceneState.time);
	CHECK_GL_ERROR();
	glUniform1i(boatShaderProgram.fogSwitch, sceneState.fog);
	glUseProgram(0);

	/**
	* \brief Set up fog
	*/
	glUseProgram(frogShaderProgram.program);
	glUniform1i(frogShaderProgram.fogSwitch, sceneState.fog);
	glUseProgram(0);

	/**
	* \brief Set up crow 
	*/
	glUseProgram(crowShaderProgram.program);
	glUniform1f(crowShaderProgram.alpha, crowAlpha);
	CHECK_GL_ERROR();
	glUniform1i(crowShaderProgram.fogSwitch, sceneState.fog);
	glUseProgram(0);

	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	CHECK_GL_ERROR();

	/**
	* \brief Draw objects
	*/

	glStencilFunc(GL_ALWAYS, 1, -1);
	drawTerrain(sceneObjects.terrain, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 2, -1);
	drawHut(sceneObjects.hut, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 3, -1);
	drawBench(sceneObjects.bench, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 4, -1);
	drawCampfire(sceneObjects.campfire, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 5, -1);
	drawBoat(sceneObjects.boat, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 6, -1);
	drawWood(sceneObjects.wood, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 7, -1);
	drawLargeWood(sceneObjects.largeWood, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 8, -1);
	drawFrog(sceneObjects.frog, sceneState.viewMatrix, sceneState.projectionMatrix);

	glStencilFunc(GL_ALWAYS, 9, -1);
	if (sceneState.chunkPicked) glDisable(GL_DEPTH_TEST);
	drawWoodChunk(sceneObjects.chunk, sceneState.viewMatrix, sceneState.projectionMatrix);
	if (sceneState.chunkPicked) glEnable(GL_DEPTH_TEST);

	/**
	* \brief Draw skybox
	*/
	glDisable(GL_STENCIL_TEST);
	glUseProgram(skyboxShaderProgram.program);
	glUniform1i(skyboxShaderProgram.daylight, sceneState.dayLight);
	drawSkybox(sceneState.viewMatrix, sceneState.projectionMatrix, sceneState.fog);
	CHECK_GL_ERROR();
	
	/**
	* \brief Draw trees
	*/
	for (Trees::iterator i = sceneObjects.trees.begin();
		i != sceneObjects.trees.end();
		++i) {
		BaseObject* tree = (BaseObject*)(*i);
		drawTree(tree, sceneState.viewMatrix, sceneState.projectionMatrix);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/**
	* \brief Draw crow
	*/
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 10, -1);
	drawCrow(sceneObjects.crow, sceneState.viewMatrix, sceneState.projectionMatrix, sceneState.fog, sceneState.time);
	glDisable(GL_STENCIL_TEST);

	/**
	* \brief Draw flame
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	if (sceneState.campfireLit) drawFlame(sceneObjects.flame, sceneState.viewMatrix, sceneState.projectionMatrix);
	glDisable(GL_BLEND);

	/**
	* \brief Draw rain banner
	*/
	if (sceneState.rain) drawBanner(sceneObjects.rainBanner, otrhoViewMatrix, orthoProjMatrix);

	/**
	* \brief Draw water
	*/
	drawWaterAnimation(sceneObjects.waterAnimation, sceneState.viewMatrix, sceneState.projectionMatrix);
	drawWaterBlend(sceneObjects.waterBlend, sceneState.viewMatrix, sceneState.projectionMatrix);
}

/**
	* \brief Update display
	*/
void display() {
	GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	mask |= GL_STENCIL_BUFFER_BIT;

	CHECK_GL_ERROR();
	glClear(mask);
	drawWindowContents();

	glutSwapBuffers();
}

/**
	* \brief Reshape window
*/
void reshape(int w, int h) {
	sceneState.winWidth = w;
	sceneState.winHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/**
	* \brief Update current scene state
*/
void update(int) {
	float time = sceneState.time;
	sceneState.time = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	float delta = sceneState.time - time;

	if (sceneState.cameraFree) {
		if (sceneState.keys[KEY_UP_ARROW]) sceneState.camera->cameraKeyboardInputManager(FORWARD, delta);
		if (sceneState.keys[KEY_LEFT_ARROW]) sceneState.camera->cameraKeyboardInputManager(LEFT, delta);
		if (sceneState.keys[KEY_DOWN_ARROW]) sceneState.camera->cameraKeyboardInputManager(BACKWARD, delta);
		if (sceneState.keys[KEY_RIGHT_ARROW]) sceneState.camera->cameraKeyboardInputManager(RIGHT, delta);
	}

	sceneObjects.crow->position.x = 4 + 2 * cos(glm::radians(30.0f) * sceneState.time * sceneState.crowSpeed);
	sceneObjects.crow->position.y = crowHeight * glm::sin(0.14 * (sceneState.time / 10) * sceneState.crowSpeed) + 1.5;				//possibly change
	sceneObjects.crow->position.z = 2 * sin(glm::radians(30.0f) * sceneState.time * sceneState.crowSpeed);
	sceneObjects.crow->y = -30.0f * sceneState.time * sceneState.crowSpeed;

	sceneObjects.rainBanner->elapsedTime = sceneState.time;
	sceneObjects.waterAnimation->elapsedTime = sceneState.time;

	if (!sceneState.dayLight) {
		sceneObjects.waterAnimation->position = glm::vec3(-7.0f, -0.99f, -2.1f);
	}
	else sceneObjects.waterAnimation->position = glm::vec3(-7.0f, -1.0f, -2.1f);

	if (sceneState.dayLight) {
		sceneObjects.waterBlend->position = glm::vec3(-7.0f, -0.99f, -2.1f);
	}
	else sceneObjects.waterBlend->position = glm::vec3(-7.0f, -1.0f, -2.1f);

	if (!sceneState.cameraFree && sceneState.camView != 2) switchCamera();

	glutTimerFunc(33, update, 0);
	glutPostRedisplay();
}

/**
	* \brief Chunk drag handling
*/
void dragChunk(int mouseX, int mouseY) {
	if (sceneState.chunkPicked) {
		unsigned char depth = 0;
		glReadPixels(mouseX, WIN_HEIGHT - 1 - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &depth);
		glm::mat4 inverse = glm::inverse(sceneState.projectionMatrix * sceneState.viewMatrix);
		float in[4];
		float z = depth / 255.0f;

		in[0] = (2.0f * (((float)mouseX) / sceneState.winWidth)) - 1.0f;
		in[1] = (2.0f * (((float)mouseY) / sceneState.winHeight)) - 1.0f;
		in[2] = 2.0f * z - 1.0f;
		in[3] = 1.0f;

		glm::vec4 position = inverse * glm::vec4(in[0], in[1], in[2], in[3]);
		position.w = 1.0f / position.w;
		position.x *= position.w;
		position.y *= position.w;
		position.z *= position.w;

		glm::vec3 newPosition = glm::vec3(position.x, position.y + 0.05f, position.z);
		sceneObjects.chunk->position = newPosition;
	}
}


/**
	* \brief Mouse input handling
*/
void mouseInputManager(int buttonState, int buttonPressed, int mouseX, int mouseY) {
	if (buttonPressed == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN) {
		unsigned char item = 0;
		glReadPixels(mouseX, WIN_HEIGHT - 1 - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &item);

		switch (item) {
		case 0:
			cout << "CLICKED: Background" << endl;
			break;
		case 4:
			cout << "CLICKED: Campfire" << endl;
			sceneState.campfireLit = !sceneState.campfireLit;
			sceneState.campfireLight = !sceneState.campfireLight;
			break;
		case 5:
			cout << "CLICKED: Boat" << endl;
			break;
		case 8:
			cout << "CLICKED: Frog" << endl;
			newFrogColor();
			break;
		case 9:
			cout << "CLICKED: Chunk" << endl;
			sceneState.chunkPicked = !sceneState.chunkPicked;
			break;
		case 10:
			cout << "CLICKED: Crow" << endl;
			sceneState.crowSpeed = sceneState.crowSpeed < 5.0f ? sceneState.crowSpeed + 0.5f : 1.0f;
		default:
			cout << "Clicked on object with index of " << (int)item << endl;
		}
	}
}

/**
	* \brief Move camera according to the mouse moving, cursor stays in the center of the window
*/
void passiveMouseMotion(int mouseX, int mouseY) {
	if (mouseY != sceneState.winHeight / 2 && mouseX != sceneState.winWidth / 2) {
		sceneState.camera->CameraMouseInputManager(0.5f * (mouseX - sceneState.winWidth / 2), -0.5f
			* (mouseY - sceneState.winHeight / 2));
		glutWarpPointer(sceneState.winWidth / 2, sceneState.winHeight / 2);
		glutPostRedisplay();
	}
	dragChunk(mouseX, mouseY);
}

/**
	* \brief Keyboar input handling, when the key on the keyboard is pressed
*/
void keyboardManager(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 27:   //ESC - close window
		glutLeaveMainLoop();
		break;
	case 'w': //W- Move forward
		if (sceneState.cameraFree) sceneState.keys[KEY_UP_ARROW] = true;
		break;
	case 'a': //A - Move left
		if (sceneState.cameraFree) sceneState.keys[KEY_LEFT_ARROW] = true;
		break;
	case 's': //S - Move back
		if (sceneState.cameraFree) sceneState.keys[KEY_DOWN_ARROW] = true;
		break;
	case 'd': //D - Move right
		if (sceneState.cameraFree) sceneState.keys[KEY_RIGHT_ARROW] = true;
		break;
	case 'r': //R - Restart scene
		restart();
		break;
	case 'n': //N - Switch to night mode
		sceneState.dayLight = !sceneState.dayLight;
		break;
	case 'c': //C - Change camera view
		sceneState.camView++;
		if (sceneState.camView == 4) sceneState.camView = 0;
		cout << sceneState.camView << endl;
		//sceneState.camView %= CAMERA_POSITIONS;
		break;
	case 't': //T - reflector on/off
		sceneState.refelctor = !sceneState.refelctor;
		break;
	case 'x': //X - Free camera controll on/off
		sceneState.cameraFree = !sceneState.cameraFree;
		if (sceneState.cameraFree) {
			glutMotionFunc(passiveMouseMotion);
			glutPassiveMotionFunc(passiveMouseMotion);
			glutWarpPointer(sceneState.winWidth / 2, sceneState.winHeight / 2);
		}
		else {
			glutMotionFunc(NULL);
			glutPassiveMotionFunc(NULL);
		}
		break;
	case 'e': //E - Rain on/off
		sceneState.rain = !sceneState.rain;
		break;
	case 'f': //F - Fog on/off
		sceneState.fog = !sceneState.fog;
		break;
	default:
		cout << "ERROR: Unidentified key!" << endl;
		break;
	}
}

/**
	* \brief Keyboard release manager, when the key on the keyboard is released
*/
void keyboardReleaseManager(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 'w': //W - Move forward
		if (sceneState.cameraFree) sceneState.keys[KEY_UP_ARROW] = false;
		break;
	case 'a': //A - Move left
		if (sceneState.cameraFree) sceneState.keys[KEY_LEFT_ARROW] = false;
		break;
	case 's': //S - Move back
		if (sceneState.cameraFree) sceneState.keys[KEY_DOWN_ARROW] = false;
		break;
	case 'd': //D - Move right
		if (sceneState.cameraFree) sceneState.keys[KEY_RIGHT_ARROW] = false;
		break;
	default:
		break;
	}
}

/**
	* \brief Handles arrows keys input on the keyboard, when key is pressed
*/
void specialKeyboardManager(int key, int mouseX, int mouseY) {
	switch (key) {
	case GLUT_KEY_UP: //W - Move forward
		if (sceneState.cameraFree) sceneState.keys[KEY_UP_ARROW] = true;
		break;
	case GLUT_KEY_LEFT: //A - Move left
		if (sceneState.cameraFree) sceneState.keys[KEY_LEFT_ARROW] = true;
		break;
	case GLUT_KEY_DOWN: //S - Move back
		if (sceneState.cameraFree) sceneState.keys[KEY_DOWN_ARROW] = true;
		break;
	case GLUT_KEY_RIGHT: //D - Move right
		if (sceneState.cameraFree) sceneState.keys[KEY_RIGHT_ARROW] = true;
		break;
	default:
		break;
	}
}

/**
	* \brief Handles arrows keys input on the keyboard, when key is released
*/
void specialKeyboardReleaseManager(int key, int mouseX, int mouseY) {
	switch (key) {
	case GLUT_KEY_UP: //W - Move forward
		if (sceneState.cameraFree) sceneState.keys[KEY_UP_ARROW] = false;
		break;
	case GLUT_KEY_LEFT: //A - Move left
		if (sceneState.cameraFree) sceneState.keys[KEY_LEFT_ARROW] = false;
		break;
	case GLUT_KEY_DOWN: //S - Move back
		if (sceneState.cameraFree) sceneState.keys[KEY_DOWN_ARROW] = false;
		break;
	case GLUT_KEY_RIGHT: //D - Move right
		if (sceneState.cameraFree) sceneState.keys[KEY_RIGHT_ARROW] = false;
		break;
	default:
		break;
	}
}

/**
	* \brief Initialize application. Called once
*/
void initApp() {
	srand((unsigned int)time(NULL));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	useLight = true;
	sceneState.camView = -1;

	initShaderPrograms();
	initModels();
	restart();
}

/**
	* \brief Clean ups the scene in the end of the scene
*/
void endApp() {
	deleteObjects();
	deleteModels();
	deleteShaderPrograms();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowSize(win_width, win_height);
	glutCreateWindow(WIN_TITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardManager);
	glutKeyboardUpFunc(keyboardReleaseManager);
	glutSpecialFunc(specialKeyboardManager);
	glutSpecialUpFunc(specialKeyboardReleaseManager);
	glutMouseFunc(mouseInputManager);
	glutTimerFunc(33, update, 0);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");
	
	initApp();
	glutCloseFunc(endApp);
	glutMainLoop();

	return 0;
}
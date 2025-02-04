/**
 * \file	render_stuff.h
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#ifndef RENDER_STUFF_H
#define RENDER_STUFF_H

#include "data.h"
#include "camera.h"

/**
* \brief Define geometry of the objects in the scene structure
*/

typedef struct {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	unsigned int numTriangles;

	glm::vec3 vertexMin;
	glm::vec3 vertexMax;

	GLuint texture;
	GLuint specularMap;
	GLuint normalMap;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiness;

} MeshGeometry;

/**
* \brief Scene light parameters structure
*/
typedef struct {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float cosCutOff;
	float exponent;

} Light;

/**
* \brief Object parameters structure for dynamic objects
*/
typedef struct Object{
	glm::vec3 position;
	glm::vec3 direction;

	float startTime;
	float elapsedTime;
	float speed;
	float size;

} Object;

/**
* \brief Object parameters structure for static objects
*/
typedef struct {
	glm::vec3 position;
	glm::vec3 hitboxMax;
	glm::vec3 hitboxMin;

	float size;
	float x, y, z; //Angle
	glm::vec3 color; //for frog interaction
} BaseObject;

/**
* \brief Rain banner object deffinition
*/
typedef struct RainBannerObject : public Object {

} RainBannerObject;

/**
* \brief Water object deffinition
*/
typedef struct WaterObject : public Object {
	int testureFrames;
	float frameDuration;
} WaterObject;


/**
* \brief Shader program structure
*/
typedef struct _commonShader{
	GLuint program;

	GLint position;
	GLint normal; 
	GLint texture;

	GLint pvmMatrix;
	GLint viewMatrix;
	GLint modelMatrix;
	GLint normalMatrix;
	GLint projectionMatrix;

	GLint diffuse;
	GLint ambient;
	GLint specular;
	GLint shiness;

	GLint useTexture;
	GLint textureSample;
	GLint useNormalMap;
	GLint normalMap;

	GLint lightPosition;
	GLint lightAmbient;
	GLint reflectorPosition;
	GLint reflectorDirection;
	GLint reflectorSwitch;

	GLint time;

	//Unique parameters for different shader programs
	GLint dayLight;

	GLint fogSwitch;

	GLint campfireLightSwitch;

	GLint alpha;
} Shader;

/**
* \brief Skybox shader program
*/
typedef struct _skyboxShader{
	GLuint program;

	GLint screenCoord;
	GLint skyboxSample;
	GLint daylight;
	GLint inversePVM;
	GLint skyboxFogSwitch;

} SkyboxShaderProgram;

/**
* \brief Water animation shader program
*/
typedef struct _waterShader{
	GLuint program;

	GLint position;
	GLint textureCoord;
	GLint pvmMatrix;
	GLint vMatrix;
	GLint textureSample;
	GLint frameDuration;

	GLint time;

} WaterShaderProgram;

/**
* \brief Water blend animation shader program
*/
typedef struct _waterBlendShader{
	GLuint program;

	GLint position;
	GLint textureCoord;
	GLint pvmMatrix;
	GLint vMatrix;
	GLint textrueSample;
	GLint frameDuration;

	GLint time;

} WaterBlendShaderProgram;

/**
* \brief Rainbanner animation shader program
*/
typedef struct _bannerShader{
	GLuint program;

	GLint position;
	GLint textureCoord;
	GLint textureSample;
	GLint PvmMatrix;
	GLint time;

} RainShaderProgram;

/**
* \brief Definition of functions to draw objects
*/
void cameraCollision(Camera *cam, std::vector<BaseObject*> objects);
void drawTerrain(BaseObject* terrain, glm::mat4& vierwMatrix, glm::mat4 &projectionMatrix);
void drawSkybox(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool fog);
void drawHut(BaseObject* hut, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawTree(BaseObject* tree, glm::mat4& viewMatrix, glm::mat4 &projectionMatrix);
void drawCampfire(BaseObject* campfire, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawFlame(BaseObject* flame, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawBoat(BaseObject* boat, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void setupWoodChunk(BaseObject *wood);
void drawWood(BaseObject* wood, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawLargeWood(BaseObject* lagreWood, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawWoodChunk(BaseObject* chunk, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawCrow(BaseObject* crow, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool fog, float time);
void drawBanner(RainBannerObject* rainBanner, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawFrog(BaseObject* frog, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawBench(BaseObject* bench, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawWaterAnimation(WaterObject* water, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
void drawWaterBlend(WaterObject* water, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);

/**
* \brief Shader programs functions definition
*/
void initShaderPrograms();
void deleteShaderPrograms();

/**
* \brief Models functions definition
*/
void initModels();
void deleteModels();
#endif 

/**
 * \file	render_stuff.cpp
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#include "pgr.h"
#include "render_stuff.h"
#include "../assets/wood/chunk.h"
#include <iostream>

/**
* \brief Flags for assimp loader - Triangulate polygons | Transforems scene hierechy | Calculate normals per vertex
*/
#define ASSIMP_IMPORTER_FLAGS (aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices)

/**
* \brief Mesh geometry definitions
*/
MeshGeometry* terrainGeometry = NULL;
MeshGeometry* skyboxGeometry = NULL;
MeshGeometry* waterAnimationGeometry = NULL;
MeshGeometry* waterBlendGeometry = NULL;
MeshGeometry* frogGeometry = NULL;
MeshGeometry* crowGeometry = NULL;
MeshGeometry* benchGeometry = NULL;
MeshGeometry* boatGeometry = NULL;
MeshGeometry* flameGeometry = NULL;
MeshGeometry* rainBannerGeometry = NULL;
std::vector<MeshGeometry*> campfireGeometry;
std::vector<MeshGeometry*> hutGeometry;
std::vector<MeshGeometry*> treeGeometry;
std::vector<MeshGeometry*> woodGeometry;
std::vector<MeshGeometry*> largeWoodGeometry;

/**
* \brief Objects models and textures pathes
*/
const char* SKYBOX_MAP[] = {
		"assets/skybox/posx.jpg",
		"assets/skybox/negx.jpg",
		"assets/skybox/posy.jpg",
		"assets/skybox/negy.jpg", 
		"assets/skybox/posz.jpg",
		"assets/skybox/negz.jpg"
};

const char* TERRAIN_MODEL = "assets/terrain/terrain.obj";
const char* HUT_MODEL = "assets/hut/hut.obj";
const char* BOAT_MODEL = "assets/boat/Cannoe.obj";
const char* CROW_MODEL = "assets/crow/AmericanCrow.obj";
const char* FROG_MODEL = "assets/frog/frog.obj";
const char* TREE_MODEL = "assets/tree/tree.obj";
const char* BENCH_MODEL = "assets/bench/bench2.obj";
const char* CAMPFIRE_MODEL = "assets/campfire/campfire.obj";
const char* LARGE_WOOD_MODEL = "assets/wood/large_wood.obj";
const char* WOOD_MODEL = "assets/wood/wood.obj";

const char* FLAME_TEXTURE = "assets/campfire/flame.jpg";
const char* BANNER_TEXTURE = "assets/rain/rain.png";
const char* WOOD_CHUNK_TEXTURE = "assets/wood/wood.jpg";
const char* WATER_ANIMATION_TEXTURE = "assets/water/water.jpg";

/**
* \brief Shader programs definitions
*/
Shader shaderProgram;
SkyboxShaderProgram skyboxShaderProgram;
Shader crowShaderProgram;
Shader flameShaderProgram;
Shader boatShaderProgram;
Shader frogShaderProgram;
RainShaderProgram rainShaderProgram;
WaterShaderProgram waterShaderProgram;
WaterBlendShaderProgram waterBlendShaderProgram;

bool useLight = true;

/**
* \brief Movable object's structure 
*/
struct chunk {
	GLint position;
	GLint PVM;
	GLuint textureCoord;
	GLuint textBuffer;
	GLuint normal;
	GLuint normalCoord;

	glm::vec3 pos;
	glm::vec3 rot;
	float size;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	
	GLuint texture;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiness;
} chunk;


/**
* \brief Function to create a collision between camera and objects
*/
void cameraCollision(Camera* camera, std::vector<BaseObject*> objects) {

	for (const auto& obj : hutGeometry) {
		Box* box = (Box*)malloc(sizeof(Box));
		box->maxCoord = obj->vertexMax;
		box->minCoord = obj->vertexMin;
		box->position = objects[0]->position;
		box->size = objects[0]->size;
		box->colliderShape = true;
		camera->objects.push_back(box);
	}

	Box* boatBox = (Box*)malloc(sizeof(Box));
	boatBox->maxCoord = boatGeometry->vertexMax;
	boatBox->minCoord = boatGeometry->vertexMin;
	boatBox->position = objects[4]->position;
	boatBox->size = objects[4]->size;
	boatBox->colliderShape = true;
	camera->objects.push_back(boatBox);

	for (const auto& obj : woodGeometry) {
		Box* box = (Box*)malloc(sizeof(Box));
		box->maxCoord = obj->vertexMax;
		box->minCoord = obj->vertexMin;
		box->position = objects[2]->position;
		box->size = objects[2]->size;
		box->colliderShape = true;
		camera->objects.push_back(box);
	}

	for (const auto& obj : largeWoodGeometry) {
		Box* box = (Box*)malloc(sizeof(Box));
		box->maxCoord = obj->vertexMax;
		box->minCoord = obj->vertexMin;
		box->position = objects[3]->position;
		box->size = objects[3]->size;
		box->colliderShape = true;
		camera->objects.push_back(box);
	}

	Box* benchBox = (Box*)malloc(sizeof(Box));
	benchBox->maxCoord = benchGeometry->vertexMax;
	benchBox->minCoord = benchGeometry->vertexMin;
	benchBox->position = objects[4]->position;
	benchBox->size = objects[4]->size;
	benchBox->colliderShape = true;
	camera->objects.push_back(benchBox);
}


/**
* \brief Setup matrix uniforms for shaders
*/
void setTransformUniforms(glm::mat4& model, glm::mat4& viewMatrix, glm::mat4& projectMatrix, Shader& shader) {
	glm::mat4 PVM = projectMatrix * viewMatrix * model;

	glUniformMatrix4fv(shader.pvmMatrix, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniformMatrix4fv(shader.viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(shader.modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

	const glm::mat4 rotationMatrix = glm::mat4(model[0], model[1], model[2], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	glm::mat4 normal = glm::transpose(glm::inverse(rotationMatrix));
	
	glUniformMatrix4fv(shader.normal, 1, GL_FALSE, glm::value_ptr(normal));
}


/**
* \brief Setup material/texture uniforms for shaders
*/
void setMaterialUniforms(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shiness, GLuint texture, GLuint normalMap, Shader& shader) {
	glUniform3fv(shader.diffuse, 1, glm::value_ptr(diffuse));
	glUniform3fv(shader.ambient, 1, glm::value_ptr(ambient));
	glUniform3fv(shader.specular, 1, glm::value_ptr(specular));
	glUniform1f(shader.shiness, shiness);

	if (texture != -1) {
		glUniform1i(shader.useTexture, true);
		CHECK_GL_ERROR();
		glUniform1i(shader.textureSample, 0);
		CHECK_GL_ERROR();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		CHECK_GL_ERROR();
	}
	else glUniform1i(shader.useTexture, 0);
}


/**
* \brief Create object model matrix
*/
glm::mat4 getModelMatrix(BaseObject* object) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), object->position);
	model = glm::rotate(model, glm::radians(object->x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(object->y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(object->z), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(object->size, object->size, object->size));

	return model;
}

//Draw terrain
void drawTerrain(BaseObject* terrain, glm::mat4& vierwMatrix, glm::mat4 &projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(terrain);
	setTransformUniforms(model, vierwMatrix, projectionMatrix, shaderProgram);
	setMaterialUniforms(terrainGeometry->ambient, 
		terrainGeometry->diffuse, 
		terrainGeometry->specular, 
		terrainGeometry->shiness, 
		terrainGeometry->texture, 0, shaderProgram);
	glBindVertexArray(terrainGeometry->vao);
	glDrawElements(GL_TRIANGLES, terrainGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw skybox
void drawSkybox(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool fog) {
	glUseProgram(skyboxShaderProgram.program);

	glm::mat4 model = projectionMatrix * viewMatrix;
	glm::mat4 rotation = viewMatrix;
	rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 inversePVM = glm::inverse(projectionMatrix * rotation);
	glUniformMatrix4fv(skyboxShaderProgram.inversePVM, 1, GL_FALSE, glm::value_ptr(inversePVM));
	glUniform1i(skyboxShaderProgram.skyboxSample, 0);
	glUniform1i(skyboxShaderProgram.skyboxFogSwitch, fog);
	
	glBindVertexArray(skyboxGeometry->vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, skyboxGeometry->numTriangles + 2);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);
}

//Draw hut
void drawHut(BaseObject* hut, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(hut);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	for (size_t i = 0; i < hutGeometry.size(); i++) {
		setMaterialUniforms(hutGeometry[i]->ambient, 
			hutGeometry[i]->diffuse, 
			hutGeometry[i]->specular, 
			hutGeometry[i]->shiness, 
			hutGeometry[i]->texture, 
			hutGeometry[i]->normalMap, shaderProgram);
		glBindVertexArray(hutGeometry[i]->vao);
		glDrawElements(GL_TRIANGLES, hutGeometry[i]->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw tree
void drawTree(BaseObject* tree, glm::mat4& viewMatrix, glm::mat4 &projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(tree);

	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	for (size_t i = 0; i < treeGeometry.size(); i++) {
		glBindVertexArray(treeGeometry[i]->vao);

		setMaterialUniforms(treeGeometry[i]->ambient, 
			treeGeometry[i]->diffuse, 
			treeGeometry[i]->specular,
			treeGeometry[i]->shiness,
			treeGeometry[i]->texture,
			treeGeometry[i]->normalMap, shaderProgram); 
		
		glDrawElements(GL_TRIANGLES, treeGeometry[i]->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;

}


//Draw campfire
void drawCampfire(BaseObject* campfire, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(campfire);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	for (size_t i = 0; i < campfireGeometry.size(); i++) {
		setMaterialUniforms(campfireGeometry[i]->ambient,
			campfireGeometry[i]->diffuse,
			campfireGeometry[i]->specular,
			campfireGeometry[i]->shiness,
			campfireGeometry[i]->texture,
			campfireGeometry[i]->normalMap, shaderProgram);
		glBindVertexArray(campfireGeometry[i]->vao);
		glDrawElements(GL_TRIANGLES, campfireGeometry[i]->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw flame
void drawFlame(BaseObject* flame, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(flameShaderProgram.program);

	glm::mat4 model = glm::mat4(viewMatrix[0], viewMatrix[1], viewMatrix[2], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	model = glm::transpose(model);
	model = glm::scale(glm::translate(glm::mat4(1.0f), flame->position) * model, glm::vec3(flame->size));

	glm::mat4 PVM = projectionMatrix * viewMatrix * model;

	glUniformMatrix4fv(flameShaderProgram.pvmMatrix, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniformMatrix4fv(flameShaderProgram.viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(flameShaderProgram.modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(flameShaderProgram.useTexture, true);
	//GPU linker
	glUniform1i(flameShaderProgram.textureSample, 0);
	//OpenGL BindTexture
	glActiveTexture(GL_TEXTURE0 + 0);

	glBindTexture(GL_TEXTURE_2D, flameGeometry->texture);
	CHECK_GL_ERROR();
	glBindVertexArray(flameGeometry->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, flameGeometry->numTriangles);
	
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw boat
void drawBoat(BaseObject* boat, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {

	glUseProgram(boatShaderProgram.program);

	glm::mat4 model = getModelMatrix(boat);
	setTransformUniforms(model, viewMatrix, projectionMatrix, boatShaderProgram);

	setMaterialUniforms(boatGeometry->ambient,
		boatGeometry->diffuse,
		boatGeometry->specular,
		boatGeometry->shiness,
		boatGeometry->texture,
		0, boatShaderProgram);

	glBindVertexArray(boatGeometry->vao);
	glDrawElements(GL_TRIANGLES, boatGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw wood
void drawWood(BaseObject* wood, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(wood);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	for (size_t i = 0; i < woodGeometry.size(); i++) {
		setMaterialUniforms(woodGeometry[i]->ambient,
			woodGeometry[i]->diffuse,
			woodGeometry[i]->specular,
			woodGeometry[i]->shiness,
			woodGeometry[i]->texture,
			woodGeometry[i]->normalMap, shaderProgram);
		glBindVertexArray(woodGeometry[i]->vao);
		glDrawElements(GL_TRIANGLES, woodGeometry[i]->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw wood with an axe
void drawLargeWood(BaseObject* lagreWood, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(lagreWood);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	for (size_t i = 0; i < largeWoodGeometry.size(); i++) {
		setMaterialUniforms(largeWoodGeometry[i]->ambient,
			largeWoodGeometry[i]->diffuse,
			largeWoodGeometry[i]->specular,
			largeWoodGeometry[i]->shiness,
			largeWoodGeometry[i]->texture,
			largeWoodGeometry[i]->normalMap, shaderProgram);
		glBindVertexArray(largeWoodGeometry[i]->vao);
		glDrawElements(GL_TRIANGLES, largeWoodGeometry[i]->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}

	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw wooden chunk - movable object
void drawWoodChunk(BaseObject* ch, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(ch);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	setMaterialUniforms(chunk.ambient, 
		chunk.diffuse, 
		chunk.specular,
		chunk.shiness, 
		chunk.texture, 
		0, shaderProgram);

	glBindVertexArray(chunk.vao);
	glDrawElements(GL_TRIANGLES, wood_n_triangles * 3, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw frog
void drawFrog(BaseObject* frog, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(frogShaderProgram.program);
	glm::mat4 model = getModelMatrix(frog);
	setTransformUniforms(model, viewMatrix, projectionMatrix, frogShaderProgram);
	setMaterialUniforms(frogGeometry->ambient, 
		frogGeometry->diffuse, 
		frogGeometry->specular, 
		frogGeometry->shiness, 
		frogGeometry->texture, 
		0, frogShaderProgram);
	glBindVertexArray(frogGeometry->vao);
	glDrawElements(GL_TRIANGLES, frogGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw bench
void drawBench(BaseObject* bench, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram.program);

	glm::mat4 model = getModelMatrix(bench);
	setTransformUniforms(model, viewMatrix, projectionMatrix, shaderProgram);

	setMaterialUniforms(benchGeometry->ambient, 
		benchGeometry->diffuse, 
		benchGeometry->specular, 
		benchGeometry->shiness, 
		benchGeometry->texture, 
		benchGeometry->normalMap, shaderProgram);

	glBindVertexArray(benchGeometry->vao);
	glDrawElements(GL_TRIANGLES, benchGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	return;

}

//Draw crow
void drawCrow(BaseObject* crow, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool fog, float time) {
	glUseProgram(crowShaderProgram.program);

	glm::mat4 model = getModelMatrix(crow);
	setTransformUniforms(model, viewMatrix, projectionMatrix, crowShaderProgram);

	glUniform1f(crowShaderProgram.time, time);
	CHECK_GL_ERROR();
	glUniform1i(crowShaderProgram.fogSwitch, fog);
	CHECK_GL_ERROR();

	glBindVertexArray(crowGeometry->vao);
	glDrawElements(GL_TRIANGLES, crowGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}


//Draw Rain banner
void drawBanner(RainBannerObject* rainBanner, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(rainShaderProgram.program);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), rainBanner->position);
	model = glm::scale(model, glm::vec3(rainBanner->size));

	glm::mat4 PVM = projectionMatrix * viewMatrix * model;
	glUniformMatrix4fv(rainShaderProgram.PvmMatrix, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniform1f(rainShaderProgram.time, (rainBanner->elapsedTime - rainBanner->startTime)* rainBanner->speed);
	glUniform1i(rainShaderProgram.textureSample, 0);

	glBindTexture(GL_TEXTURE_2D, rainBannerGeometry->texture);
	glBindVertexArray(rainBannerGeometry->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, rainBannerGeometry->numTriangles);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	return;
}


//Draw water animation
void drawWaterAnimation(WaterObject* water, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glUseProgram(waterShaderProgram.program);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), water->position);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 scalingVector(15.0f * WATER_SIZE, -80.0f * WATER_SIZE,  WATER_SIZE);
	model = glm::scale(model, scalingVector);
	//model = glm::scale(model, glm::vec3(water->size * 0.7f));

	glm::mat4 PVM = projectionMatrix * viewMatrix * model;
	glUniformMatrix4fv(waterShaderProgram.pvmMatrix, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniformMatrix4fv(waterShaderProgram.vMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform1f(waterShaderProgram.time, water->elapsedTime - water->startTime);
	glUniform1i(waterShaderProgram.textureSample, 0);
	glUniform1f(waterShaderProgram.frameDuration, water->frameDuration);

	glBindVertexArray(waterAnimationGeometry->vao);
	glBindTexture(GL_TEXTURE_2D, waterAnimationGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, waterAnimationGeometry->numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

//Draw water blend animation
void drawWaterBlend(WaterObject* water, glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(waterBlendShaderProgram.program);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), water->position);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 scalingVector(15.0f * WATER_SIZE, -80.0f * WATER_SIZE, WATER_SIZE);
	model = glm::scale(model, scalingVector);

	glm::mat4 PVM = projectionMatrix * viewMatrix * model;
	glUniformMatrix4fv(waterBlendShaderProgram.pvmMatrix, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniformMatrix4fv(waterBlendShaderProgram.vMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glBindVertexArray(waterBlendGeometry->vao);
	glBindTexture(GL_TEXTURE_2D, waterBlendGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, waterBlendGeometry->numTriangles);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}

/**
* \brief Setup wooden chunk - movable object
*/
void setupWoodChunk(BaseObject* wood) {
	wood->size = chunk.size;
	wood->position = chunk.pos;
	wood->x = chunk.rot[0];
	wood->y = chunk.rot[1];
	wood->z = chunk.rot[2];
}

/**
* \brief Setup skybox geometry
*/
void initSkyboxGeometry(MeshGeometry** geometry) {
	*geometry = new MeshGeometry;
	static const float screenCoords[] = {
		1.0f, 1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};

	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);

	glGenBuffers(1, &((*geometry)->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(skyboxShaderProgram.screenCoord);
	glVertexAttribPointer(skyboxShaderProgram.screenCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	(*geometry)->numTriangles = 2;
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &((*geometry)->texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, (*geometry)->texture);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		std::string texture = SKYBOX_MAP[i];
		std::cout << "LOADING: Cube map texture: " << texture << std::endl;
		if (!pgr::loadTexImage2D(texture, targets[i])) {
			pgr::dieWithError("ERROR: Couldn't download skybox texture: " + texture);
		}
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();
}
/**
* \brief Setup rain banner geometry
*/
void initBannerGeometry(MeshGeometry** geometry) {
	*geometry = new MeshGeometry;

	glBindTexture(GL_TEXTURE_2D, (*geometry)->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);

	glGenBuffers(1, &((*geometry)->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bannerVertexData), bannerVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(rainShaderProgram.position);
	glEnableVertexAttribArray(rainShaderProgram.textureCoord);

	glVertexAttribPointer(rainShaderProgram.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(rainShaderProgram.textureCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	(*geometry)->texture = pgr::createTexture(BANNER_TEXTURE);
	(*geometry)->numTriangles = bannerNumVerticies;
}

/**
* \brief Setup water geometry
*/
void initWaterGeometry(MeshGeometry** geometry) {
	*geometry = new MeshGeometry;
	(*geometry)->texture = pgr::createTexture(WATER_ANIMATION_TEXTURE, true);

	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);

	glGenBuffers(1, &((*geometry)->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertexData), waterVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(waterShaderProgram.position);
	glVertexAttribPointer(waterShaderProgram.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(waterShaderProgram.textureCoord);
	glVertexAttribPointer(waterShaderProgram.textureCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	
	(*geometry)->numTriangles = waterNumVerticies;
}

/**
* \brief Setup water blend geometry
*/
void initWaterBlendGeometry(MeshGeometry** geometry) {
	*geometry = new MeshGeometry;
	(*geometry)->texture = pgr::createTexture(WATER_ANIMATION_TEXTURE, true);

	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);

	glGenBuffers(1, &((*geometry)->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertexData), waterVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(waterBlendShaderProgram.position);
	glVertexAttribPointer(waterBlendShaderProgram.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(waterBlendShaderProgram.textureCoord);
	glVertexAttribPointer(waterBlendShaderProgram.textureCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	
	(*geometry)->numTriangles = waterNumVerticies;
}

/**
* \brief Setup flame geometry
*/
void initFlameGeometry(MeshGeometry** geometry) {
	*geometry = new MeshGeometry;
	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);

	glGenBuffers(1, &((*geometry)->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flameVertexData), flameVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(flameShaderProgram.position);

	glVertexAttribPointer(flameShaderProgram.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(flameShaderProgram.texture);
	glVertexAttribPointer(flameShaderProgram.texture, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	(*geometry)->texture = pgr::createTexture(FLAME_TEXTURE);
	(*geometry)->numTriangles = flameNumVerticies;
}

/**
* \brief Setup wooden chunk geometry
*/
void initChunkGeometry() {
	chunk.position = glGetAttribLocation(shaderProgram.program, "position");
	chunk.PVM = glGetUniformLocation(shaderProgram.program, "PVM");
	chunk.textureCoord = glGetAttribLocation(shaderProgram.program, "texCoord");
	chunk.normalCoord = glGetAttribLocation(shaderProgram.program, "normal");
	chunk.texture = pgr::createTexture(WOOD_CHUNK_TEXTURE, true);

	glGenVertexArrays(1, &(chunk.vao));
	glBindVertexArray(chunk.vao);
	glGenBuffers(1, &chunk.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo);
	glBufferData(GL_ARRAY_BUFFER, wood_n_vertices * wood_n_attribs_per_vertex * sizeof(float), wood_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &chunk.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int) * wood_n_triangles, wood_triangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(chunk.position);
	glVertexAttribPointer(chunk.position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glGenBuffers(1, &chunk.normal);
	glBindBuffer(GL_ARRAY_BUFFER, chunk.normal);
	glBufferData(GL_ARRAY_BUFFER, wood_n_vertices * wood_n_attribs_per_vertex * sizeof(float), wood_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(chunk.normalCoord);
	glVertexAttribPointer(chunk.normalCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float))); //invalid values - change
	CHECK_GL_ERROR();

	glGenBuffers(1, &chunk.textBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, chunk.textBuffer);
	glBufferData(GL_ARRAY_BUFFER, wood_n_vertices * wood_n_attribs_per_vertex * sizeof(float), wood_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(chunk.textureCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));  //invalid values - change
	CHECK_GL_ERROR();
	glBindVertexArray(0);

	chunk.ambient = glm::vec3(woodAmbient[0], woodAmbient[1], woodAmbient[2]);
	chunk.diffuse = glm::vec3(woodDiffuse[0], woodDiffuse[1], woodDiffuse[2]);
	chunk.specular = glm::vec3(woodSpecular[0], woodSpecular[1], woodSpecular[2]);
	chunk.shiness = woodShiness;
	chunk.pos = glm::vec3(1.0f, -0.8f, 1.5f); //Change
	chunk.rot = glm::vec3(0.0f, 0.0f, 0.0f);
	chunk.size = WOOD_CHUNK_SIZE;
}

/**
* \brief Load a single mesh using assimp library
*/
bool loadSingleMesh(MeshGeometry** geometry, Shader& shader, const std::string& fname) {
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	const aiScene* scene = importer.ReadFile(fname.c_str(), 0 | ASSIMP_IMPORTER_FLAGS);

	//Check if load failed
	if (scene == NULL) {
		std::cerr << "ERROR: Assimp error " << importer.GetErrorString() << std::endl;
		*geometry = NULL;
		return false;
	}
	//Checking if have one mesh in loaded scene
	if (scene->mNumMeshes != 1) {
		std::cerr << "ERROR: Loader can't process files with many meshes! Meshes: " << scene->mNumMeshes << std::endl;
		*geometry = NULL;
		return false;
	}

	//Copying loaded scene into OpenGL
	const aiMesh* mesh = scene->mMeshes[0];
	*geometry = new MeshGeometry;

	(*geometry)->vertexMax = glm::vec3(0.0f, 0.0f, 0.0f);
	(*geometry)->vertexMin = glm::vec3(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		if ((*geometry)->vertexMax.x < mesh->mVertices[i].x) (*geometry)->vertexMax.x = mesh->mVertices[i].x;
		else if ((*geometry)->vertexMin.x > mesh->mVertices[i].x) (*geometry)->vertexMin.x = mesh->mVertices[i].x;
	
		if ((*geometry)->vertexMax.y < mesh->mVertices[i].y) (*geometry)->vertexMax.y = mesh->mVertices[i].y;
		else if ((*geometry)->vertexMin.y > mesh->mVertices[i].y) (*geometry)->vertexMin.y = mesh->mVertices[i].y;
	
		if ((*geometry)->vertexMax.z < mesh->mVertices[i].z) (*geometry)->vertexMax.z = mesh->mVertices[i].z;
		else if ((*geometry)->vertexMin.z > mesh->mVertices[i].z) (*geometry)->vertexMin.z = mesh->mVertices[i].z;
	}

	glGenBuffers(1, &((*geometry)->vbo)); //Generate new buffer
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo); //Bind buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW); //Buffer data for verticies
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);		
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);	//Upload data for normals

	float* textureCoords = new float[mesh->mNumVertices * 2];
	float* currentTextureCoords = textureCoords;
	aiVector3D vector;

	if (mesh->HasTextureCoords(0)) {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vector = (mesh->mTextureCoords[0])[i];
			*currentTextureCoords++ = vector.x;
			*currentTextureCoords++ = vector.y;
		}
	}

	glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float) * mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);
	unsigned int* indicies = new unsigned int[mesh->mNumFaces * 3];

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		indicies[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indicies[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indicies[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	glGenBuffers(1, &((*geometry)->ebo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumFaces, indicies, GL_STATIC_DRAW);

	delete[] indicies;

	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	aiString name;
	aiColor4D color;
	aiReturn ret = AI_SUCCESS;

	material->Get(AI_MATKEY_NAME, name);

	if ((ret == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	(*geometry)->diffuse = glm::vec3(color.r, color.g, color.b);

	if ((ret = aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	(*geometry)->ambient = glm::vec3(color.r, color.g, color.b);

	if ((ret == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	(*geometry)->specular = glm::vec3(color.r, color.g, color.b);

	ai_real shiness, strength;
	unsigned int max = 1;

	if ((ret = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shiness, &max)) != AI_SUCCESS) shiness = 1.0f;
	max = 1;
	if ((ret = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS) strength = 1.0f;

	(*geometry)->shiness = shiness * strength;
	(*geometry)->texture = 0;

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path;
		aiReturn textureRet = material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string texture = path.data;
		size_t file = fname.find_last_of("/\\");

		if (file != std::string::npos) texture.insert(0, fname.substr(0, file + 1));
		std::cout << "LOADING: Texture file " << texture << std::endl;
		(*geometry)->texture = pgr::createTexture(texture);
	}
	CHECK_GL_ERROR();

	if (material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
		aiString path;
		aiReturn textureRet = material->GetTexture(aiTextureType_AMBIENT, 0, &path);
		std::string texture = path.data;
		size_t file = fname.find_last_of("/\\");

		if (file != std::string::npos) texture.insert(0, fname.substr(0, file + 1));
		std::cout << "LOADING: Texture file " << texture << std::endl;
		(*geometry)->normalMap = pgr::createTexture(texture);
	}
	CHECK_GL_ERROR();

	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		aiString path;
		aiReturn textureRet = material->GetTexture(aiTextureType_SPECULAR, 0, &path);
		std::string texture = path.data;
		size_t file = fname.find_last_of("/\\");

		if (file != std::string::npos) texture.insert(0, fname.substr(0, file + 1));
		std::cout << "LOADING: Texture file " << texture << std::endl;
		(*geometry)->specularMap = pgr::createTexture(texture);
	}
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &((*geometry)->vao));
	glBindVertexArray((*geometry)->vao);
	CHECK_GL_ERROR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vbo);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(shader.position);
	glVertexAttribPointer(shader.position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(shader.normal);
	glVertexAttribPointer(shader.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(shader.texture);
	glVertexAttribPointer(shader.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	(*geometry)->numTriangles = mesh->mNumFaces;

	return true;
}

/**
* \brief Load a multiple meshes using assimp library
*/
bool loadMultipleMesh(std::vector<MeshGeometry*>& geometryBuffer, Shader& shader, const std::string& fname) {
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	const aiScene* scene = importer.ReadFile(fname.c_str(), 0 | ASSIMP_IMPORTER_FLAGS);

	if (scene == NULL) {
		std::cerr << "ERROR: Assimp error " << importer.GetErrorString() << std::endl;
		return false;
	}

	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];

		MeshGeometry* geometry = new MeshGeometry;
		geometry->vertexMax = glm::vec3(0.0f, 0.0f, 0.0f);
		geometry->vertexMin = glm::vec3(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			if (geometry->vertexMax.x < mesh->mVertices[j].x) geometry->vertexMax.x = mesh->mVertices[j].x;
			else if (geometry->vertexMin.x > mesh->mVertices[j].x) geometry->vertexMin.x = mesh->mVertices[j].x;

			if (geometry->vertexMax.y < mesh->mVertices[j].y) geometry->vertexMax.y = mesh->mVertices[j].y;
			else if (geometry->vertexMin.y > mesh->mVertices[j].y) geometry->vertexMin.y = mesh->mVertices[j].y;

			if (geometry->vertexMax.z < mesh->mVertices[j].z) geometry->vertexMax.z = mesh->mVertices[j].z;
			else if (geometry->vertexMin.z > mesh->mVertices[j].z) geometry->vertexMin.z = mesh->mVertices[j].z;
		}

		glGenBuffers(1, &((geometry)->vbo));
		glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);

		float* textureCoords = new float[mesh->mNumVertices * 2];
		float* currentTextureCoords = textureCoords;

		aiVector3D vector;

		if (mesh->HasTextureCoords(0)) {
			for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
				vector = (mesh->mTextureCoords[0])[j];
				*currentTextureCoords++ = vector.x;
				*currentTextureCoords++ = vector.y;
			}
		}

		glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float) * mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);
		unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];

		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			indices[j * 3 + 0] = mesh->mFaces[j].mIndices[0];
			indices[j * 3 + 1] = mesh->mFaces[j].mIndices[1];
			indices[j * 3 + 2] = mesh->mFaces[j].mIndices[2];
		}

		glGenBuffers(1, &((geometry)->ebo));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (geometry)->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

		delete[] indices;

		const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString name;
		aiColor4D color;
		aiReturn ret = AI_SUCCESS;

		material->Get(AI_MATKEY_NAME, name);

		if ((ret = aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		(geometry)->diffuse = glm::vec3(color.r, color.g, color.b);

		if ((ret = aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		(geometry)->ambient = glm::vec3(color.r, color.g, color.b);

		if ((ret = aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS) color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		(geometry)->specular = glm::vec3(color.r, color.g, color.b);

		ai_real shiness, strength;
		unsigned int max = 1;

		if ((ret = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shiness, &max)) != AI_SUCCESS) shiness = 1.0f;

		max = 1;
		if ((ret = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS) strength = 1.0f;

		(geometry)->shiness = shiness * strength;
		(geometry)->texture = 0;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			aiReturn textureRet = material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			std::string texturePath = path.data;
			size_t file = fname.find_last_of("/\\");

			if (file != std::string::npos) texturePath.insert(0, fname.substr(0, file + 1));
			std::cout << "LOADING: Texture file " << texturePath << std::endl;
			geometry->texture = pgr::createTexture(texturePath);
		}
		CHECK_GL_ERROR();

		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
			aiString path;
			aiReturn textureRet = material->GetTexture(aiTextureType_SPECULAR, 0, &path);
			std::string texturePath = path.data;
			size_t file = fname.find_last_of("/\\");

			if (file != std::string::npos) texturePath.insert(0, fname.substr(0, file + 1));
			std::cout << "LOADING: Texture file " << texturePath << std::endl;
			geometry->specularMap = pgr::createTexture(texturePath);
		}
		CHECK_GL_ERROR();

		if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString path;
			aiReturn textureRet = material->GetTexture(aiTextureType_NORMALS, 0, &path);
			std::string texturePath = path.data;
			size_t file = fname.find_last_of("/\\");

			if (file != std::string::npos) texturePath.insert(0, fname.substr(0, file + 1));
			std::cout << "LOADING: Texture file " << texturePath << std::endl;
			geometry->normalMap = pgr::createTexture(texturePath);
		}
		CHECK_GL_ERROR();


		glGenVertexArrays(1, &((geometry)->vao));
		glBindVertexArray((geometry)->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (geometry)->ebo);
		glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vbo);
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.position);
		glVertexAttribPointer(shader.position, 3, GL_FLOAT, GL_FALSE, 0, 0);
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.normal);
		glVertexAttribPointer(shader.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.texture);
		glVertexAttribPointer(shader.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
		CHECK_GL_ERROR();

		glBindVertexArray(0);

		geometry->numTriangles = mesh->mNumFaces;
		geometryBuffer.push_back(geometry);
	}
	return true;
}

/**
* \brief Initialize models vertex buffers and vertex arrays
*/
void initModels() {

	if (!(loadSingleMesh(&terrainGeometry, shaderProgram, TERRAIN_MODEL))) {
		std::cerr << "ERROR: Terrain model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadSingleMesh(&crowGeometry, crowShaderProgram, CROW_MODEL))) {
		std::cerr << "ERROR: Crow model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadSingleMesh(&boatGeometry, boatShaderProgram, BOAT_MODEL))) {
		std::cerr << "ERROR: Boat model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadMultipleMesh(hutGeometry, shaderProgram, HUT_MODEL))) {
		std::cerr << "ERROR: Hut model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadSingleMesh(&benchGeometry, shaderProgram, BENCH_MODEL))) {
		std::cerr << "ERROR: Bench model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadMultipleMesh(treeGeometry, shaderProgram, TREE_MODEL))) {
		std::cerr << "ERROR: Tree model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadMultipleMesh(campfireGeometry, shaderProgram, CAMPFIRE_MODEL))) {
		std::cerr << "ERROR: Campfire model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadMultipleMesh(woodGeometry, shaderProgram, WOOD_MODEL))) {
		std::cerr << "ERROR: Wood model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadMultipleMesh(largeWoodGeometry, shaderProgram, LARGE_WOOD_MODEL))) {
		std::cerr << "ERROR: Large wood model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	if (!(loadSingleMesh(&frogGeometry, frogShaderProgram, FROG_MODEL))) {
		std::cerr << "ERROR: Frog model initializing failed!" << std::endl;
	}
	CHECK_GL_ERROR();

	/**
	* \brief Initialize geometry for special objects
	*/
	initSkyboxGeometry(&skyboxGeometry);
	initFlameGeometry(&flameGeometry);
	initBannerGeometry(&rainBannerGeometry);
	initWaterGeometry(&waterAnimationGeometry);
	initWaterBlendGeometry(&waterBlendGeometry);
	initChunkGeometry();
}

/**
* \brief Initialize skybox shader
*/
void initSkyboxShader() {
	std::vector<GLuint> shaderList;
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skybox.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skybox.frag"));
	skyboxShaderProgram.program = pgr::createProgram(shaderList);

	skyboxShaderProgram.screenCoord = glGetAttribLocation(skyboxShaderProgram.program, "screenCoord");
	skyboxShaderProgram.skyboxFogSwitch = glGetUniformLocation(skyboxShaderProgram.program, "fogSkyboxChange");
	skyboxShaderProgram.daylight = glGetUniformLocation(skyboxShaderProgram.program, "dayLight");
	skyboxShaderProgram.skyboxSample = glGetUniformLocation(skyboxShaderProgram.program, "skyboxSampler");
	skyboxShaderProgram.inversePVM = glGetUniformLocation(skyboxShaderProgram.program, "inversePVmatrix");

}

/**
* \brief Initialize water animation shader
*/
void initWaterAnimationShader() {
	std::vector<GLuint> shaderList;
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/waterAnimation.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/waterAnimation.frag"));

	waterShaderProgram.program = pgr::createProgram(shaderList);
	waterShaderProgram.position = glGetAttribLocation(waterShaderProgram.program, "position");
	waterShaderProgram.textureCoord = glGetAttribLocation(waterShaderProgram.program, "texCoord");
	waterShaderProgram.pvmMatrix = glGetUniformLocation(waterShaderProgram.program, "PVMmatrix");
	waterShaderProgram.vMatrix = glGetUniformLocation(waterShaderProgram.program, "Vmatrix");
	waterShaderProgram.time = glGetUniformLocation(waterShaderProgram.program, "time");
	waterShaderProgram.textureSample = glGetUniformLocation(waterShaderProgram.program, "texSampler");
	waterShaderProgram.frameDuration = glGetUniformLocation(waterShaderProgram.program, "frameDuration");

}
/**
* \brief Initialize water blend animation shader
*/
void initWaterBlendSahder() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/waterBlend.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/waterBlend.frag"));
	waterBlendShaderProgram.program = pgr::createProgram(shaderList);

	waterBlendShaderProgram.position = glGetAttribLocation(waterBlendShaderProgram.program, "position");
	waterBlendShaderProgram.textureCoord = glGetAttribLocation(waterBlendShaderProgram.program, "texCoord");
	waterBlendShaderProgram.pvmMatrix = glGetUniformLocation(waterBlendShaderProgram.program, "PVMmatrix");
	waterBlendShaderProgram.vMatrix = glGetUniformLocation(waterBlendShaderProgram.program, "Vmatrix");
	waterBlendShaderProgram.time = glGetUniformLocation(waterBlendShaderProgram.program, "time");
	waterBlendShaderProgram.textrueSample = glGetUniformLocation(waterBlendShaderProgram.program, "texSampler");
	waterBlendShaderProgram.frameDuration = glGetUniformLocation(waterBlendShaderProgram.program, "frameDuration");
}

/**
* \brief Initialize rain banner shader
*/
void initBannerShader() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/banner.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/banner.frag"));
	rainShaderProgram.program = pgr::createProgram(shaderList);

	rainShaderProgram.position = glGetAttribLocation(rainShaderProgram.program, "position");
	rainShaderProgram.textureCoord = glGetAttribLocation(rainShaderProgram.program, "texCoord");
	rainShaderProgram.PvmMatrix = glGetUniformLocation(rainShaderProgram.program, "PVMmatrix");
	rainShaderProgram.time = glGetUniformLocation(rainShaderProgram.program, "time");
	rainShaderProgram.textureSample = glGetUniformLocation(rainShaderProgram.program, "texSampler");
}

/**
* \brief Initialize flame shader
*/
void initFlameShader() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/flame.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/flame.frag"));
	flameShaderProgram.program = pgr::createProgram(shaderList);

	flameShaderProgram.position = glGetAttribLocation(flameShaderProgram.program, "position");
	flameShaderProgram.normal = glGetAttribLocation(flameShaderProgram.program, "normal");
	flameShaderProgram.texture = glGetAttribLocation(flameShaderProgram.program, "texCoord");
	flameShaderProgram.pvmMatrix = glGetUniformLocation(flameShaderProgram.program, "PVMmatrix");
	flameShaderProgram.time = glGetUniformLocation(flameShaderProgram.program, "time");
	flameShaderProgram.textureSample = glGetUniformLocation(flameShaderProgram.program, "texSampler");
	flameShaderProgram.useTexture = glGetUniformLocation(flameShaderProgram.program, "useTexture");
	flameShaderProgram.viewMatrix = glGetUniformLocation(flameShaderProgram.program, "Vmatrix");
	flameShaderProgram.modelMatrix = glGetUniformLocation(flameShaderProgram.program, "Mmatrix");
	flameShaderProgram.normalMatrix = glGetUniformLocation(flameShaderProgram.program, "normalMatrix");
	flameShaderProgram.fogSwitch = glGetUniformLocation(flameShaderProgram.program, "fireFogSwitch");
	flameShaderProgram.alpha = glGetUniformLocation(flameShaderProgram.program, "alpha");
	flameShaderProgram.normalMap = glGetUniformLocation(flameShaderProgram.program, "normalMap");
	flameShaderProgram.useNormalMap = glGetUniformLocation(flameShaderProgram.program, "useNormalMap");
}

/**
* \brief Initialize lighting shader
*/
void initLightingShader() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/light.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/light.frag"));
	shaderProgram.program = pgr::createProgram(shaderList);

	shaderProgram.position = glGetAttribLocation(shaderProgram.program, "position");
	shaderProgram.normal = glGetAttribLocation(shaderProgram.program, "normal");
	shaderProgram.texture = glGetAttribLocation(shaderProgram.program, "texCoord");
	shaderProgram.pvmMatrix = glGetUniformLocation(shaderProgram.program, "PVMmatrix");
	shaderProgram.viewMatrix = glGetUniformLocation(shaderProgram.program, "Vmatrix");
	shaderProgram.modelMatrix = glGetUniformLocation(shaderProgram.program, "Mmatrix");
	shaderProgram.normalMatrix = glGetUniformLocation(shaderProgram.program, "normalMatrix");
	shaderProgram.time = glGetUniformLocation(shaderProgram.program, "time");
	shaderProgram.ambient = glGetUniformLocation(shaderProgram.program, "material.ambient");
	shaderProgram.diffuse = glGetUniformLocation(shaderProgram.program, "material.diffuse");
	shaderProgram.specular = glGetUniformLocation(shaderProgram.program, "material.specular");
	shaderProgram.shiness = glGetUniformLocation(shaderProgram.program, "material.shiness");
	shaderProgram.textureSample = glGetUniformLocation(shaderProgram.program, "texSampler");
	shaderProgram.useTexture = glGetUniformLocation(shaderProgram.program, "isTextureUsed");
	shaderProgram.lightPosition = glGetUniformLocation(shaderProgram.program, "sunPosition");
	shaderProgram.lightAmbient = glGetUniformLocation(shaderProgram.program, "sunAmbience");
	shaderProgram.reflectorPosition = glGetUniformLocation(shaderProgram.program, "reflectorPosition");
	shaderProgram.reflectorDirection = glGetUniformLocation(shaderProgram.program, "reflectorDirection");
	shaderProgram.reflectorSwitch = glGetUniformLocation(shaderProgram.program, "reflectorSwitch");
	shaderProgram.campfireLightSwitch = glGetUniformLocation(shaderProgram.program, "lightSwitch");
	shaderProgram.fogSwitch = glGetUniformLocation(shaderProgram.program, "fogSwitch");
	shaderProgram.dayLight = glGetUniformLocation(shaderProgram.program, "dayLight");
	shaderProgram.normalMap = glGetUniformLocation(shaderProgram.program, "normalMap");
	shaderProgram.useNormalMap = glGetUniformLocation(shaderProgram.program, "useNormalMap");
}

/**
* \brief Initialize crow shader
*/
void initCrowShader() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/crow.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/crow.frag"));
	crowShaderProgram.program = pgr::createProgram(shaderList);

	crowShaderProgram.position = glGetAttribLocation(crowShaderProgram.program, "position");
	crowShaderProgram.normal = glGetAttribLocation(crowShaderProgram.program, "normal");
	crowShaderProgram.texture = glGetAttribLocation(crowShaderProgram.program, "texCoord");
	crowShaderProgram.pvmMatrix = glGetUniformLocation(crowShaderProgram.program, "PVMmatrix");
	crowShaderProgram.viewMatrix = glGetUniformLocation(crowShaderProgram.program, "Vmatrix");
	crowShaderProgram.modelMatrix = glGetUniformLocation(crowShaderProgram.program, "Mmatrix");
	crowShaderProgram.normalMatrix = glGetUniformLocation(crowShaderProgram.program, "normalMatrix");
	crowShaderProgram.fogSwitch = glGetUniformLocation(crowShaderProgram.program, "crowFogChange");
	crowShaderProgram.alpha = glGetUniformLocation(crowShaderProgram.program, "alpha");
	crowShaderProgram.time = glGetUniformLocation(crowShaderProgram.program, "time");
	crowShaderProgram.useTexture = glGetUniformLocation(crowShaderProgram.program, "useTexture");
	crowShaderProgram.textureSample = glGetUniformLocation(crowShaderProgram.program, "texSampler");
}

/**
* \brief Initialize boat shader
*/
void initBoatShader() {
	std::vector<GLuint> shaderList;

	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/boat.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/boat.frag"));
	boatShaderProgram.program = pgr::createProgram(shaderList);

	boatShaderProgram.position = glGetAttribLocation(boatShaderProgram.program, "position");
	boatShaderProgram.normal = glGetAttribLocation(boatShaderProgram.program, "normal");
	boatShaderProgram.texture = glGetAttribLocation(boatShaderProgram.program, "texCoord");
	boatShaderProgram.pvmMatrix = glGetUniformLocation(boatShaderProgram.program, "PVMmatrix");
	boatShaderProgram.viewMatrix = glGetUniformLocation(boatShaderProgram.program, "Vmatrix");
	boatShaderProgram.modelMatrix = glGetUniformLocation(boatShaderProgram.program, "Mmatrix");
	boatShaderProgram.normalMatrix = glGetUniformLocation(boatShaderProgram.program, "normalMatrix");
	boatShaderProgram.time = glGetUniformLocation(boatShaderProgram.program, "time");
	boatShaderProgram.fogSwitch = glGetUniformLocation(boatShaderProgram.program, "fogChange");
	boatShaderProgram.normalMap = glGetUniformLocation(boatShaderProgram.program, "normalMap");
	boatShaderProgram.useNormalMap = glGetUniformLocation(boatShaderProgram.program, "useNormalMap");
	boatShaderProgram.alpha = glGetUniformLocation(boatShaderProgram.program, "alpha");
	boatShaderProgram.useTexture = glGetUniformLocation(boatShaderProgram.program, "useTexture");
	boatShaderProgram.textureSample = glGetUniformLocation(boatShaderProgram.program, "texSampler");
	boatShaderProgram.ambient = glGetUniformLocation(boatShaderProgram.program, "material.ambient");
	boatShaderProgram.diffuse = glGetUniformLocation(boatShaderProgram.program, "material.diffuse");
	boatShaderProgram.specular = glGetUniformLocation(boatShaderProgram.program, "material.specualr");
	boatShaderProgram.shiness = glGetUniformLocation(boatShaderProgram.program, "material.shiness");
}

/**
* \brief Initialize frog shader
*/
void initFrogShader() {
	std::vector<GLuint> shaderList;
	shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/frog.vert"));
	shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/frog.frag"));
	frogShaderProgram.program = pgr::createProgram(shaderList);

	frogShaderProgram.position = glGetAttribLocation(frogShaderProgram.program, "position");
	frogShaderProgram.normal = glGetAttribLocation(frogShaderProgram.program, "normal");
	frogShaderProgram.texture = glGetAttribLocation(frogShaderProgram.program, "texCoord");

	frogShaderProgram.pvmMatrix = glGetUniformLocation(frogShaderProgram.program, "PVMmatrix");
	frogShaderProgram.viewMatrix = glGetUniformLocation(frogShaderProgram.program, "Vmatrix");
	frogShaderProgram.modelMatrix = glGetUniformLocation(frogShaderProgram.program, "Mmatrix");
	frogShaderProgram.normalMatrix = glGetUniformLocation(frogShaderProgram.program, "normalMatrix");
	frogShaderProgram.fogSwitch = glGetUniformLocation(frogShaderProgram.program, "frogFogChange");
	frogShaderProgram.normalMap = glGetUniformLocation(frogShaderProgram.program, "normalMap");
	frogShaderProgram.useNormalMap = glGetUniformLocation(frogShaderProgram.program, "useNormalMap");

	frogShaderProgram.diffuse = glGetUniformLocation(frogShaderProgram.program, "diffuse");
	frogShaderProgram.ambient = glGetUniformLocation(frogShaderProgram.program, "ambient");
	frogShaderProgram.specular = glGetUniformLocation(frogShaderProgram.program, "specular");
	frogShaderProgram.shiness = glGetUniformLocation(frogShaderProgram.program, "shiness");

	frogShaderProgram.useTexture = glGetUniformLocation(frogShaderProgram.program, "useTexture");
	frogShaderProgram.textureSample = glGetUniformLocation(frogShaderProgram.program, "texSampler");
}

/**
* \brief Initialize shaders
*/
void initShaderPrograms() {
	initLightingShader();
	initSkyboxShader();
	initFlameShader();
	initBannerShader();
	initWaterAnimationShader();
	initWaterBlendSahder();
	initCrowShader();
	initBoatShader();
	initFrogShader();
	
}

/**
* \brief Cleanup shaders
*/
void deleteShaderPrograms() {
	pgr::deleteProgramAndShaders(shaderProgram.program);
	pgr::deleteProgramAndShaders(skyboxShaderProgram.program);
	pgr::deleteProgramAndShaders(flameShaderProgram.program);
	pgr::deleteProgramAndShaders(waterBlendShaderProgram.program);
	pgr::deleteProgramAndShaders(waterShaderProgram.program);
	pgr::deleteProgramAndShaders(crowShaderProgram.program);
	pgr::deleteProgramAndShaders(frogShaderProgram.program);
	pgr::deleteProgramAndShaders(boatShaderProgram.program);
	pgr::deleteProgramAndShaders(rainShaderProgram.program);
}
/**
* \brief Cleanup geometry
*/

void deleteGeometry(MeshGeometry* geometry) {
	glDeleteVertexArrays(1, &(geometry->vao));
	glDeleteBuffers(1, &(geometry->ebo));
	glDeleteBuffers(1, &(geometry->vbo));

	if (geometry->texture != 0) glDeleteTextures(1, &(geometry->texture));
}

/**
* \brief Cleanup models
*/
void deleteModels() {
	deleteGeometry(skyboxGeometry);
	deleteGeometry(terrainGeometry);
	deleteGeometry(benchGeometry);
	deleteGeometry(crowGeometry);
	deleteGeometry(frogGeometry);
	deleteGeometry(waterAnimationGeometry);
	deleteGeometry(waterBlendGeometry);
	deleteGeometry(flameGeometry);
	deleteGeometry(rainBannerGeometry);
	deleteGeometry(boatGeometry);
	deleteGeometry(frogGeometry);

	std::vector<MeshGeometry*> geaometry;
	
	geaometry = hutGeometry;
	for (std::vector<MeshGeometry*>::iterator i = geaometry.begin();
		i != geaometry.end();
		++i) {
		deleteGeometry(*i); 
	}

	geaometry = campfireGeometry;
	for (std::vector<MeshGeometry*>::iterator i = geaometry.begin();
		i != geaometry.end();
		++i) {
		deleteGeometry(*i);
	}

	geaometry = treeGeometry;
	for (std::vector<MeshGeometry*>::iterator i = geaometry.begin();
		i != geaometry.end();
		++i) {
		deleteGeometry(*i);
	}

	geaometry = woodGeometry;
	for (std::vector<MeshGeometry*>::iterator i = geaometry.begin();
		i != geaometry.end();
		++i) {
		deleteGeometry(*i);
	}

	geaometry = largeWoodGeometry;
	for (std::vector<MeshGeometry*>::iterator i = geaometry.begin();
		i != geaometry.end();
		++i) {
		deleteGeometry(*i);
	}
}
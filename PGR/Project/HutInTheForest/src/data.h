/**
 * \file	data.h
 * \author	Gleb Prachanov
 * \date	May 2024
*/

#ifndef DATA_H
#define DATA_H

/**
* \ brief Window size and name
*/
#define WIN_TITLE	"Hut in the forest"
#define WIN_WIDTH	800
#define WIN_HEIGHT	600

/**
* \ brief Scene size
*/
#define SCENE_WIDTH		1.0f
#define SCENE_HEIGHT	1.0f
#define SCENE_DEPTH		1.0f


/**
* \ brief Size of objects
*/
#define TERRAIN_SIZE	12.0f
#define HUT_SIZE	1.0f
#define LAKE_SIZE	1.0f
#define WATER_SIZE	1.0f
#define CROW_SIZE	0.6f
#define TREE_SIZE	1.0f
#define BENCH_SIZE	1.0f
#define CAMPFIRE_SIZE	0.2f
#define FLAME_SIZE		0.2f
#define WOOD_CHUNK_SIZE		0.05f
#define WOOD_SIZE	0.1f
#define LARGE_WOOD_SIZE 0.19f
#define BOAT_SIZE	1.0f
#define FROG_SIZE	0.4f

#define CAM_SENSIVITY	0.3f
#define CAMERA_POSITIONS	4


/**
* \ brief Keymap
*/
enum {
	KEY_LEFT_ARROW,
	KEY_UP_ARROW,
	KEY_DOWN_ARROW,
	KEY_RIGHT_ARROW,
	KEYS_COUNT
};


/**
* \ brief Campfire flame geometry definiton
*/
const int flameNumVerticies = 4;
const float flameVertexData[flameNumVerticies * 5] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f,	0.0f, 1.0f, 1.0f
};

/**
* \ brief Water geometry definiton
*/
const int waterNumVerticies = 4;
const float waterVertexData[waterNumVerticies * 5] = {
	-1.0f, 0.05f, 0.0f, 0.5f, 0.6f,
	-1.0f, 0.05f, 0.0f, 0.5f, 0.4f,
	1.0f, 0.05f, 0.0f, 4.0f, 0.6f,
	1.0f, -0.05f, 0.0f, 4.0f, 0.4f
};

/**
* \ brief Rain banner geometry definiton
*/
const int bannerNumVerticies = 4;
const float bannerVertexData[bannerNumVerticies * 5] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f,	 0.0f, 1.0f, 1.0f
};
#endif

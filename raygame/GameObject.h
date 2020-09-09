#pragma once
#include "raylib.h"
#include "glm.hpp"

class GameObject
{
private:
	
public:
	GameObject();
	
	// image to draw
	Texture2D sprite;

	// update logic
	void update();

	// draw sprite
	void draw() const;
};


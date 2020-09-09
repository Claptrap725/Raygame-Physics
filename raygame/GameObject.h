#pragma once
#include "raylib.h"
#include "glm.hpp"

class GameObject
{
private:
	glm::vec2 forces;
public:
	GameObject();
	
	Texture2D sprite;


	void update();

};


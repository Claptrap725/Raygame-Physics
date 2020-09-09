#include "game.h" // header to be implemented

#include <iostream> // cout
#include <ctime>	// time
#include <cstdlib>	// rand, srand

#include "raylib.h" // raylib

// inclusion order
// - header that I am implementing
// - system libraries
// - project libraries
// - other headers from this project

game::game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
	shouldRunFixedUpdate = true;

	srand(time(0));
}

void game::init() 
{
	int screenWidth = 1280;
	int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
}

bool game::update()
{
	accumulatedDeltaTime += GetFrameTime();
	if (accumulatedDeltaTime >= fixedTimeStep)
	{
		shouldRunFixedUpdate = true;
		accumulatedDeltaTime = 0;
	}

	for (auto& obj : GameObjects)
	{
		obj.update();
	}
	
	return !WindowShouldClose();
}

void game::fixedUpdate()
{
	shouldRunFixedUpdate = false;
	for (auto& obj : RigidBodies)
	{
		obj.fixedUpdate(fixedTimeStep);
	}
}

void game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto& obj : GameObjects)
	{
		obj.draw();
	}

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
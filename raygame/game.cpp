#include <iostream>
#include "game.h"
#include "raylib.h"

game::game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
	shouldRunFixedUpdate = true;
}

void game::init() 
{
	int screenWidth = 800;
	int screenHeight = 450;

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

	std::cout << "Update\n";
	
	return !WindowShouldClose();
}

void game::fixedUpdate()
{
	shouldRunFixedUpdate = false;
	std::cout << "Fixed Update\n";
}

void game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
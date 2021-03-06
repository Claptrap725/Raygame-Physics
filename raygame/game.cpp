#include "game.h" // header to be implemented

#include <iostream> // cout
#include <ctime>	// time
#include <cstdlib>	// rand, srand

#include "raylib.h" // raylib
#include "enumUtils.h"

// inclusion order
// - header that I am implementing
// - system libraries
// - project libraries
// - other headers from this project

collisionMap setupCollisionChecks()
{
	collisionMap map;

	map[static_cast<collisionPair>(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	map[static_cast<collisionPair>(shapeType::AABB | shapeType::AABB)] = checkBoxBox;
	map[static_cast<collisionPair>(shapeType::CIRCLE | shapeType::AABB)] = checkCircleBox;

	return map;
}

collisionMap game::collisionCheckers = setupCollisionChecks();
std::vector<RigidBody> game::rigidBodies;

game::game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
	shouldRunFixedUpdate = true;

	srand((unsigned int)time(0));
}

void game::init() 
{
	InitWindow(1280, 720, "raylib [core] example - basic window");

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

	for (auto& obj : rigidBodies)
	{
		obj.update();
	}

	if (IsMouseButtonPressed(0))
	{
		auto cursorPos = GetMousePosition();

		RigidBody spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.mass = (float)(rand() % 30) + 40;
		spawn.collider.colliderShape = shapeType::CIRCLE;
		spawn.collider.circleData.radius = spawn.mass/3.1415f*2;
		
		if (IsKeyDown(KEY_LEFT_CONTROL))
			spawn.useGravity = false;

		rigidBodies.push_back(spawn);
	}

	if (IsMouseButtonPressed(1))
	{
		auto cursorPos = GetMousePosition();

		RigidBody spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.mass = (float)((rand() % 30) + 40);
		spawn.collider.colliderShape = shapeType::AABB;
		spawn.collider.aabbData.halfExtents = glm::vec2{ glm::sqrt(spawn.mass)*5,  glm::sqrt(spawn.mass)*5 };
		
		if (IsKeyDown(KEY_LEFT_CONTROL))
			spawn.useGravity = false;

		rigidBodies.push_back(spawn);
	}

	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
	{
		for (auto& obj : rigidBodies)
		{
			auto cursorPosRaw = GetMousePosition();
			glm::vec2 cursorPos = { cursorPosRaw.x, cursorPosRaw.y };
			if (checkPoint(cursorPos, obj.pos, obj.collider))
			{
				obj.pos = cursorPos;
				obj.vel = { 0,0 };
				break;
			}
		}
	}
	
	return !WindowShouldClose();
}

void game::fixedUpdate()
{
	shouldRunFixedUpdate = false;

	for (auto& lhs : rigidBodies)
	{
		for (auto& rhs : rigidBodies)
		{
			if (&lhs == &rhs) continue;

			auto *first = &lhs;
			auto *second = &rhs;

			if (static_cast<uint8_t>(lhs.collider.colliderShape) >
				static_cast<uint8_t>(rhs.collider.colliderShape))
			{
				first = &rhs;
				second = &lhs;
			}

			collisionPair pairType = static_cast<collisionPair>(lhs.collider.colliderShape | rhs.collider.colliderShape);

			bool collision = collisionCheckers[pairType](first->pos, first->collider, second->pos, second->collider);


			if (collision)
			{
				first->onCollision(second);
				second->onCollision(first);
			}
		}
	}

	for (auto& obj : rigidBodies)
	{
		obj.fixedUpdate(fixedTimeStep);
	}
}

void game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto& obj : rigidBodies)
	{
		obj.draw();
	}

	DrawText("Left click to spawn circle.", 10, 10, 30, BLACK);
	DrawText("Right click to spawn box.", 10, 50, 30, BLACK);
	DrawText("Hold left CTRL to spawn objects without gravity.", 10, 90, 30, BLACK);
	DrawText("Middle click to drag objects.", 10, 130, 30, BLACK);

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
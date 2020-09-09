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

	for (auto& obj : gameObjects)
	{
		obj.update();
	}

	if (IsMouseButtonPressed(0))
	{
		auto cursorPos = GetMousePosition();

		RigidBody spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.mass = (rand() % 10) + 1;
		spawn.collider.colliderShape = shapeType::CIRCLE;
		spawn.collider.circleData.radius = spawn.mass;
		spawn.addImpulse({ 1000,0 });


		rigidBodies.push_back(spawn);
	}

	if (IsMouseButtonPressed(1))
	{
		auto cursorPos = GetMousePosition();

		RigidBody spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.mass = (rand() % 10) + 1;
		spawn.collider.colliderShape = shapeType::AABB;
		spawn.collider.aabbData.halfExtents = glm::vec2{ glm::sqrt(spawn.mass),  glm::sqrt(spawn.mass) };
		spawn.addImpulse({ 0,-1000 });


		rigidBodies.push_back(spawn);
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

			//if (lhs.collider.colliderShape == shapeType::CIRCLE && rhs.collider.colliderShape == shapeType::CIRCLE)
			//	collision = checkCircleCircle(lhs.pos, lhs.collider, rhs.pos, rhs.collider);


			if (collision)
				std::cout << "COLLISION!\n";
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

	for (auto& obj : gameObjects)
	{
		obj.draw();
	}

	for (auto& obj : rigidBodies)
	{
		obj.draw();
	}

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
#pragma once
#include <vector>
#include <unordered_map>
#include "RigidBody.h"

using collisionPair = uint8_t;
using collisionFunc = bool(*)(glm::vec2, collider, glm::vec2, collider);
using collisionMap = std::unordered_map<collisionPair, collisionFunc>;

class game
{
	float accumulatedDeltaTime;

	static collisionMap collisionCheckers;
public:
	static std::vector<RigidBody> rigidBodies;

	// When true game will run fixedUpdate() this frame
	bool shouldRunFixedUpdate;
	// Time between every fixedUpdate()
	float fixedTimeStep;

	game();

	// Create window and initialize rendering
	void init();
	// Update logic for game
	// - Returns true if window should stay open
	bool update();
	// Update physics for game
	void fixedUpdate();
	// Render objects to screen
	void draw() const;
	// Destroy window and deinitialize game
	void exit();
};


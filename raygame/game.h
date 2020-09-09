#pragma once
class game
{
	float accumulatedDeltaTime;

public:
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

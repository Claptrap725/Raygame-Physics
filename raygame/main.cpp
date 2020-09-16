/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "glm.hpp"
#include "game.h"

game app;

int main()
{
	app.init();
	app.fixedTimeStep = 1.0f / 30.0f;
	
	bool shouldRun = true;

	while (shouldRun)    // Detect window close button or ESC key
	{
		shouldRun = app.update();

		if (app.shouldRunFixedUpdate)
			app.fixedUpdate();
		
		app.draw();
	}

	app.exit();

	return 0;
}
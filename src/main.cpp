/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Body.h"
#include "World.h"
#include "GravitationalEffector.h"
#include "../AreaEffector.h"
#include "DragEffector.h"

World world = World();

int main ()
{
	//SetRandomSeed(GetRandomFloat());
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - previousTime;
	float DT = 0.0f;
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;
	bool simulate = true;

	world.AddEffector(new GravitationalEffector(Vector2{ 900, 600 }, 200, 100000.0f));
	world.AddEffector(new AreaEffector(Vector2{ 200, 200 }, 200, 180, 100000.0f));
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
			Body body;
			body.position = GetMousePosition();
			float angle = GetRandomFloat() * (2 * PI);
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.velocity = direction * GetRandomFloat() * 200;
			body.acceleration = Vector2{ 0,0 };
			body.size = GetRandomValue(10, 40);
			body.damping = 0.2f;
			body.mass = body.size;
			body.bodyType = BodyType::Dynamic;
			body.restutuion = 0.99f;

			world.AddBody(body);
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
			Body body;
			body.position = GetMousePosition();
			body.size = GetRandomValue(10, 40);
			body.mass = body.size;
			body.bodyType = BodyType::Static;

			world.AddBody(body);
		}


		if (IsKeyPressed(KEY_SPACE)) { simulate = !simulate; }

		if (simulate) {
			currentTime = std::chrono::high_resolution_clock::now();
			elapsed = currentTime - previousTime;
			DT = elapsed.count();
			previousTime = currentTime;

			// update
			timeAccum += DT;
			// Run as many fixed steps as needed and subtract the accumulator.
			while (timeAccum >= fixedTimeStep) {
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		std::string fpsText = "FPS: ";
		fpsText += GetFPS();

		// draw some text using the default font
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		world.Draw();
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

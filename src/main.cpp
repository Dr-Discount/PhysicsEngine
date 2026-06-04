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
#include "PointEffector.h"
#include "world_camera.h"
#include "Spring.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

World world = World();
GuiPhysicsState state;

void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());

	Effector* effector = nullptr;
	switch ((EffectorType)state.EffectorTypeActive)
	{
	case(EffectorType::Drag):
		world.AddEffector(new DragEffector(position, state.EffectorSizeValue, 200));
		break;
	case(EffectorType::Point):
		world.AddEffector(new PointEffector(position, state.EffectorSizeValue, state.EffectorForceValue));
		break;
	case(EffectorType::Gravitation):
		world.AddEffector(new GravitationalEffector(position, state.EffectorSizeValue, state.EffectorForceValue));
		break;
	case(EffectorType::Area):
		world.AddEffector(new AreaEffector(position, state.EffectorSizeValue, state.EffectorAngleValue, state.EffectorForceValue));
		break;
	}

	if (effector) world.AddEffector(effector);
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;
	body.position = camera.ScreenToWorld(GetMousePosition());
	float angle = GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.velocity = direction * GetRandomFloat() * 200;
	body.acceleration = Vector2{ 0,0 };
	body.size = state.BodySizeValue;
	body.damping = state.BodyDampingValue;
	body.mass = state.BodyMassValue;
	body.bodyType = (BodyType)state.BodyTypeActive;
	body.restutuion = state.BodyRestitutionValue;
	body.color = ColorFromHSV(GetRandomFloat(360.0f), 1, 1);

	world.AddBody(body);
}

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
	InitWindow(1400, 1000, "Hello Raylib");
	state = InitGuiPhysics();
	GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 100);
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	world.SetSpringMultiplier(state.SpringStiffnessValue);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		if (IsKeyPressed(KEY_TAB)) {
			state.PhysicsPanelActive = !state.PhysicsPanelActive;
		}
		bool DontSpawn = CheckCollisionPointRec(GetMousePosition(), Rectangle{ state.anchor02.x, state.anchor02.y, 304, 664}) && state.PhysicsPanelActive;

		if (!DontSpawn) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
				AddBody(world, world_camera);
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
				selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
				AddEffector(world, world_camera);
			}
			if (selectedBody) {
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
					if (IsKeyDown(KEY_LEFT_CONTROL)) {
						Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
						Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, state.SpringStiffnessValue);
						selectedBody->AddForce(force);

						DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
					}
					else {
						connectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
					}
				}
				else {
					if (selectedBody && connectedBody) {
						float distance = Vector2Distance(selectedBody->position, connectedBody->position);
						world.AddSpring(*selectedBody, *connectedBody, 40, state.SpringStiffnessValue, state.SpringDampingValue);
					}
					selectedBody = nullptr;
					connectedBody = nullptr;
				}
			} 
		}

		World::SetGravity({ 0, -state.GravityValue });

		if (state.SimulateActive) {
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

		world_camera.Begin();
		world.Draw();
		DrawCircleLinesV(world_camera.ScreenToWorld(GetMousePosition()), state.BodySizeValue, BLUE);
		
		if (selectedBody) {
			DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, RED);
		}
		
		world_camera.End();

		GuiPhysics(&state);
		
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
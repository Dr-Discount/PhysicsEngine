#pragma once
#include "Body.h"
#include "raymath.h"
#include <vector>
#include <chrono>
#include "Random.h"

class World {
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - previousTime;

	std::vector<Body> bodies;
	Vector2 gravity{ 0, 9.81f };

public:
	World() {}
	float DT = 0.0f;
	Random random;

	void Step(float DT);
	void Draw();
	void AddBody(const Body& body);
};
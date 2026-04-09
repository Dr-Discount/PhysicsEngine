#pragma once
#include "Body.h"
#include "raymath.h"
#include <vector>
#include <chrono>
#include "Random.h"
#include "Collision.h"

class World {
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;
	Vector2 gravity{ 0, 9.81f };

public:
	World() {}

	std::vector<Body>& GetBodies() { return bodies; }
	void Step(float DT);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
};
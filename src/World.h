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
	static Vector2 gravity;


public:
	World() {}

	std::vector<Body>& GetBodies() { return bodies; }
	void Step(float DT);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }
	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }
	
private:
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };
};
#pragma once
#include "Body.h"
#include "raymath.h"
#include <vector>
#include <chrono>
#include "Random.h"
#include "Collision.h"
#include "Spring.h"

class World {
public:
	World() {}

	std::vector<Body>& GetBodies() { return bodies; }
	void Step(float DT);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness = 1, float damping = 1);
	void UpdateCollision();
	
	Body* GetBodyIntersect(Vector2 position);

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }
	void SetSpringMultiplier(float multiplier) { this->springMultiplier = multiplier; }

	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }

private:
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };
	
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<class Spring*> springs;
	std::vector<Contact> contacts;
	static Vector2 gravity;
	float springMultiplier;
};
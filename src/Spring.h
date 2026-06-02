#pragma once
#include "raylib.h"

class Spring {
public:
	struct Body* bodyA;
	struct Body* bodyB;

	float restLength;
	float stiffness;
	float damping;

public:
	Spring() = default;
	Spring(Body* a, Body* b, float restLength, float stiffness = 1, float damping = 1) :
		bodyA{ a },
		bodyB{ b },
		restLength{ restLength },
		stiffness{ stiffness },
		damping{ damping }
	{ }

	void Apply(float multiplier = 1);
	void Draw();

	static Vector2 GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness = 1);
};
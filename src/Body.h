#pragma once
#include <raylib.h>
#include <raymath.h>

struct Body {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float size;
    float mass = 1;
    float restutuion = 0.9f;

	void ExplicitEular(Body& body, float DT);
	void SemiImplicitEular(float DT);
	void Step(float DT);
	void AddForce(Vector2 force);
    void Draw() const;
};
#pragma once
#include <raylib.h>
#include <raymath.h>

enum BodyType {
    Dynamic,
    Kinematic,
    Static
};

struct Body {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float size;
    float mass = 1;
    float restutuion = 0.9f;
    float gravityScale = 10.0f;
    float damping = 0.0f;
    BodyType bodyType = BodyType::Dynamic;

	void ExplicitEular(float DT);
	void SemiImplicitEular(float DT);
	void Step(float DT);
	void AddForce(Vector2 force);
    void Draw() const;
};
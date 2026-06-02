#include "Spring.h"
#include "raymath.h"
#include "Body.h"

void Spring::Apply(float multiplier) {
	Vector2 force = GetSpringForce(bodyA->position, bodyB->position, restLength, stiffness * multiplier);

	Vector2 direction = bodyB->position - bodyA->position;
	Vector2 ndirection = Vector2Normalize(direction);

	Vector2 rv = bodyB->velocity - bodyA->velocity;
	float dampingFactor = Vector2DotProduct(ndirection, rv);

	Vector2 dampingForce = ndirection * (damping * dampingFactor);
	force -= dampingForce;

	bodyA->AddForce(Vector2Negate(force));
	bodyB->AddForce(force);
}

void Spring::Draw() {
	DrawLineV(bodyA->position, bodyB->position, WHITE);
}

Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness)
{
	Vector2 direction = positionB - positionA;
	float length = Vector2Length(direction);
	float displacment = length - restLength;
	float magitude = -(displacment * stiffness);

	return Vector2Normalize(direction) * magitude;
}

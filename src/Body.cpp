#include "Body.h"

void Body::ExplicitEular(Body& body, float DT) {
	body.position += body.velocity * DT;
	body.velocity += body.acceleration * DT;
}

void Body::SemiImplicitEular(float DT) {
	velocity += acceleration * DT;
	position += velocity * DT;
}

void Body::Step(float DT) {
	SemiImplicitEular(DT);
}

void Body::AddForce(Vector2 force) {
	acceleration += force / mass;
}

void Body::Draw() const {
	DrawCircleV(position, size, RED);
}

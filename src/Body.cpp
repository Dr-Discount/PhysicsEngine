#include "Body.h"

void Body::ExplicitEular(float DT) {
	position += velocity * DT;
	velocity += acceleration * DT;
	//velocity *= (1.0f / (1.0f + damping));
}

void Body::SemiImplicitEular(float DT) {
	velocity += acceleration * DT;
	position += velocity * DT;
	velocity *= (1.0f / (1.0f + (damping * DT)));
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

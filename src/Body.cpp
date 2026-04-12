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
	if (bodyType == BodyType::Static)
		return;
	SemiImplicitEular(DT);
}

void Body::AddForce(Vector2 force) {
	if (bodyType == BodyType::Static)
		return;
	acceleration += force / mass;
}

void Body::Draw() const {
	if(bodyType == BodyType::Dynamic)
		DrawCircleV(position, size, RED);
	else
		DrawCircleV(position, size, YELLOW);
}

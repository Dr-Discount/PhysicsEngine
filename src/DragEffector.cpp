#pragma once
#include "DragEffector.h"

void DragEffector::Apply(std::vector<Body>& ibodies) {
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies) {
		Vector2 force = Vector2Negate(body->velocity) * drag;
		body->AddForce(force);
	}
}

void DragEffector::Draw() {
	Effector::Draw();
	DrawCircleV(position, size, Fade(BLUE, 0.3f));
}
#include "DragEffector.h"

void DragEffector::Apply(std::vector<Body>& ibodies) {
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	const float MAX_FORCE = 10000.0f; // safety cap to avoid extreme impulses

	for (auto& body : bodies) {
		if (body->bodyType != BodyType::Dynamic) continue;

		Vector2 force = Vector2Scale(body->velocity, -drag * body->mass);

		float mag = Vector2Length(force);
		if (mag > MAX_FORCE && mag > 1e-6f) {
			force = Vector2Scale(Vector2Normalize(force), MAX_FORCE);
		}

		body->AddForce(force);
	}
}

void DragEffector::Draw() {
	Effector::Draw();
	DrawCircleV(position, size, Fade(BLUE, 0.3f));
}
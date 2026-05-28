#include "AreaEffector.h"

void AreaEffector::Apply(std::vector<Body>& ibodies) {
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies) {
		Vector2 direction = Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
		Vector2 force = direction * -forceMagitude;
		body->AddForce(force);
	}
}

void AreaEffector::Draw() {
	Effector::Draw();
	DrawCircleV(position, size, Fade(YELLOW, 0.3f));
}
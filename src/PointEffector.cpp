#include "PointEffector.h"

void PointEffector::Apply(std::vector<Body>& bodies) {
	for (auto& body : bodies) {
		Vector2 direction = position - body.position;
		if (Vector2Length(direction) < size) {
			Vector2 force = Vector2Normalize(direction) * -forceMagitude;
			body.AddForce(force);
		}
	}
}

void PointEffector::Draw(){
	DrawCircleLinesV(position, size, GREEN);
}

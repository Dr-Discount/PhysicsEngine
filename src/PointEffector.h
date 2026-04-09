#pragma once
#include "Effector.h"

class PointEffector : public Effector {
public:
	PointEffector(Vector2 Position, float Size, float ForceMagitude) {
		position = Position;
		size = Size;
		forceMagitude = ForceMagitude;
	}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;
private:
	Vector2 position;
	float size;
	float forceMagitude;
};
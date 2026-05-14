#pragma once
#include "Effector.h"

class PointEffector : public Effector {
public:
	PointEffector(Vector2 Position, float Size, float ForceMagitude) :
		Effector(Position, Size),
		forceMagitude{ ForceMagitude }
	{}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;
private:
	
	float forceMagitude;
};
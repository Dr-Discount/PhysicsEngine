#pragma once
#include "Effector.h"
class AreaEffector : public Effector {
public:
	AreaEffector(Vector2 Position, float Size, float Angle, float ForceMagitude) :
		Effector(Position, Size),
		forceMagitude{ ForceMagitude },
		angle{Angle}
	{
	}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;
private:
	float angle;
	float forceMagitude;
};


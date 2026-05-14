#pragma once
#include "Effector.h"
class DragEffector : public Effector {
public:
	DragEffector(Vector2 Position, float Size, float drag) :
		Effector(Position, Size),
		drag{drag}
	{
	}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;
private:
	float drag
};

#pragma once
#include "raymath.h"
#include <resource_dir.h>

struct Random {

	float GetRandomFloat() {
		return GetRandomValue(0, 10000) / (float)10000;
	}
	float GetRandomFloat(float Max) {
		return GetRandomValue(0, Max);
	}
	float GetRandomFloat(float Min, float Max) {
		return GetRandomValue(Min, Max);
	}
};
#pragma once
#include "raylib.h"

	inline float GetRandomFloat() {
		return GetRandomValue(0, 10000) / (float)10000;
	}
	inline float GetRandomFloat(float Max) {
		return GetRandomFloat() * Max;
	}
	inline float GetRandomFloat(float Min, float Max) {
		return GetRandomFloat() * (Max - Min);
	}
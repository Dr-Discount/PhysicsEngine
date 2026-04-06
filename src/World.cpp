#include "World.h"

void World::Step(float DT) {
	currentTime = std::chrono::high_resolution_clock::now();
	elapsed = currentTime - previousTime;
	DT = elapsed.count();
	
	if (DT > 0.05f) DT = 0.05f;

	previousTime = currentTime;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
		Body body;
		body.position = GetMousePosition();
		float angle = random.GetRandomFloat() * (2 * PI);
		Vector2 direction;
		direction.x = cosf(angle);
		direction.y = sinf(angle);

		body.velocity = direction * random.GetRandomFloat() * 1000;
		body.acceleration = Vector2{ 0,0 };
		body.size = GetRandomValue(3, 10);

		AddBody(body);
	}

	for (auto& body : bodies) {
		body.acceleration = Vector2{ 0, 0 };

		body.AddForce(gravity * 100);

		if (body.position.x > GetScreenWidth()) {
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.x < 0) {
			body.position.x = 0 + body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.y > GetScreenHeight()) {
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restutuion;
		}

	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Vector2 position = GetMousePosition();

		for (auto& body : bodies) {
			Vector2 direction = position - body.position;
			if (Vector2Length(direction) < 100.0f) {
				Vector2 force = Vector2Normalize(direction) * -10000.0f;
				body.AddForce(force);
			}
		}

		DrawCircleLinesV(position, 100, WHITE);
	}
	for (auto& body : bodies)
		body.Step(DT);
}

void World::Draw() {
	DrawCircleV(GetMousePosition(), 5, BLUE);
	for (const auto& body : bodies) {
		body.Draw();
	}
}

void World::AddBody(const Body& body) {
	bodies.push_back(body);
}

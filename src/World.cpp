#include "World.h"
#include "Effector.h"
#include "PointEffector.h"

void World::Step(float DT) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
		Body body;
		body.position = GetMousePosition();
		float angle = GetRandomFloat() * (2 * PI);
		Vector2 direction;
		direction.x = cosf(angle);
		direction.y = sinf(angle);

		body.velocity = direction * GetRandomFloat() * 500;
		body.acceleration = Vector2{ 0,0 };
		body.size = GetRandomValue(2, 50);
		body.damping = 0.3f;
		body.mass = body.size;

		AddBody(body);
	}

	contacts.clear();
	Contact::CreateContacts(bodies, contacts);
	Contact::SeparateContacts(contacts);

	for (auto& body : bodies) {
		body.acceleration = Vector2{ 0, 0 };

		//body.AddForce(gravity * 100);

		if (body.position.x + body.size > GetScreenWidth()) {
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.x - body.size < 0) {
			body.position.x = 0 + body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.y - body.size < 0) {
			body.position.y = 0 + body.size;
			body.velocity.y *= -body.restutuion;
		}
		if (body.position.y + body.size > GetScreenHeight()) {
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restutuion;
		}

	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Vector2 position = GetMousePosition();
		
		Effector* effector = new PointEffector(position, 150.0f, 7500.0f);
		AddEffector(effector);
	}

	for (auto& effector : effectors) effector->Apply(bodies);
	for (auto& body : bodies) body.Step(DT);
}

void World::Draw() {
	DrawCircleV(GetMousePosition(), 5, BLUE);
	for (auto& effector : effectors) effector->Draw();
	for (const auto& body : bodies) body.Draw();
}

void World::AddBody(const Body& body) {
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

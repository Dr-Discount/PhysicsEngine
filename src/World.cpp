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

		body.velocity = direction * GetRandomFloat() * 200;
		body.acceleration = Vector2{ 0,0 };
		body.size = GetRandomValue(10, 40);
		body.damping = 0.2f;
		body.mass = body.size;
		body.bodyType = BodyType::Dynamic;
		body.restutuion = 0.5f;

		AddBody(body);
	} else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
		Body body;
		body.position = GetMousePosition();
		body.size = GetRandomValue(10, 40);
		body.mass = body.size;
		body.bodyType = BodyType::Static;

		AddBody(body);
	}

	// reset accelerations
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };

	// apply effectors (forces) before integrating
	for (auto& effector : effectors) effector->Apply(bodies);

	// integrate bodies
	for (auto& body : bodies) body.Step(DT);

	// simple world bounds (position correction + velocity bounce)
	for (auto& body : bodies) {
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

	// collisions: detect -> resolve velocities -> positional correction
	contacts.clear();
	Contact::CreateContacts(bodies, contacts);
	Contact::ResolveContacts(contacts);
	Contact::SeparateContacts(contacts);

	// right-mouse creates a point effector — consider creating only on press to avoid accumulating effectors
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Vector2 position = GetMousePosition();
		
		Effector* effector = new PointEffector(position, 150.0f, 7500.0f);
		AddEffector(effector);
	}
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

#include "World.h"
#include "Effector.h"
#include "PointEffector.h"

Vector2 World::gravity = { 0, 9.81f };

void World::Step(float DT) {
	// reset accelerations
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };

	// apply effectors (forces) before integrating
	for (auto& effector : effectors) effector->Apply(bodies);

	// integrate bodies
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale);
	for (auto& body : bodies) body.Step(DT);

	// simple world bounds (position correction + velocity bounce)
	for (auto& body : bodies) {
		if (body.position.x + body.size > boundsMax.x) {
			body.position.x = boundsMax.x - body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.x - body.size < boundsMin.x) {
			body.position.x = boundsMin.x + body.size;
			body.velocity.x *= -body.restutuion;
		}
		if (body.position.y - body.size < boundsMin.y) {
			body.position.y = boundsMin.y + body.size;
			body.velocity.y *= -body.restutuion;
		}
		if (body.position.y + body.size > boundsMax.y) {
			body.position.y = boundsMax.y - body.size;
			body.velocity.y *= -body.restutuion;
		}
	}

	// collisions: detect -> resolve velocities -> positional correction
	contacts.clear();
	Contact::CreateContacts(bodies, contacts);
	Contact::ResolveContacts(contacts);
	Contact::SeparateContacts(contacts);
}

void World::Draw() {
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

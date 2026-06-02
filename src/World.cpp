#include "World.h"
#include "Effector.h"
#include "PointEffector.h"
#include "Spring.h"
#include <cstddef>

Vector2 World::gravity = { 0, 9.81f };

void World::Step(float DT) {
	// update gravity
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale);

	// force effectors
	for (auto& effector : effectors) effector->Apply(bodies);

	// spring
	for (auto& spring : springs) spring->Apply(springMultiplier);

	// integrator
	for (auto& body : bodies) body.Step(DT);
	for (int i = 0; i < 4; i++) UpdateCollision();
}

void World::Draw() {
	DrawLineV(Vector2{ 0, boundsMin.y }, Vector2{ 0, boundsMax.y }, WHITE);
	for (float x = 5; x < (boundsMax.x - boundsMin.x) * 0.5; x += 5) {
		DrawLineV(Vector2{ x, boundsMin.y }, Vector2{ x, boundsMax.y }, GRAY);
		DrawLineV(Vector2{ -x, boundsMin.y }, Vector2{ -x, boundsMax.y }, GRAY);
	}
	DrawLineV(Vector2{ boundsMin.x, 0 }, Vector2{ boundsMax.x, 0 }, WHITE);
	for (float y = 5; y < (boundsMax.y - boundsMin.y) * 0.5; y += 5) {
		DrawLineV(Vector2{ boundsMin.x, y }, Vector2{ boundsMax.x, y }, GRAY);
		DrawLineV(Vector2{ boundsMin.x, -y }, Vector2{ boundsMax.x, -y }, GRAY);
	}

	for (auto& effector : effectors) effector->Draw();
	for (auto& spring : springs) spring->Draw();
	for (const auto& body : bodies) body.Draw();
}

void World::AddBody(const Body& body) {
	// capture old base pointer so we can detect a reallocation move
	Body* oldBase = bodies.empty() ? nullptr : bodies.data();

	// push the new body (may reallocate)
	bodies.push_back(body);

	// if the underlying storage moved, rebase spring pointers
	Body* newBase = bodies.data();
	if (oldBase != nullptr && newBase != oldBase) {
		for (auto& s : springs) {	
			// skip null pointers (defensive)
			if (!s) continue;

			// compute indices relative to old base and apply to new base
			if (s->bodyA) {
				std::ptrdiff_t idxA = s->bodyA - oldBase;
				if (idxA >= 0 && (size_t)idxA < bodies.size()) s->bodyA = newBase + idxA;
				else s->bodyA = nullptr; // defensive: mark invalid if out of range
			}
			if (s->bodyB) {
				std::ptrdiff_t idxB = s->bodyB - oldBase;
				if (idxB >= 0 && (size_t)idxB < bodies.size()) s->bodyB = newBase + idxB;
				else s->bodyB = nullptr;
			}
		}
	}
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping)
{
	springs.push_back(new Spring(&bodyA, &bodyB, restLength, stiffness, damping));
}

void World::UpdateCollision()
{
	// reset acceleration
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 }; for (auto& body : bodies) {
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

Body* World::GetBodyIntersect(Vector2 position)
{
	for (auto& body : bodies) {
		if (CheckCollisionPointCircle(position, body.position, body.size)) {
			return &body;
		}
	}

	return nullptr;
}
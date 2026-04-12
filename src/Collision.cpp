#include "Collision.h"
#include <algorithm>

static bool Intersects(const Body& bodyA, const Body& bodyB)
{
    float distance = Vector2Distance(bodyA.position, bodyB.position);
    float radiusSum = bodyA.size + bodyB.size;
    return distance < radiusSum;
}

void Contact::CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < (int)bodies.size(); i++)
	{
		for (int j = i + 1; j < (int)bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.bodyType == BodyType::Static && bodyB.bodyType == BodyType::Static)
				continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distance = Vector2Length(direction);

				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;

				if (distance > 1e-6f)
				{
					contact.normal = Vector2Normalize(direction);
				}
				else
				{
					contact.normal = Vector2{ 1.0f, 0.0f };
				}

				contact.restitution = std::min(contact.bodyA->restutuion, contact.bodyB->restutuion);

				contacts.push_back(contact);
			}
		}
	}
}

void Contact::SeparateContacts(std::vector<Contact>& contacts)
{
	const float percent = 0.8f;
	const float slop = 0.01f;   

	for (auto& contact : contacts)
	{
		float invMassA = (contact.bodyA->bodyType == BodyType::Dynamic && contact.bodyA->mass > 0.0f) ? 1.0f / contact.bodyA->mass : 0.0f;
		float invMassB = (contact.bodyB->bodyType == BodyType::Dynamic && contact.bodyB->mass > 0.0f) ? 1.0f / contact.bodyB->mass : 0.0f;
		float totalInverseMass = invMassA + invMassB;

		if (totalInverseMass == 0.0f) continue;

		float penetration = std::max(contact.depth - slop, 0.0f);
		if (penetration <= 0.0f) continue;

		Vector2 correction = contact.normal * (penetration / totalInverseMass * percent);

		if (contact.bodyA->bodyType == BodyType::Dynamic)
			contact.bodyA->position = contact.bodyA->position + (correction * invMassA);
		if (contact.bodyB->bodyType == BodyType::Dynamic)
			contact.bodyB->position = contact.bodyB->position - (correction * invMassB);
	}
}

void Contact::ResolveContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float invMassA = (contact.bodyA->bodyType == BodyType::Dynamic && contact.bodyA->mass > 0.0f) ? 1.0f / contact.bodyA->mass : 0.0f;
		float invMassB = (contact.bodyB->bodyType == BodyType::Dynamic && contact.bodyB->mass > 0.0f) ? 1.0f / contact.bodyB->mass : 0.0f;
		float totalInverseMass = invMassA + invMassB;

		if (totalInverseMass == 0.0f) continue;

		Vector2 velA = (contact.bodyA->bodyType == BodyType::Dynamic) ? contact.bodyA->velocity : Vector2{0.0f, 0.0f};
		Vector2 velB = (contact.bodyB->bodyType == BodyType::Dynamic) ? contact.bodyB->velocity : Vector2{0.0f, 0.0f};

		Vector2 rv = velA - velB;
		float nv = Vector2DotProduct(rv, contact.normal);

		if (nv > 0.0f) continue;
		float impulseMagnitude = -(1.0f + contact.restitution) * nv / totalInverseMass;

		Vector2 impulse = contact.normal * impulseMagnitude;

		if (contact.bodyA->bodyType == BodyType::Dynamic)
			contact.bodyA->velocity = contact.bodyA->velocity + impulse * invMassA;
		if (contact.bodyB->bodyType == BodyType::Dynamic)
			contact.bodyB->velocity = contact.bodyB->velocity - impulse * invMassB;
	}
}

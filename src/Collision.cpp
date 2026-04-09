#include "Collision.h"

static bool Intersects(const Body& bodyA, const Body& bodyB)
{
    float distance = Vector2Distance(bodyA.position, bodyB.position);
    float radiusSum = bodyA.size + bodyB.size;
    return distance < radiusSum;
}

void Contact::CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			//if (< bodyA is not dynamic and bodyB is not dynamic) continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distance = Vector2Length(direction);

				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);

				contacts.push_back(contact);
			}
		}
	}
}

void Contact::SeparateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float invMassA = (contact.bodyA->mass > 0.0f) ? 1.0f / contact.bodyA->mass : 0.0f;
		float invMassB = (contact.bodyB->mass > 0.0f) ? 1.0f / contact.bodyB->mass : 0.0f;
		float totalInverseMass = invMassA + invMassB;

		if (totalInverseMass == 0.0f) continue;

		Vector2 separation = contact.normal * (contact.depth / totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * invMassA);
		contact.bodyB->position = contact.bodyB->position - (separation * invMassB);
	}
}

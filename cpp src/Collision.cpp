#include "Collision.h"
#include <algorithm>
#include <cmath>

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

			// skip pairs where neither is dynamic
			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic)
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

				// guard against zero-length direction (exact overlap)
				if (distance > 1e-6f)
				{
					contact.normal = Vector2Normalize(direction);
				}
				else
				{
					// choose an arbitrary normal when positions coincide
					contact.normal = Vector2{ 1.0f, 0.0f };
				}

				// clamp restitution into [0,1]
				contact.restitution = std::clamp(std::min(bodyA.restutuion, bodyB.restutuion), 0.0f, 1.0f);

				contacts.push_back(contact);
			}
		}
	}
}

void Contact::SeparateContacts(std::vector<Contact>& contacts)
{
	// positional correction parameters
	const float percent = 0.8f;        // positional correction percentage
	const float slop = 0.01f;          // penetration allowance

	for (auto& contact : contacts)
	{
		bool aDynamic = (contact.bodyA->bodyType == BodyType::Dynamic);
		bool bDynamic = (contact.bodyB->bodyType == BodyType::Dynamic);

		// both static -> nothing to do
		if (!aDynamic && !bDynamic) continue;

		// compute inverse masses (0 for static)
		float invMassA = (aDynamic && contact.bodyA->mass > 0.0f) ? 1.0f / contact.bodyA->mass : 0.0f;
		float invMassB = (bDynamic && contact.bodyB->mass > 0.0f) ? 1.0f / contact.bodyB->mass : 0.0f;
		float totalInverseMass = invMassA + invMassB;

		// use Baumgarte-style correction: only correct (depth - slop), scaled by percent
		float correctionDepth = std::max(contact.depth - slop, 0.0f);
		if (correctionDepth <= 0.0f) continue;

		// both dynamic: split correction by inverse mass
		if (aDynamic && bDynamic && totalInverseMass > 0.0f)
		{
			Vector2 correction = contact.normal * (percent * correctionDepth / totalInverseMass);
			contact.bodyA->position = contact.bodyA->position + (correction * invMassA);
			contact.bodyB->position = contact.bodyB->position - (correction * invMassB);
		}
		else if (aDynamic) // only A dynamic -> move A out a bit
		{
			Vector2 correction = contact.normal * (percent * correctionDepth);
			contact.bodyA->position = contact.bodyA->position + correction;
		}
		else if (bDynamic) // only B dynamic -> move B out a bit
		{
			Vector2 correction = contact.normal * (percent * correctionDepth);
			contact.bodyB->position = contact.bodyB->position - correction;
		}
	}
}

void Contact::ResolveContacts(std::vector<Contact>& contacts)
{
	const float MAX_RELATIVE_VEL = 50.0f;   // clamp approach speed used for impulse
	const float MAX_IMPULSE = 200.0f;       // cap per-contact impulse magnitude
	const float DEEP_PENETRATION_THRESHOLD = 1.0f; // reduce restitution for deep penetration

	for (auto& contact : contacts)
	{
		bool aDynamic = (contact.bodyA->bodyType == BodyType::Dynamic);
		bool bDynamic = (contact.bodyB->bodyType == BodyType::Dynamic);

		// both static -> nothing to do
		if (!aDynamic && !bDynamic) continue;

		// compute inverse masses (0 for static)
		float invMassA = (aDynamic && contact.bodyA->mass > 0.0f) ? 1.0f / contact.bodyA->mass : 0.0f;
		float invMassB = (bDynamic && contact.bodyB->mass > 0.0f) ? 1.0f / contact.bodyB->mass : 0.0f;
		float totalInverseMass = invMassA + invMassB;
		if (totalInverseMass == 0.0f) continue;

		// compute relative velocity
		Vector2 rv = contact.bodyA->velocity - contact.bodyB->velocity;
		// project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// skip if bodies are separating (positive along normal)
		if (nv > 0.0f) continue;

		// choose restitution:
		// - dynamic vs dynamic: always allow restitution
		// - when deep penetration occurs reduce restitution to avoid huge bounces
		float restitutionUsed = contact.restitution;
		if (contact.depth > DEEP_PENETRATION_THRESHOLD)
		{
			// deep penetration -> be conservative
			restitutionUsed = 0.0f;
		}

		// clamp approach velocity to avoid extreme impulses caused by numerical issues / deep penetration
		float cappedNv = std::max(nv, -MAX_RELATIVE_VEL); // nv is negative when approaching

		// impulse scalar = -(1 + restitution) * vn / (1/mA + 1/mB)
		float impulseMagnitude = -(1.0f + restitutionUsed) * cappedNv / totalInverseMass;

		// clamp impulse to a sane maximum
		if (impulseMagnitude > MAX_IMPULSE) impulseMagnitude = MAX_IMPULSE;

		// impulse vector along contact normal
		Vector2 impulse = contact.normal * impulseMagnitude;

		// apply impulses only to dynamic bodies
		if (aDynamic)
		{
			contact.bodyA->velocity = contact.bodyA->velocity + impulse * invMassA;
		}
		if (bDynamic)
		{
			contact.bodyB->velocity = contact.bodyB->velocity - impulse * invMassB;
		}

		// Ensure static bodies remain still (defensive)
		if (!aDynamic) contact.bodyA->velocity = Vector2{ 0.0f, 0.0f };
		if (!bDynamic) contact.bodyB->velocity = Vector2{ 0.0f, 0.0f };
	}
}
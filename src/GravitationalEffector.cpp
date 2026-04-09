#include "GravitationalEffector.h"
#include <cmath>
#include <algorithm>

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& bodyA = bodies[i];
            Body& bodyB = bodies[j];

            Vector2 direction = bodyB.position - bodyA.position;

            float distance = Vector2Length(direction);

            distance = fmaxf(distance, 1.0f);

            float forceMagnitude = strength * (bodyA.mass * bodyB.mass) / (distance * distance);

            Vector2 force = Vector2Normalize(direction) * forceMagnitude;

            bodyA.AddForce(force * 10);
            bodyB.AddForce(force * -10);
        }
    }
}

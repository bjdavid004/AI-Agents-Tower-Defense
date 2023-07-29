#include "FlockingBehaviour.h"
#include "Agent.h"
const float alignmentWeight = 0.5f;
const float cohesionWeight = 0.3f;
const float separationWeight = 0.8f;

glm::vec3 FlockingBehaviour::Alignment(const Agent* agent, const std::vector<Agent*>& neighbors)
{
    // Calculate the average velocity of neighboring agents
    glm::vec3 averageVelocity(0.0f);

    for (const Agent* neighbor : neighbors)
    {
        averageVelocity += neighbor->GetVelocity();
    }

    if (!neighbors.empty())
    {
        averageVelocity /= static_cast<float>(neighbors.size());
    }

    // Calculate the alignment force
    glm::vec3 alignmentForce = (averageVelocity - agent->GetVelocity()) * alignmentWeight;

    return alignmentForce;
}

glm::vec3 FlockingBehaviour::Cohesion(const Agent* agent, const std::vector<Agent*>& neighbors)
{
    // Calculate the center of mass of neighboring agents
    glm::vec3 centerOfMass(0.0f);

    for (const Agent* neighbor : neighbors)
    {
        centerOfMass += neighbor->GetPosition();
    }

    if (!neighbors.empty())
    {
        centerOfMass /= static_cast<float>(neighbors.size());
    }

    // Calculate the cohesion force
    glm::vec3 cohesionForce = (centerOfMass - agent->GetPosition()) * cohesionWeight;

    return cohesionForce;
}

glm::vec3 FlockingBehaviour::Separation(const Agent* agent, const std::vector<Agent*>& neighbors)
{
    glm::vec3 separationForce(0.0f);

    for (const Agent* neighbor : neighbors)
    {
        // Calculate the distance between the agent and its neighbor
        glm::vec3 direction = agent->GetPosition() - neighbor->GetPosition();
        float distance = glm::length(direction);

        // Apply separation force based on the distance
        if (distance > 0.0f)
        {
            separationForce += (direction / distance) * (separationWeight / distance);
        }
    }

    return separationForce;
}


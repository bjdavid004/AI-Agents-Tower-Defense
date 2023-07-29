#include "SteeringBehaviours.h"
#include "Agent.h"
glm::vec3 SteeringBehaviours::Seek(const Agent* agent, const glm::vec3& target)
{
    glm::vec3 desiredVelocity = glm::normalize(target - agent->GetPosition()) * agent->GetMaxSpeed();
    glm::vec3 steeringForce = desiredVelocity - agent->GetVelocity();
    return steeringForce;
}

glm::vec3 SteeringBehaviours::Flee(const Agent* agent, const glm::vec3& target)
{
    glm::vec3 desiredVelocity = glm::normalize(agent->GetPosition() - target) * agent->GetMaxSpeed();
    glm::vec3 steeringForce = desiredVelocity - agent->GetVelocity();
    return steeringForce;
}

glm::vec3 SteeringBehaviours::Arrival(const Agent* agent, const glm::vec3& target, float slowingRadius)
{
    glm::vec3 desiredVelocity = target - agent->GetPosition();
    float distance = glm::length(desiredVelocity);

    if (distance <= slowingRadius) {
        float speed = agent->GetMaxSpeed() * (distance / slowingRadius);
        desiredVelocity = glm::normalize(desiredVelocity) * speed;
    }
    else {
        desiredVelocity = glm::normalize(desiredVelocity) * agent->GetMaxSpeed();
    }

    glm::vec3 steeringForce = desiredVelocity - agent->GetVelocity();
    return steeringForce;
}

glm::vec3 SteeringBehaviours::ObstacleAvoidance(const Agent* agent, float maxSeeAhead, float avoidRadius, const std::vector<Agent*>& neighbors)
{
    glm::vec3 ahead = agent->GetPosition() + glm::normalize(agent->GetVelocity()) * maxSeeAhead;
    // Check for collisions with obstacles
    for (const Agent* neighbor : neighbors) {
        // Ignore the agent itself
        if (neighbor == agent)
            continue;

        float distance = glm::distance(agent->GetPosition(), neighbor->GetPosition());

        // Check if the neighbor is within the avoidance radius
        if (distance < avoidRadius) {
            glm::vec3 avoidanceForce = ahead - neighbor->GetPosition();
            avoidanceForce = glm::normalize(avoidanceForce) * agent->GetMaxAvoidForce();
            return avoidanceForce;
        }
    }

    return glm::vec3(0.0f);
}


glm::vec3 SteeringBehaviours::PathFollowing(const Agent* agent, const std::vector<glm::vec3>& path, float pathRadius)
{
    glm::vec3 currentTarget = path[agent->GetCurrentPathIndex()];

    // Check if the agent has reached the current target position
    if (glm::distance(agent->GetPosition(), currentTarget) <= pathRadius) {
        // Increment the path index to move to the next target position
        agent->IncrementCurrentPathIndex();
        // Wrap around to the beginning if the end of the path is reached
        if (agent->GetCurrentPathIndex() >= path.size()) {
            agent->SetCurrentPathIndex(0);
        }
        // Update the current target position
        currentTarget = path[agent->GetCurrentPathIndex()];
    }

    // Seek the current target position
    return Seek(agent, currentTarget);
}


#include "CompoundBehaviour.h"
#include "SteeringBehaviours.h"
#include "FlockingBehaviour.h"

glm::vec3 CompoundBehaviour::CreateCompoundBehaviour(const Agent* agent, const std::vector<Agent*>& neighbors)
{
    glm::vec3 compoundForce(0.0f);
    const float seekWeight = 1.0f;
    const float fleeWeight = 0.8f;
    const float arrivalWeight = 0.9f;
    const float avoidanceWeight = 0.7f;
    const float alignmentWeight = 0.5f;
    const float cohesionWeight = 0.6f;
    const float separationWeight = 0.8f;
    float slowingRadius = 50.0f;
    float avoidRadius = 30.0f; 
    float maxSeeAhead = 100.0f;
    glm::vec3 targetPosition(100.0f, 0.0f, 50.0f); 
    glm::vec3 enemyPosition(-50.0f, 0.0f, 0.0f);

    // Calculate individual steering forces
    glm::vec3 seekForce = SteeringBehaviours::Seek(agent, targetPosition);
    glm::vec3 fleeForce = SteeringBehaviours::Flee(agent, enemyPosition);
    glm::vec3 arrivalForce = SteeringBehaviours::Arrival(agent, targetPosition, slowingRadius);
    glm::vec3 obstacleAvoidanceForce = SteeringBehaviours::ObstacleAvoidance(agent, maxSeeAhead, avoidRadius, neighbors);

    // Apply weights to each steering force based on importance
    seekForce *= seekWeight;
    fleeForce *= fleeWeight;
    arrivalForce *= arrivalWeight;
    obstacleAvoidanceForce *= avoidanceWeight;

    // Combine the steering forces
    compoundForce += seekForce + fleeForce + arrivalForce + obstacleAvoidanceForce;

    // Apply flocking behaviors
    glm::vec3 alignmentForce = FlockingBehaviour::Alignment(agent, neighbors);
    glm::vec3 cohesionForce = FlockingBehaviour::Cohesion(agent, neighbors);
    glm::vec3 separationForce = FlockingBehaviour::Separation(agent, neighbors);

    // Apply weights to flocking forces based on importance
    alignmentForce *= alignmentWeight;
    cohesionForce *= cohesionWeight;
    separationForce *= separationWeight;

    // Combine the flocking forces
    compoundForce += alignmentForce + cohesionForce + separationForce;

    return compoundForce;
}
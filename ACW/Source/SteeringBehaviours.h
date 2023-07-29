#pragma once

#include "GameEntity.h"
#include <vector>

class Agent;

class SteeringBehaviours {
public:
    static glm::vec3 Seek(const Agent* agent, const glm::vec3& target);
    static glm::vec3 Flee(const Agent* agent, const glm::vec3& target);
    static glm::vec3 Arrival(const Agent* agent, const glm::vec3& target, float slowingRadius);
    static glm::vec3 ObstacleAvoidance(const Agent* agent, float maxSeeAhead, float avoidRadius, const std::vector<Agent*>& neighbors);
    static glm::vec3 PathFollowing(const Agent* agent, const std::vector<glm::vec3>& path, float pathRadius);
};


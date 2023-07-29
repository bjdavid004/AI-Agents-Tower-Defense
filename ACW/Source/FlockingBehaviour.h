#pragma once

#include <vector>
#include "GameEntity.h"

class Agent;

class FlockingBehaviour {
public:
    static glm::vec3 Alignment(const Agent* agent, const std::vector<Agent*>& neighbors);
    static glm::vec3 Cohesion(const Agent* agent, const std::vector<Agent*>& neighbors);
    static glm::vec3 Separation(const Agent* agent, const std::vector<Agent*>& neighbors);
};


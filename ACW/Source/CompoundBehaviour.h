#pragma once

#include <vector>
#include "GameEntity.h"

class Agent;

class CompoundBehaviour {
public:
    glm::vec3 CreateCompoundBehaviour(const Agent* agent, const std::vector<Agent*>& neighbors);
};


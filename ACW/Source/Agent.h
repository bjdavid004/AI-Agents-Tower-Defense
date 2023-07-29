#pragma once
#include <vector>
#include "GameEntity.h"
#include "IntegrationMethod.h"
#include "AgentActions.h"
#include "Pipe.h"

class Agent : public GameEntity
{
private:    
    AgentActions action;
    IntegrationMethod integrationMethod;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 direction;
    float maxSpeed = 0.003f;
    float maxAvoidForce = 0.2f;
    mutable int currentPathIndex;       
       
public:    
    int agentHealth = 5;
    mutable int captureWait = 0;
	void Update(const float pSeconds, glm::vec3 towerPosition);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
    void SetIntegrationMethod(IntegrationMethod method);
    void SetAgentActions(AgentActions action);
    AgentActions GetAgentAction() const {
        return action;
    }
    const char* GetAgentActions() const {
        if (action == AgentActions::Idel)
            return "Idle";
        else if (action == AgentActions::Moving)
            return "Moving";
        else if (action == AgentActions::Capturing)
            return "Capturing";
        else if (action == AgentActions::Searching)
            return "Searching";        
        return "Unknown";
    }
    const glm::vec3& GetVelocity() const { return velocity; }
    const glm::vec3& GetDirection() const { return direction; }
    float GetMaxSpeed() const {return maxSpeed;}
    float GetMaxAvoidForce() const { return maxAvoidForce; }
    int GetCurrentPathIndex() const { return currentPathIndex; }

    glm::vec3 GetPosition() const
    {
        glm::mat4 model = GetModel();
        glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

    void SetPosition(glm::vec3 pPosition)
    {
        glm::mat4& model = GetModel();
        model[3][0] = pPosition.x;
        model[3][1] = pPosition.y;
        model[3][2] = pPosition.z;
    }
    void SetVelocity(const glm::vec3& newVelocity);
    void SetDirection(const glm::vec3& newDirection);
    void SetCurrentPathIndex(int newIndex) const;
    void IncrementCurrentPathIndex() const;
    glm::vec3 ComputeAcceleration(const glm::vec3& position, const glm::vec3& velocity);
};
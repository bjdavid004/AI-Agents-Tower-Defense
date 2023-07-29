#include "Agent.h"
//#include "SteeringBehaviours.h"
//#include "FlockingBehaviour.h"
//#include "CompoundBehaviour.h"
#include "helpers/GuiHelpers.h"
#include "helpers/RenderHelpers.h"
//#include "helpers/IRenderHelpers.h"
//#include "helpers/IGuiHelpers.h"

//Criteria 1.1 Physical Simulation
const float GRAVITY = 0.98f;  // Acceleration due to gravity
const float mass = 2.0f;


void Agent::Update(float pSeconds, glm::vec3 towerPosition)
{
    glm::vec3 velocity = GetVelocity();
    glm::vec3 position = this->GetPosition();
    if (velocity.x == 0.0f && velocity.z == 0.0f)
    {
        velocity.x = this->GetMaxSpeed();
        velocity.z = this->GetMaxSpeed();
    }        
    switch (integrationMethod)
    {
        case IntegrationMethod::Euler:
        {
            // Euler's method
            if(position.y > 1.0f)
                velocity.y -= GRAVITY * pSeconds;     
            if (std::abs(position.x - towerPosition.x) < 1.0f)
                velocity.x = 0.0f;
            if (std::abs(position.z - towerPosition.z) < 1.0f)
                velocity.z = 0.0f;
            velocity.x = (position.x > towerPosition.x) ? -std::abs(velocity.x) : std::abs(velocity.x);
            velocity.z = (position.z > towerPosition.z) ? -std::abs(velocity.z) : std::abs(velocity.z);
           
            position = glm::vec3(position.x + velocity.x, position.y, position.z + velocity.z);
            break;
        }
        case IntegrationMethod::Verlet:
        {
            // Verlet integration
            glm::vec3 previousPosition = position;
            if (std::abs(position.x - towerPosition.x) < 1.0f)
                velocity.x = 0.0f;
            if (std::abs(position.z - towerPosition.z) < 1.0f)
                velocity.z = 0.0f;
            glm::vec3 velo = velocity * pSeconds + 4.0f * glm::vec3(0.035f, -GRAVITY, 0.035f) * pSeconds * pSeconds;
            if (previousPosition.y <= 1.0f)
            {
                position += glm::vec3(velo.x,0,velo.z);
            }
            else
                position += glm::vec3(velo.x, velo.y, velo.z);
            velocity = (position - previousPosition) / pSeconds;
            break;
        }

        case IntegrationMethod::RungeKutta:
        {
            // Runge-Kutta method
            glm::vec3 k1, k2, k3, k4;

            // Calculate k1
            glm::vec3 initialPosition = position;
            glm::vec3 initialVelocity = velocity;
            k1 = ComputeAcceleration(initialPosition, initialVelocity) / mass;

            // Calculate k2
            glm::vec3 position2 = initialPosition + k1 * (pSeconds / 2.0f);
            glm::vec3 velocity2 = initialVelocity + k1 * (pSeconds / 2.0f);
            k2 = ComputeAcceleration(position2, velocity2) / mass;

            // Calculate k3
            glm::vec3 position3 = initialPosition + k2 * (pSeconds / 2.0f);
            glm::vec3 velocity3 = initialVelocity + k2 * (pSeconds / 2.0f);
            k3 = ComputeAcceleration(position3, velocity3) / mass;

            // Calculate k4
            glm::vec3 position4 = initialPosition + k3 * pSeconds;
            glm::vec3 velocity4 = initialVelocity + k3 * pSeconds;
            k4 = ComputeAcceleration(position4, velocity4) / mass;

            // Update position and velocity using weighted average of k1, k2, k3, and k4
            position = initialPosition + (k1 + 2.0f * k2 + 2.0f * k3 + k4) * (pSeconds / 6.0f);
            velocity = initialVelocity + (k1 + 2.0f * k2 + 2.0f * k3 + k4) * (pSeconds / 6.0f);

            break;
        }
    }

    // Update position and velocity
    SetPosition(position);
    SetVelocity(velocity);
}

glm::vec3 Agent::ComputeAcceleration(const glm::vec3& position, const glm::vec3& velocity)
{
    glm::vec3 acceleration(0.0f, -GRAVITY, 0.0f);
    return acceleration;
}

void Agent::SetIntegrationMethod(IntegrationMethod method)
{
    integrationMethod = method;
}
void Agent::SetAgentActions(AgentActions newAction) {
    action = newAction;
}
void Agent::Render(const IRenderHelpers& pHelper) const
{
    pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
    pHelper.RenderGui(this);
}

//Criteria 1.1 Physical Simulation
void Agent::SetVelocity(const glm::vec3& newVelocity)
{
    velocity = newVelocity;
}
void Agent::SetDirection(const glm::vec3& newDirection) {
    direction = newDirection;
}
void Agent::IncrementCurrentPathIndex() const
{
    currentPathIndex++;
}
void Agent::SetCurrentPathIndex(int newvalue) const
{
    currentPathIndex = newvalue;
}
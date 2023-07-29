#include "Ball.h"
#include "World.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"

const float GRAVITY = 0.0098f;
const float acceleration = 0.05f;
float velocityDecay = 0.00003f;
//float velocityDecay = 0.00009f;//fast decay
Ball::Ball()
{
	glm::vec3& colour = GetColour();
	colour.x = 0.8f;
	colour.y = 0.1f;
	colour.z = 0.2f;  
	m_Radius = 0.5f;
    restitution = 0.8f;
}

void Ball::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Ball::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}
//Criteria 1.2 Movement
void Ball::SetIntegrationMethod(IntegrationMethod method)
{
    integrationMethod = method;
}
void Ball::Update(float pDeltaTime, bool collision)
{
    glm::vec3 velocity = GetVelocity();
    // Collision response with terrain
    if (HasCollidedWithTerrain())
    {
        velocity.y = -velocity.y * restitution;  // Apply restitution factor      
    }
    if (collision) {
        velocity.x = -velocity.x;
    }
    switch (integrationMethod)
    {
    case IntegrationMethod::Euler:
    {
        // Euler's method
        // Perform Euler's method calculations for ball movement
        // Criteria 1.2 Euler integration
        glm::vec3 position = GetPosition();
        if (position.y > 1.0f)
            velocity.y -= GRAVITY * pDeltaTime;
        else
            velocity.y = 0.0f;        
        float signX = glm::sign(velocity.x);
        float signZ = glm::sign(velocity.z);
        float decayX = signX * velocityDecay;
        float decayZ = signZ * velocityDecay;

        glm::vec3 pos = this->GetPosition();
        this->SetPosition(glm::vec3(pos.x+ velocity.x, pos.y + velocity.y, pos.z + velocity.z));
        SetVelocity(glm::vec3(velocity.x - decayX, velocity.y, velocity.z - decayZ));
        break;
    }
    case IntegrationMethod::Verlet:
    {
        // Verlet integration
        // Perform Verlet integration calculations for ball movement
        // Criteria 1.2 Verlet integration
        glm::vec3 position = GetPosition();
        glm::vec3 velocity = GetVelocity();
        glm::vec3 previousPosition = this->GetPosition();

        // Update position using Verlet integration
        glm::vec3 newPosition = 2.0f * position - previousPosition + acceleration * pDeltaTime * pDeltaTime;

        // Update velocity based on the new position
        velocity = (newPosition - previousPosition) / (2.0f * pDeltaTime);

        // Set the new position and velocity
        SetPosition(newPosition);
        SetVelocity(velocity);
        break;
    }
    case IntegrationMethod::RungeKutta:
    {
        // Runge-Kutta integration
        // Perform the calculations using Runge-Kutta methods
        // Criteria 1.2 Runge-Kutta integration
        glm::vec3 position = GetPosition();
        glm::vec3 velocity = GetVelocity();

        float dt = pDeltaTime;
        glm::vec3 acceleration = -0.01f * velocity;
        glm::vec3 k1v = acceleration * dt;
        glm::vec3 k1p = velocity * dt;

        glm::vec3 k2v = acceleration * dt;
        glm::vec3 k2p = (velocity + 0.5f * k1v) * dt;

        glm::vec3 k3v = acceleration * dt;
        glm::vec3 k3p = (velocity + 0.5f * k2v) * dt;

        glm::vec3 k4v = acceleration * dt;
        glm::vec3 k4p = (velocity + k3v) * dt;

        glm::vec3 newPosition = position + (k1p + 2.0f * k2p + 2.0f * k3p + k4p) / 6.0f;
        glm::vec3 newVelocity = velocity + (k1v + 2.0f * k2v + 2.0f * k3v + k4v) / 6.0f;

        SetPosition(newPosition);
        SetVelocity(newVelocity);
        break;
    }
    }
}

bool Ball::HasCollidedWithTerrain() const
{
    const float floorLevel = 0.5f;

    float ballRadius = GetRadius();
    glm::vec3 ballPosition = GetPosition();

    if (ballPosition.y - ballRadius < floorLevel)
    {
        return true;
    }
    // No collision with terrain
    return false;
}

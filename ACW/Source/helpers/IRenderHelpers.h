#pragma once

class GameEntity;
class Floor;
class Camera;
class Pipe;
class Ball;
class Agent;
class World;

class IRenderHelpers
{
private:

public:
	virtual ~IRenderHelpers() {}
	virtual void Init() = 0;
	virtual void SetSize(unsigned int pWidth, unsigned int pHeight) = 0;
	virtual void Render(const GameEntity*) const = 0;
	virtual void Render(const Camera*) const = 0;
	virtual void Render(const Floor* pFloor) const = 0;
	virtual void Render(const Pipe* pPipe) const = 0;
	virtual void Render(const Ball* pBall) const = 0;
	virtual void Render(const Agent* pAgent) const = 0;
	virtual void Render(const World* pWorld) const = 0;
};

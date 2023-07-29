#pragma once

class GameEntity;
class Pipe;
class Camera;
class Ball;
class Agent;
class World;

class IGuiHelpers
{
public:
	virtual void RenderGui(GameEntity*) const = 0;
	virtual void RenderGui(Pipe*) const = 0;
	virtual void RenderGui(Camera*) const = 0;
	virtual void RenderGui(Ball*) const = 0;
	virtual void RenderGui(Agent* pAgent) const = 0;
	virtual void RenderGui(World* pWorld) const = 0;
};
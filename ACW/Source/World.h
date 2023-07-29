#pragma once
#include "GameEntity.h"
#include "Agent.h"
#include "Ball.h"
#include "Pipe.h"
//#include "GridNode.h"

class World :
	public GameEntity
{
public:
	const static int GRID_WIDTH = 25;//X
	const static int GRID_DEPTH = 20;//z
	const float CELL_SIZE_X = 100 / GRID_WIDTH; // Calculate the size of each grid cell in the X direction
	const float CELL_SIZE_Z = 100 / GRID_DEPTH; // Calculate the size of each grid cell in the Z direction


private:
	const static int NUMBER_OF_TOWERS = 4;
	const static int NUMBER_OF_AGENTS = 4;
	const static int INITIAIL_NUMBER_OF_BALLS = 4;

	int _gridHeights[GRID_WIDTH][GRID_DEPTH];
	glm::vec3 _gridColours[GRID_WIDTH][GRID_DEPTH];
	GameEntity* _grid[GRID_WIDTH][GRID_DEPTH];  // 2D grid for pathfinding

	Agent _agents[NUMBER_OF_AGENTS];
	Pipe _towers[NUMBER_OF_TOWERS];
	Ball _balls[INITIAIL_NUMBER_OF_BALLS];
public:

	World();
	~World();

	int GetGridHeight(int pX, int pZ) const
	{
		return _gridHeights[pX][pZ];
	}

	glm::vec3 GetGridColour(int pX, int pZ) const
	{
		return _gridColours[pX][pZ];
	}
	glm::vec3 GetPosition() const
	{
		glm::mat4 model = GetModel();
		glm::vec3 translation(model[3][0], model[3][1], model[3][2]);
		return translation;
	}
	void CreateAgent(int agentNumber);
	Ball CreateCannonball(int towerNumber, Ball b);
	bool CheckCollision(Ball thisBall, const Ball*) const;
	void CheckWallCollision(Ball& ball);
	bool CheckTowerCollision(Agent& agent);
	void SphereCylinderCollision(Ball thisBall, int ballId);
	Pipe agentShortestPath(Agent agent, bool toCapture);
	Agent nearestCannonTarget(Ball b);
	void Update(float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	void ChangeIntegrationMethod(IntegrationMethod IntegrationMethod);
	glm::vec3 CalculatePathToNearestTowerDijkstra(Agent agent, const glm::vec3& targetPos);
	//Criteria 1.5 Spatial Segmentation
	void InitializeGrid();
	void UpdateGrid();
};


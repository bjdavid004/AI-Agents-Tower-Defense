#include "World.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include "IntegrationMethod.h"
#include "EntityType.h"
#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <unordered_set>
#include <cmath>
#include "GridNode.h"
int activeCanonballs_count = 0;
static int NUMBER_OF_BALLS = 4;
int visited[4] = { 0, 0, 0, 0 };

World::World()
{	
	glm::vec3 postion_ball(0, 0, 0);
	glm::vec3 neutral_tower_color;	
	//_towers
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		glm::vec3 position;
		if (i == 0) // Top-left corner
			position = glm::vec3(0, 3.2f, 0);
		else if (i == 1) // Top-right corner
			position = glm::vec3(GRID_WIDTH - 1, 3.2f, 0);
		else if (i == 2) // Bottom-left corner
			position = glm::vec3(0, 3.2f, GRID_DEPTH - 1);
		else if (i == 3) // Bottom-right corner
			position = glm::vec3(GRID_WIDTH - 1, 3.2f, GRID_DEPTH - 1);		
		_towers[i].Translate(position);
		_towers[i].SetType(EntityType::Tower);
	}
	neutral_tower_color = _towers[0].GetColour();
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{		
		float x = rand() % (GRID_WIDTH - 3) - 4;
		float z = rand() % (GRID_DEPTH - 3) - 3;
		_agents[i].SetIntegrationMethod(IntegrationMethod::Euler);
		_agents[i].SetAgentActions(AgentActions::Moving);
		_agents[i].SetPosition(glm::vec3(x, 0.0f, z));
		_agents[i].SetVelocity(glm::vec3(0.0038f, 0.0f, 0.0038f));
		_agents[i].SetDirection(agentShortestPath(_agents[i], true).GetPosition());
		_agents[i].SetType(EntityType::Agent);
		if (i % 2 == 0)
		{
			_agents[i].GetColour() = glm::vec3(1, 0, 0);
		}
		else
		{
			_agents[i].GetColour() = glm::vec3(0, 0, 1);
		}
	}
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
			Ball b;
			b = CreateCannonball(i, b);
			_balls[activeCanonballs_count++] = b;			
	}
	InitializeGrid();
}

World::~World()
{

}

void World::InitializeGrid()
{
	for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int z = 0; z < GRID_DEPTH; z++)
		{
			_grid[x][z] = nullptr;
			_gridHeights[x][z] = 0;
		}
	}
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		_gridHeights[i][0] = 3;
		_gridHeights[i][GRID_DEPTH - 1] = 3;
	}

	for (int i = 0; i < GRID_DEPTH; i++)
	{
		_gridHeights[0][i] = 3;
		_gridHeights[GRID_WIDTH - 1][i] = 3;
	}
	glm::vec3 colours[3] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			_gridColours[i][j] = colours[rand() % 3];
		}
	}
}
void World::ChangeIntegrationMethod(IntegrationMethod newMethod) {
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		_agents[i].SetIntegrationMethod(newMethod);
	}
	for (int i = 0; i < activeCanonballs_count; i++)
	{
		_balls[i].SetIntegrationMethod(newMethod);
	}
}
void World::UpdateGrid()
{
	for (int x = 0; x < GRID_WIDTH; x++)
	{
		for (int z = 0; z < GRID_DEPTH; z++)
		{
			_grid[x][z] = nullptr;
		}
	}
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		Agent& agent = _agents[i];
		int x = static_cast<int>(agent.GetPosition().x / CELL_SIZE_X);
		int z = static_cast<int>(agent.GetPosition().z / CELL_SIZE_Z);

		if (x >= 0 && x < GRID_WIDTH && z >= 0 && z < GRID_DEPTH)
		{
			_grid[x][z] = &agent;
		}
	}
}

void World::Update(float pSeconds)
{
	UpdateGrid();
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{		
		glm::vec3 direction = CalculatePathToNearestTowerDijkstra(_agents[i], _agents[i].GetDirection());
		if (_agents[i].GetAgentAction() != AgentActions::Idel && _agents[i].GetAgentAction() != AgentActions::Searching)
		{
			if (!CheckTowerCollision(_agents[i]))
			{
				_agents[i].SetAgentActions(AgentActions::Moving);
				_agents[i].Update(pSeconds, direction);
			}
			else
			{
				_agents[i].SetAgentActions(AgentActions::Capturing);
				if (_agents[i].captureWait++ > 400)
				{
					_agents[i].captureWait = 0;
					for (int j = 0; j < NUMBER_OF_TOWERS; j++)
					{
						if (glm::distance(_agents[i].GetPosition().x, _towers[j].GetPosition().x) <= 1.5f && glm::distance(_agents[i].GetPosition().z, _towers[j].GetPosition().z) <= 1.5f)
						{
							glm::vec3 position = _towers[j].GetPosition();
							int x = static_cast<int>(position.x);
							int z = static_cast<int>(position.z);
							if (i % 2 == 0)
							{
								_towers[j].GetColour() = glm::vec3(1, 0, 0);
								_gridColours[x][z] = glm::vec3(1, 0, 0);
								visited[j] = 1;
							}
							else
							{
								_towers[j].GetColour() = glm::vec3(0, 0, 1);
								_gridColours[x][z] = glm::vec3(0, 0, 1);
								visited[j] = 2;
							}
							_agents[i].Update(0.03f, _agents[i].GetDirection());
							_agents[i].SetAgentActions(AgentActions::Idel);
						}
					}
					_agents[i].SetDirection(_towers[(std::rand() % 4)].GetPosition());
				}
				else {
					for (int j = 0; j < NUMBER_OF_TOWERS; j++)
					{						
						if (glm::distance(_agents[i].GetPosition().x, _towers[j].GetPosition().x) <= 1.5f && glm::distance(_agents[i].GetPosition().z, _towers[j].GetPosition().z) <= 1.5f)
						{
							if (i % 2 == 0)
							{
								if (_towers[j].GetColour() == glm::vec3(1, 0, 0))
									_agents[i].SetAgentActions(AgentActions::Idel);
							}
							else
							{
								if (_towers[j].GetColour() == glm::vec3(0, 0, 1))
									_agents[i].SetAgentActions(AgentActions::Idel);
							}
						}
					}
				}
			}
		}
		if(_agents[i].GetAgentAction() == AgentActions::Idel) {
			std::vector<int> filteredPositions;
			for (int i = 0; i < NUMBER_OF_TOWERS; i++) {
				if (visited[i] == 0) {
					filteredPositions.push_back(i);
				}
				if (i % 2 == 0 && visited[i] == 2) {
					filteredPositions.push_back(i);
				}
				if (i % 2 != 0 && visited[i] == 1) {
					filteredPositions.push_back(i);
				}
			}			
			int randomIndex = std::rand() % filteredPositions.size();
			int randomPosition = filteredPositions[randomIndex];
			_agents[i].SetDirection(_towers[randomPosition].GetPosition());
			_agents[i].Update(pSeconds, _agents[i].GetDirection());
			_agents[i].SetAgentActions(AgentActions::Searching);
		}
		else if (_agents[i].GetAgentAction() == AgentActions::Searching)
		{
			_agents[i].Update(pSeconds, _agents[i].GetDirection());
			_agents[i].SetAgentActions(AgentActions::Moving);
		}
	}
	
	int number_of_cannonballs = activeCanonballs_count;
	for (int i = 0; i < number_of_cannonballs; i++)
	{		
		CheckWallCollision(_balls[i]);
		SphereCylinderCollision(_balls[i], i);
		_balls[i].Update(pSeconds, CheckCollision(_balls[i], _balls));
		if (std::abs(_balls[i].GetVelocity().x) < 0.0003f && std::abs(_balls[i].GetVelocity().z) < 0.0003f) {
			_balls[i] = Ball();
			activeCanonballs_count--;
		}
		if (activeCanonballs_count < 4)
		{
			Ball b;
			b = CreateCannonball(i, b);
			_balls[i] = b;
			activeCanonballs_count++;
		}
	}
}
Ball World::CreateCannonball(int towerNumber,Ball b) {	
	float x = rand() % (GRID_WIDTH - 3) - 2;
	float z = rand() % (GRID_DEPTH - 3) - 2;
	b.SetPosition(glm::vec3(_towers[towerNumber].GetPosition().x, _towers[towerNumber].GetPosition().y + 2.0f, _towers[towerNumber].GetPosition().z));
	Agent a = nearestCannonTarget(b);
	glm::vec3 direction = glm::normalize(a.GetPosition() - b.GetPosition());

	b.SetVelocity(direction * 0.04f);
	b.SetIntegrationMethod(IntegrationMethod::Euler);
	b.GetColour() = _towers[towerNumber].GetColour();
	b.SetType(EntityType::Ball);
	return b;
}
// Find the nearest pipe tower
Pipe World::agentShortestPath(Agent agent, bool toCapture) {
	glm::vec3 agentPosition = agent.GetPosition();	
	Pipe nearestTower;
	float minDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		glm::vec3 tower_pos = _towers[i].GetPosition();
		float distance = sqrt(pow(agentPosition.x - tower_pos.x, 2) + pow(agentPosition.z - tower_pos.z, 2));
		if (distance < 2.0f)
			continue;

		if (distance < minDistance) {
			minDistance = distance;
			nearestTower = _towers[i];
		}
	}
	return nearestTower;
}
// Find the nearest agent
Agent World::nearestCannonTarget(Ball b) {	
	Agent nearestTarget;
	nearestTarget.SetPosition(glm::vec3(GRID_WIDTH / 2.0f, 0.0f, GRID_DEPTH / 2.0f));
	float minDistance = glm::distance(b.GetPosition(), nearestTarget.GetPosition());

	for (int i = 0; i < NUMBER_OF_AGENTS; i++) {
		float distance = glm::distance(b.GetPosition(), _agents[i].GetPosition());

		if (_agents[i].GetColour() != b.GetColour() && distance > 8.0f) {
			if (distance < minDistance) {
				minDistance = distance;
				nearestTarget = _agents[i];
			}
		}
	}
	return nearestTarget;
}
void World::SphereCylinderCollision(Ball thisBall, int ballId) {
	glm::vec3 m_Position = thisBall.GetPosition() +(thisBall.GetVelocity());
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		if (thisBall.GetColour() == _agents[i].GetColour())
			continue;
		float _distance = glm::distance(_agents[i].GetPosition(), thisBall.GetPosition());
		if (_distance < 1.1f)
		{
			_agents[i].agentHealth--;
			if (_agents[i].agentHealth <= 0)
			{
				_agents[i] = Agent();
				CreateAgent(i);
				Ball b;
				thisBall = CreateCannonball(ballId, b);
				_balls[ballId] = b;
				return;
			}								
			if (i % 2 == 0)
			{
				_agents[i].GetColour() = glm::vec3(_agents[i].GetColour().x - 0.15, 0, 0);
			}
			else
			{
				_agents[i].GetColour() = glm::vec3(0, 0, _agents[i].GetColour().z - 0.15);
			}
			Ball b;
			thisBall = CreateCannonball(ballId, b);
			_balls[ballId] = b;
		}
	}
}
void World::CreateAgent(int agentNumber) {
	_agents[agentNumber].SetIntegrationMethod(IntegrationMethod::Euler);
	_agents[agentNumber].SetAgentActions(AgentActions::Moving);
	if (agentNumber % 2 == 0)
	{
		_agents[agentNumber].GetColour() = glm::vec3(1, 0, 0);
	}
	else
	{
		_agents[agentNumber].GetColour() = glm::vec3(0, 0, 1);
	}
	float x = rand() % (GRID_WIDTH - 2);
	float z = rand() % (GRID_DEPTH - 2);
	if (x < 1)
		x += 3;
	else if (x > GRID_WIDTH)
		x -= 2;
	if (z < 1)
		z += 2;
	else if (z > GRID_DEPTH)
		z -= 2;
	_agents[agentNumber].SetPosition(glm::vec3(x, 0.0f, z));
	_agents[agentNumber].SetVelocity(glm::vec3(0.003f, 0.0f, 0.003f));	
	_agents[agentNumber].SetDirection(agentShortestPath(_agents[agentNumber], true).GetPosition());
	_agents[agentNumber].Update(0.03f, _agents[agentNumber].GetDirection());
}
bool World::CheckCollision(Ball thisBall, const Ball*) const
{
	glm::vec3 position = thisBall.GetPosition();
	float radius = thisBall.GetRadius();
	for (const Ball& otherBall : _balls)
	{
		if (thisBall.GetPosition() != otherBall.GetPosition())
		{
			glm::vec3 otherPosition = otherBall.GetPosition();
			float otherRadius = otherBall.GetRadius();
			float distance = glm::distance(position, otherPosition);

			if (distance < radius + otherRadius)
			{
				// Collision detected
				return true;
			}
		}
	}
	return false;
}
void World::CheckWallCollision(Ball& ball)
{
	glm::vec3 position = ball.GetPosition();
	float radius = ball.GetRadius();
	if (position.x - radius < 0.0f)
	{
		// Collision with left wall
		ball.SetPosition(glm::vec3(radius, position.y, position.z));
		ball.SetVelocity(glm::vec3(-ball.GetVelocity().x, ball.GetVelocity().y, ball.GetVelocity().z));
	}
	else if (position.x + radius > GRID_WIDTH - 1)
	{
		// Collision with right wall
		ball.SetPosition(glm::vec3(GRID_WIDTH - 1 - radius, position.y, position.z));
		ball.SetVelocity(glm::vec3(-ball.GetVelocity().x, ball.GetVelocity().y, ball.GetVelocity().z));
	}

	if (position.z - radius < 0.0f)
	{
		// Collision with bottom wall
		ball.SetPosition(glm::vec3(position.x, position.y, radius));
		ball.SetVelocity(glm::vec3(ball.GetVelocity().x, ball.GetVelocity().y, -ball.GetVelocity().z));
	}
	else if (position.z + radius > GRID_DEPTH - 1)
	{
		// Collision with top wall
		ball.SetPosition(glm::vec3(position.x, position.y, GRID_DEPTH - 1 - radius));
		ball.SetVelocity(glm::vec3(ball.GetVelocity().x, ball.GetVelocity().y, -ball.GetVelocity().z));
	}
}
bool World::CheckTowerCollision(Agent& agent)
{
	glm::vec3 position = agent.GetPosition();
	int istower = 0;//if 2 means tower
	if (position.x - 1.0f < 0.1f)
	{
		// Collision with left wall
		agent.SetPosition(glm::vec3(position.x, position.y, position.z + 0.008f));
		istower++;
	}
	else if (position.x + 1.0f > GRID_WIDTH - 1)
	{
		// Collision with right wall
		agent.SetPosition(glm::vec3(GRID_WIDTH - 1 - 1.0f, position.y, position.z));
		istower++;
	}

	if (position.z - 1.0f < 1.0f)
	{
		// Collision with bottom wall
		agent.SetPosition(glm::vec3(position.x, position.y, 1.0f));
		istower++;
	}
	else if (position.z + 1.0f > GRID_DEPTH - 1)
	{
		// Collision with top wall
		agent.SetPosition(glm::vec3(position.x, position.y, GRID_DEPTH - 1 - 1.0f));
		istower++;
	}
	Pipe nearestTower = agentShortestPath(agent, false);
	if (sqrt(pow(agent.GetPosition().x - nearestTower.GetPosition().x, 2) + pow(agent.GetPosition().z - nearestTower.GetPosition().z, 2)) < 1.5f)
	{
		istower = 0;		
	}
	if (istower == 2)
		return true;
	else
		return false;
}
void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.Render(&_agents[i]);
	}
	for (int i = 0; i < NUMBER_OF_BALLS; i++)
	{
		pHelper.Render(&_balls[i]);
	}
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		pHelper.Render(&_towers[i]);
	}
}
void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.RenderGui(&_agents[i]);
	}
	for (int i = 0; i < NUMBER_OF_BALLS; i++)
	{
		pHelper.RenderGui(&_balls[i]);
	}
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		pHelper.RenderGui(&_towers[i]);
	}
}
glm::vec3 World::CalculatePathToNearestTowerDijkstra(Agent agent, const glm::vec3& targetPos)
{
	glm::vec3 pos = agent.GetPosition();
	int startX = static_cast<int>(pos.x / CELL_SIZE_X);
	int startZ = static_cast<int>(pos.z / CELL_SIZE_Z);
	int targetX = static_cast<int>(targetPos.x / CELL_SIZE_X);
	int targetZ = static_cast<int>(targetPos.z / CELL_SIZE_Z);

	if (targetX < 0 || targetX >= GRID_WIDTH || targetZ < 0 || targetZ >= GRID_DEPTH)
	{
		return targetPos;
	}

	std::priority_queue<GridNode*, std::vector<GridNode*>, DijkstraComparator> openSet;
	std::unordered_set<GridNode*> visitedSet;

	GridNode* startNode = new GridNode(startX, startZ, 0, 0, nullptr);
	openSet.push(startNode);

	while (!openSet.empty())
	{
		GridNode* currentNode = openSet.top();
		openSet.pop();

		if (currentNode->x == targetX && currentNode->z == targetZ)
		{
			glm::vec3 targetPosition(currentNode->x * CELL_SIZE_X, 0.0f, currentNode->z * CELL_SIZE_Z);
			return targetPosition;
		}

		visitedSet.insert(currentNode);

		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dz = -1; dz <= 1; dz++)
			{
				if ((dx == 0 && dz == 0) || (std::abs(dx) == 1 && std::abs(dz) == 1))
				{
					break;
				}
				int neighborX = currentNode->x + dx;
				int neighborZ = currentNode->z + dz;

				if (neighborX < 0 || neighborX >= GRID_WIDTH || neighborZ < 0 || neighborZ >= GRID_DEPTH)
				{
					break;
				}

				if (_grid[neighborX][neighborZ] != nullptr)
				{
					GameEntity* neighborNode = _grid[neighborX][neighborZ];
					bool isVisited = false;
					for (GridNode* visitedNode : visitedSet)
					{
						if (agent.GetColour() != neighborNode->GetColour())
						{
							isVisited = true;
							break;
						}
					}
					if (isVisited)
					{
						continue;
					}
				}
				int distance = currentNode->distance + 1;
				GridNode* neighborNode = new GridNode(neighborX, neighborZ, distance, 0, currentNode);
				openSet.push(neighborNode);
			}
		}
	}

	return targetPos;
}

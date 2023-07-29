#include <vector>
#include <unordered_set>
#include "GameEntity.h"

class SpatialSegmentation
{
private:
    int gridSizeX;
    int gridSizeY;
    float cellSize;
    std::vector<std::vector<std::unordered_set<int>>> grid;

public:
    SpatialSegmentation(int sizeX, int sizeY, float cell);
    void AddObject(int objectID, const glm::vec3& position);
    std::unordered_set<int> GetPotentialCollisions(const glm::vec3& position);
};

SpatialSegmentation::SpatialSegmentation(int sizeX, int sizeY, float cell)
{
    gridSizeX = sizeX;
    gridSizeY = sizeY;
    cellSize = cell;

    grid.resize(gridSizeX);
    for (int i = 0; i < gridSizeX; i++)
    {
        grid[i].resize(gridSizeY);
    }
}

void SpatialSegmentation::AddObject(int objectID, const glm::vec3& position)
{
    int gridX = static_cast<int>(position.x / cellSize);
    int gridY = static_cast<int>(position.z / cellSize);

    if (gridX >= 0 && gridX < gridSizeX && gridY >= 0 && gridY < gridSizeY)
    {
        grid[gridX][gridY].insert(objectID);
    }
}

std::unordered_set<int> SpatialSegmentation::GetPotentialCollisions(const glm::vec3& position)
{
    int gridX = static_cast<int>(position.x / cellSize);
    int gridY = static_cast<int>(position.z / cellSize);

    std::unordered_set<int> potentialCollisions;

    if (gridX >= 0 && gridX < gridSizeX && gridY >= 0 && gridY < gridSizeY)
    {
        potentialCollisions = grid[gridX][gridY];
    }

    // Add objects from adjacent cells
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int adjacentX = gridX + i;
            int adjacentY = gridY + j;

            if (adjacentX >= 0 && adjacentX < gridSizeX && adjacentY >= 0 && adjacentY < gridSizeY)
            {
                const auto& objects = grid[adjacentX][adjacentY];
                potentialCollisions.insert(objects.begin(), objects.end());
            }
        }
    }

    return potentialCollisions;
}

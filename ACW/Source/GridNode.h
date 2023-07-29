#include <limits>
class GridNode
{
public:
    int x;               // X coordinate of the node
    int z;               // Z coordinate of the node
    int gCost;           // Cost from the start node
    int hCost;           // Heuristic cost to the target node
    int distance;        // Distance from the start node (used in Dijkstra's Algorithm)
    GridNode* parent;    // Parent node

    GridNode(int _x, int _z, int _gCost, int _hCost, GridNode* _parent)
        : x(_x), z(_z), gCost(_gCost), hCost(_hCost), distance(INT_MAX), parent(_parent) {}
};

struct DijkstraComparator
{
    bool operator()(const GridNode* node1, const GridNode* node2) const
    {
        return node1->distance > node2->distance;
    }
};

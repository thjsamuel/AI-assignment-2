#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Grid.h"
#include <vector>

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	void Update(const Vector3& startPos, const Vector3& targetPos);

	void FindPath(const Vector3& startPos, const Vector3& targetPos);

	CGrid* GetGrid();

private:
	CGrid* m_grid;
	std::vector<CNode*> openList;
	std::vector<CNode*> closedList;

	int GetDistance(CNode* nodeA, CNode* nodeB);
	void RetracePath(CNode* startNode, CNode* endNode);
	bool Contains(std::vector<CNode*> list, CNode* node);
};

#endif //PATHFINDING_H
#include "Pathfinding.h"
#include "Grid.h"

Pathfinding::Pathfinding()
{
	m_grid = new CGrid();
}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::Update(const Vector3& startPos, const Vector3& targetPos)
{
	FindPath(startPos, targetPos);
}

void Pathfinding::FindPath(const Vector3& startPos, const Vector3& targetPos)
{
	CNode* startNode = m_grid->GetNodeFromWorldPoint(startPos);
	CNode* targetNode = m_grid->GetNodeFromWorldPoint(targetPos);

	openList.push_back(startNode);

	while (openList.size() > 0)
	{
		CNode* currentNode = openList.front();

		int i;
		for (i = 1; i < openList.size(); i++)
		{
			if (openList[i]->GetFCost() < currentNode->GetFCost() 
				|| openList[i]->GetFCost() == currentNode->GetFCost() 
				&& openList[i]->GetHCost() < currentNode->GetHCost())
			{
				currentNode = openList[i];
			}
		}

		openList.erase((openList.begin() + i -1));
		closedList.push_back(currentNode);

		if (currentNode == targetNode)
		{
			RetracePath(startNode, targetNode);
			return;
		}

		for each (CNode* neighbour in m_grid->GetNeighbours(currentNode))
		{
			if (neighbour->GetWalkable() == false || Contains(closedList, neighbour) == true)
				continue;

			int newMovementCostToNeighbour = currentNode->GetGCost() + GetDistance(currentNode, neighbour);
			if (newMovementCostToNeighbour < neighbour->GetGCost() || Contains(openList, neighbour) == false)
			{
				neighbour->SetGCost(newMovementCostToNeighbour);
				neighbour->SetHCost(GetDistance(neighbour, targetNode));
				neighbour->SetParent(currentNode);

				if (Contains(openList, neighbour) == false)
				{
					openList.push_back(neighbour);
				}
			}
		}
	}
}

CGrid* Pathfinding::GetGrid()
{
	return m_grid;
}

int Pathfinding::GetDistance(CNode* nodeA, CNode* nodeB)
{
	int distX = Math::FAbs(nodeA->GetGridX() - nodeB->GetGridX());
	int distY = Math::FAbs(nodeA->GetGridY() - nodeB->GetGridY());

	if (distX > distY)
		return 14 * distY + 10 * (distX - distY);

	return 14 * distX + 10 * (distY - distX);
}

void Pathfinding::RetracePath(CNode* startNode, CNode* endNode)
{
	std::vector<CNode*> path;
	CNode* currentNode = endNode;

	while (currentNode != startNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->GetParent();
	}

	std::reverse(std::begin(path), std::end(path));

	m_grid->SetPath(path);
}

bool Pathfinding::Contains(std::vector<CNode*> list, CNode* node)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == node)
			return true;
	}

	return false;
}
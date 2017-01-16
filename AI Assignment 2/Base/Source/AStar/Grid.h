#ifndef GRID_H
#define GRID_H

#include "Node.h"
#include <vector>

class CGrid
{
public:
	CGrid();
	~CGrid();

	void CreateGrid();
	std::vector<CNode*> GetNeighbours(CNode* node);
	CNode* GetNodeFromWorldPoint(const Vector3& position);
	CNode** Get();

	bool Contains(std::vector<CNode*> _list, CNode* node);

	void SetPath(std::vector<CNode*> _path);
	std::vector<CNode*> GetPath();

	int GetGridSizeX();
	int GetGridSizeY();

private:
	CNode** m_grid;
	std::vector<CNode*> m_path;

	Vector3 gridSize;
	float nodeRadius;
	float nodeDiameter;
	int gridSizeX, gridSizeY;
};

#endif // GRID_H
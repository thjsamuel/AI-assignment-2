#ifndef GRID_H
#define GRID_H

#include "Node.h"

class Grid
{
public:
	Grid();
	~Grid();

	void CreateGrid();
	CNode* GetCurrentNode(Vector3 position);

	CNode** Get();

	int GetGridSizeX();
	int GetGridSizeY();

private:
	CNode** grid;
	Vector3 gridSize;
	float nodeRadius;
	float nodeDiameter;
	int gridSizeX, gridSizeY;
};

#endif // GRID_H
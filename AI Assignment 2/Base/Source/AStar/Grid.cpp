#include "Grid.h"
#include "../Application.h"

Grid::Grid()
{
	//Calculating aspect ratio
	float m_worldHeight = 100.f;
	float m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// AStar
	gridSize = Vector3(m_worldWidth, m_worldHeight, 0);

	nodeRadius = 3.f;
	nodeDiameter = nodeRadius * 2;
	gridSizeX = (int)(gridSize.x / nodeDiameter);
	gridSizeY = (int)(gridSize.y / nodeDiameter);
}

Grid::~Grid()
{
}

void Grid::CreateGrid()
{
	grid = new CNode*[gridSizeX];
	for (int i = 0; i < gridSizeX; i++)
		grid[i] = new CNode[gridSizeY];

	Vector3 worldBottomLeft = Vector3(0, 0, 0) - Vector3(1, 0, 0) * (gridSize.x / 2) - Vector3(0, 1, 0) * (gridSize.y / 2);

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			Vector3 position = Vector3(1, 0, 0) * (x * nodeDiameter + nodeRadius) + Vector3(0, 1, 0) * (y * nodeDiameter + nodeRadius);
			grid[x][y] = CNode(true, Vector3(position.x, position.y, 0));
		}
	}
}

CNode* Grid::GetCurrentNode(Vector3 position)
{
	float percentX = (position.x) / gridSize.x;
	float percentY = (position.y) / gridSize.y;

	int x = (int)((gridSizeX)* percentX);
	int y = (int)((gridSizeY)* percentY);

	return &grid[x][y];
}

CNode** Grid::Get()
{
	return grid;
}

int Grid::GetGridSizeX()
{
	return gridSizeX;
}

int Grid::GetGridSizeY()
{
	return gridSizeY;
}
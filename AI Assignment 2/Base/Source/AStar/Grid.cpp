#include "Grid.h"
#include "../Application.h"

CGrid::CGrid()
{
	//Calculating aspect ratio
	float m_worldHeight = 100.f;
	float m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// AStar
	gridSize = Vector3(m_worldWidth, m_worldHeight, 0);

	nodeRadius = 2.8f;
	nodeDiameter = nodeRadius * 2;
	gridSizeX = (int)(gridSize.x / nodeDiameter);
	gridSizeY = (int)(gridSize.y / nodeDiameter);
}

CGrid::~CGrid()
{
}

void CGrid::CreateGrid()
{
	m_grid = new CNode*[gridSizeX];
	for (int i = 0; i < gridSizeX; i++)
		m_grid[i] = new CNode[gridSizeY];

	Vector3 worldBottomLeft = Vector3(0, 0, 0) - Vector3(1, 0, 0) * (gridSize.x / 2) - Vector3(0, 1, 0) * (gridSize.y / 2);

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			Vector3 position = Vector3(1, 0, 0) * (x * nodeDiameter + nodeRadius) + Vector3(0, 1, 0) * (y * nodeDiameter + nodeRadius);
			m_grid[x][y] = CNode(true, Vector3(position.x, position.y, 0), x, y);
		}
	}
}

std::vector<CNode*> CGrid::GetNeighbours(CNode* node)
{
	std::vector<CNode*> neighbours;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
				continue;

			int checkX = node->GetGridX() + x;
			int checkY = node->GetGridY() + y;

			if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY)
			{
				neighbours.push_back(&m_grid[checkX][checkY]);
			}
		}
	}

	return neighbours;
}

CNode* CGrid::GetNodeFromWorldPoint(const Vector3& position)
{
	float percentX = (position.x) / gridSize.x;
	float percentY = (position.y) / gridSize.y;

	int x = (int)((gridSizeX)* percentX);
	int y = (int)((gridSizeY)* percentY);

	return &m_grid[x][y];
}

CNode** CGrid::Get()
{
	return m_grid;
}

bool CGrid::Contains(std::vector<CNode*> list, CNode* node)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == node)
			return true;
	}

	return false;
}

void CGrid::SetPath(std::vector<CNode*> _path)
{
	this->m_path = _path;
}

std::vector<CNode*> CGrid::GetPath()
{
	return m_path;
}

int CGrid::GetGridSizeX()
{
	return gridSizeX;
}

int CGrid::GetGridSizeY()
{
	return gridSizeY;
}
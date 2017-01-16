#include "Node.h"

CNode::CNode()
	: position(0, 0, 0)
	, bWalkable(true)
	, parent(NULL)
	, gCost(0)
	, hCost(0)
	, fCost(0)
	, gridX(0)
	, gridY(0)
{

}

CNode::CNode(bool bWalkable, Vector3 position, int _gridX, int _gridY)
	: position(position)
	, bWalkable(bWalkable)
	, parent(NULL)
	, gCost(0)
	, hCost(0)
	, fCost(0)
	, gridX(_gridX)
	, gridY(_gridY)
{
}

CNode::~CNode()
{
}

void CNode::SetParent(CNode* _parent)
{
	this->parent = _parent;
}

CNode* CNode::GetParent()
{
	return parent;
}

void CNode::SetGCost(int _gCost)
{
	this->gCost = _gCost;
}

int CNode::GetGCost()
{
	return gCost;
}

void CNode::SetHCost(int _hCost)
{
	this->hCost = _hCost;
}

int CNode::GetHCost()
{
	return hCost;
}

int CNode::GetFCost()
{
	fCost = gCost + hCost;

	return fCost;
}

int CNode::GetGridX()
{
	return gridX;
}

int CNode::GetGridY()
{
	return gridY;
}

void CNode::SetWalkable(bool bWalkable)
{
	this->bWalkable = bWalkable;
}

bool CNode::GetWalkable()
{
	return bWalkable;
}

Vector3 CNode::GetPosition()
{
	return position;
}

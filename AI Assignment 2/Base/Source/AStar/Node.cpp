#include "Node.h"

CNode::CNode()
	: position(0, 0, 0)
	, bWalkable(true)
{

}

CNode::CNode(bool bWalkable, Vector3 position)
	: position(position)
	, bWalkable(bWalkable)
{
}

CNode::~CNode()
{
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

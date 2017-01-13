#ifndef NODE_H
#define NODE_H

#include "Vector3.h"

class CNode
{
public:
	CNode();
	CNode(bool bWalkable, Vector3 position);
	~CNode();

	void SetWalkable(bool bWalkable);
	bool GetWalkable();

	Vector3 GetPosition();

private:
	bool bWalkable;
	Vector3 position;
};

#endif // NODE_H
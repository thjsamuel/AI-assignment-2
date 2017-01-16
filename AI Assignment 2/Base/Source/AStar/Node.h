#ifndef NODE_H
#define NODE_H

#include "Vector3.h"

class CNode
{
public:
	CNode();
	CNode(bool _bWalkable, Vector3 _position, int _gridX, int _gridY);
	~CNode();

	void SetParent(CNode* _parent);
	CNode* GetParent();

	void SetGCost(int _gCost);
	int GetGCost();
	
	void SetHCost(int _hCost);
	int GetHCost();

	int GetFCost();

	int GetGridX();
	int GetGridY();

	void SetWalkable(bool bWalkable);
	bool GetWalkable();

	Vector3 GetPosition();

private:
	bool bWalkable;
	Vector3 position;

	CNode* parent;

	int gCost;
	int hCost;
	int fCost;

	int gridX;
	int gridY;
};

#endif // NODE_H
#ifndef FURNITURE_H
#define FURNITURE_H

#include "Vector3.h"

struct Furniture
{
	Furniture(const Vector3& position, const Vector3& scale)
	{
		this->position = position;
		this->scale = scale;
	};

	~Furniture() {};

	Vector3 position;
	Vector3 scale;
};

#endif // FURNITURE_H
#include "EntityNames.h"

std::string GetEntityNameFromID(int ID)
{
	switch (ID)
	{
	case 0:
		return "Customer";
		break;
	case 1:
		return "Waiter";
		break;
	case 2:
		return "Chef";
		break;
	case 3:
		return "Cleaner";
		break;

	default:
		return "Unknown Entity";
	}
}
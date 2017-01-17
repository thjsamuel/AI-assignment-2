#ifndef ENTITY_NAMES_H
#define ENTITY_NAMES_H

#include <string>

enum Entities
{
	ENT_WAITER = 0,
	ENT_WAITER_OUTSIDE,
	ENT_CHEF,
	ENT_CLEANER,
	//ENT_CUSTOMER, // since many customers will be generated, using entityMap's size to set every customer's id
};

std::string GetEntityNameFromID(int ID);



#endif // ENTITY_NAMES_H
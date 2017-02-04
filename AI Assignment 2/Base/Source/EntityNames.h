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

// Indexing for every flock of customers in the scene
enum Group
{
    GROUP_1,
    GROUP_2,
    GROUP_3,
    GROUP_4,
    GROUP_5,
    GROUP_6,
    GROUP_MAX,
};

std::string GetEntityNameFromID(int ID);



#endif // ENTITY_NAMES_H
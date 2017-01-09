#ifndef S_STATE_H
#define S_STATE_H

#include <String>
using std::string;

class State
{
public:
	State();
	~State();
private:
	string name;
	int ID;
};

#endif
#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include "MyVector.h"
using std::string;

#include <map>
using std::map;

class Character
{
public:
    Character() : position(MyVector(0, 0))/*, currState(IDLE)*/  {
        for (int i = 0; i < waypoints.size(); ++i)
        {
            waypoints[i].SetPosition(0, 0);
        }
    }
    virtual ~Character() = 0;
    virtual enum States
    {
    };
    virtual void init() = 0;
    MyVector getPos() { return position; }
    void setPos(MyVector newVec) { position = newVec; }
    map<const int, MyVector> waypoints;
    bool* reachDes;
protected:
    //Character::States currState;
    MyVector position;
};

#endif
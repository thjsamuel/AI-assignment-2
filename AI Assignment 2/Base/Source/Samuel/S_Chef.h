#ifndef S_CHEF_H
#define S_CHEF_H

#include "..\Samuel\Character.h"

class Chef : public Character
{
public:
    Chef();
    ~Chef();
    enum ChefStates
    {
        IDLE,
        WALK,
        COOK,
        WAIT,
        MAX_BASE,
    };

    virtual void init();
    void update(double dt); // reads in other FSM states and updates own FSM
    void runFSM(); // senses changes in FSM and changes state accordingly
    ChefStates currState;
	string stateToString(ChefStates state);
    float cookTime;
    void setHasOrder(bool);
private:
    bool hasCollected;
    bool hasOrder;
};

#endif
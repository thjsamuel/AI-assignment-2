#ifndef S_WAITER_H
#define S_WAITER_H

#include "..\Samuel\Character.h"

class Waiter : public Character
{
public:
    Waiter();
    Waiter(MyVector min, MyVector max);
    ~Waiter();
    enum WaiterStates
    {
        IDLE,
        ARRANGE,
        TAKE_ORDER,
        COLLECT,
        SERVE,
        MAX_BASE,
    };
    virtual void init();
    void update(double dt); // reads in other FSM states and updates own FSM
    void runFSM(); // senses changes in FSM and changes state accordingly
    WaiterStates state;
    // imagine the diagonal vertices of a rectangle, one vertice is the min, the other the max, they are coordinates so they are repr5esented with MyVector
    MyVector min; // min of restaurant height
    MyVector max; // max of restaurant height, diagonally from min
    const int getTables();
    string stateToString(WaiterStates state);
    int num_serve;
    int serving_num;
    int* servingOrder;
    bool collect;
    bool isCustomerReady;
    bool goFast;
    MyVector collectArea;
private:
    int tables; // seats he or she can move
};

#endif
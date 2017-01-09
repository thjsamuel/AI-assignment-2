#ifndef S_CUSTOMER_H
#define S_CUSTOMER_H

#include "..\Samuel\Character.h"

class S_Customer : public Character
{
public:
    S_Customer();
    ~S_Customer();
    enum S_CustomerStates
    {
        IDLE,
        LINEUP,
        ORDER,
        LEAVE,
        MAX_BASE,
    };
    enum HUNGRYSTATE
    {
        YES,
        NO,
        ANGRY,
        MAX_STATUS
    };
    struct Color
    {
        float r;
        float g;
        float b;
        Color(float r, float g, float b) : r(0), g(0), b(0)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }
    };
    virtual void init();
    void update(double dt); // reads in other FSM states and updates own FSM
    void runFSM(); // senses changes in FSM and changes state accordingly
    void runNotHungryFSM();
    void renderFSM(); // senses changes in FSM and changes render accordingly
    S_CustomerStates state;
    string stateToString(S_CustomerStates state);
    int hungryChance;
    int getTableNum();
    void setTableNum(int);
    HUNGRYSTATE status;
    Color color;
    void setPatience(float patience);
    float getPatience();
    bool getAngry;
    bool isReady;
private:
    int tableNum;
    float patience; // if out of patience, gets angry, increases in speed
};

#endif
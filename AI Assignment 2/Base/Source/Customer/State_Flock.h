#ifndef STATE_FLOCK_H
#define STATE_FLOCK_H

#include "../State.h"

class CCustomer;

class CState_Flock : public CState<CCustomer>
{
public:
    static CState_Flock* GetInstance();

    virtual void Enter(CCustomer* customer, double dt);
    virtual void Execute(CCustomer* customer, double dt);
    virtual void Exit(CCustomer* customer, double dt);
    virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);
    void Composite(CCustomer* customer, double dt); // Contains codes for composition in flocking, travels towards centre of mass
    void Seperate(CCustomer* customer, double dt); // Contains codes for seperation in flocking, travels towards centre of mass
private:
    CState_Flock();
};

#endif
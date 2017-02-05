#ifndef STATE_PAY_H
#define STATE_PAY_H

#include "../State.h"

class CCustomer;

class CState_Pay : public CState<CCustomer>
{
public:
    static CState_Pay* GetInstance();

    virtual void Enter(CCustomer* customer, double dt);
    virtual void Execute(CCustomer* customer, double dt);
    virtual void Exit(CCustomer* customer, double dt);
    virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);
    void Seperate(CCustomer* customer, double dt); // Contains codes for seperation in Paying, travels towards centre of mass
private:
    CState_Pay();
};

#endif
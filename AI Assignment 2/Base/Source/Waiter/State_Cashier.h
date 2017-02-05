#ifndef STATE_CASHIER_H
#define STATE_CASHIER_H

#include "../State.h"

class CWaiter;
struct Telegram;

class CState_Cashier : public CState<CWaiter>
{
public:
    static CState_Cashier* GetInstance();

    virtual void Enter(CWaiter* waiter, double dt);
    virtual void Execute(CWaiter* waiter, double dt);
    virtual void Exit(CWaiter* waiter, double dt);
    virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
    CState_Cashier();
};

#endif // STATE_Cashier_H
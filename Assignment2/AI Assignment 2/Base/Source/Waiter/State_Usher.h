#ifndef STATE_USHER
#define STATE_USHER

#include "../State.h"

class CWaiter;

class CState_Usher : public CState<CWaiter>
{
public:
    static CState_Usher* GetInstance();

    virtual void Enter(CWaiter* waiter, double dt);
    virtual void Execute(CWaiter* waiter, double dt);
    virtual void Exit(CWaiter* waiter, double dt);
    virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
    CState_Usher();
    bool send_group;
};

#endif // STATE_WAITER_IDLE
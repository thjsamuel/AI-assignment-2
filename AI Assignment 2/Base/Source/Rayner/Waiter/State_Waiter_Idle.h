#ifndef STATE_WAITER_IDLE
#define STATE_WAITER_IDLE

#include "../State.h"

class CWaiter;

class CState_Waiter_Idle : public CState<CWaiter>
{
public:
	static CState_Waiter_Idle* GetInstance();

	virtual void Enter(CWaiter* waiter, double dt);
	virtual void Execute(CWaiter* waiter, double dt);
	virtual void Exit(CWaiter* waiter, double dt);
	virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
	CState_Waiter_Idle();
};

#endif // STATE_WAITER_IDLE
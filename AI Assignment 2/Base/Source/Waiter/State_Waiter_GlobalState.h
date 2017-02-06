#ifndef STATE_CLEANER_GLOBAL_STATE_H
#define STATE_CLEANER_GLOBAL_STATE_H

#include "../State.h"

class CWaiter;

class CState_Waiter_GlobalState : public CState<CWaiter>
{
public:
	static CState_Waiter_GlobalState* GetInstance();

	virtual void Enter(CWaiter* waiter, double dt);
	virtual void Execute(CWaiter* waiter, double dt);
	virtual void Exit(CWaiter* waiter, double dt);
	virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
	CState_Waiter_GlobalState();

	bool bSetLateOrder;
};

#endif // STATE_CLEANER_GLOBAL_STATE_H
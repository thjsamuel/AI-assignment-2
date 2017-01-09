#ifndef STATE_WAITER_GO_TOILET_H
#define STATE_WAITER_GO_TOILET_H

#include "../State.h"

class CWaiter;

class CState_Waiter_GoToilet : public CState<CWaiter>
{
public:
	static CState_Waiter_GoToilet* GetInstance();

	virtual void Enter(CWaiter* waiter, double dt);
	virtual void Execute(CWaiter* waiter, double dt);
	virtual void Exit(CWaiter* waiter, double dt);
	virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
	CState_Waiter_GoToilet();
};

#endif // STATE_WAITER_GO_TOILET_H
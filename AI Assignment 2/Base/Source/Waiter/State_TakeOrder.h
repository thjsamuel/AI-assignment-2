#ifndef STATE_TAKE_ORDER_H
#define STATE_TAKE_ORDER_H

#include "../State.h"

class CWaiter;
struct Telegram;

class CState_TakeOrder : public CState<CWaiter>
{
public:
	static CState_TakeOrder* GetInstance();

	virtual void Enter(CWaiter* waiter, double dt);
	virtual void Execute(CWaiter* waiter, double dt);
	virtual void Exit(CWaiter* waiter, double dt);
	virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
	CState_TakeOrder();
};

#endif // STATE_TAKE_ORDER_H
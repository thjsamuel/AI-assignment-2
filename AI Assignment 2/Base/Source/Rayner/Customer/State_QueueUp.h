#ifndef STATE_QUEUE_UP_H
#define STATE_QUEUE_UP_H

#include "../State.h"

class CCustomer;

class CState_QueueUp : public CState<CCustomer>
{
public:
	static CState_QueueUp* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_QueueUp();
};

#endif // STATE_QUEUE_UP_H
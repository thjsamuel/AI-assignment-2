#ifndef STATE_LEAVE_H
#define STATE_LEAVE_H

#include "../State.h"

class CCustomer;

class CState_Leave : public CState <CCustomer>
{
public:
	static CState_Leave* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_Leave();

	Vector3 targetPos;
	bool bAtBeforeEntrance;
	bool bAtEntrance;
};

#endif // STATE_LEAVE_H
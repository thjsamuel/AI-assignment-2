#ifndef STATE_FIND_SEAT_H
#define STATE_FIND_SEAT_H

#include "../State.h"

class CCustomer;

class CState_FindSeat : public CState<CCustomer>
{
public:
	static CState_FindSeat* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_FindSeat();
};

#endif // STATE_FIND_SEAT_H
#ifndef STATE_ORDER_FOOD_H
#define STATE_ORDER_FOOD_H

#include "../State.h"

class CCustomer;
struct Telegram;

class CState_OrderFood : public CState<CCustomer>
{
public:
	static CState_OrderFood* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_OrderFood();
};

#endif // STATE_ORDER_FOOD_H
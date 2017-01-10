#ifndef STATE_CUSTOMER_IDLE
#define STATE_CUSTOMER_IDLE

#include "../State.h"

class CCustomer;

class CState_Customer_Idle : public CState<CCustomer>
{
public:
	static CState_Customer_Idle* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_Customer_Idle();
};

#endif // STATE_CUSTOMER_IDLE
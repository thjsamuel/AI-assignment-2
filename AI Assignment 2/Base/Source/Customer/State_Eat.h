#ifndef STATE_EAT
#define STATE_EAT

#include "../State.h"

class CCustomer;

class CState_Eat : public CState<CCustomer>
{
public:
	static CState_Eat* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_Eat();

	bool bTimerSet;
	double timeTillDone;
	bool bCoinFlipped;
	int coin;
	bool bSentMsgToCleaner;
	bool bLeftTable;
};

#endif // STATE_EAT
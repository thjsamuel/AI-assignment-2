#ifndef STATE_COOK_H
#define STATE_COOK_H

#include "../State.h"

class Chef;
struct Telegram;

class CState_Cook : public CState <CChef>
{
public:
	static CState_Cook* GetInstance();

	virtual void Enter(CChef* chef, double dt);
	virtual void Execute(CChef* chef, double dt);
	virtual void Exit(CChef* chef, double dt);
	virtual bool OnMessage(CChef* chef, const Telegram& telegram);

private:
	CState_Cook();

	bool isTimerSet;
	double finishedCookingTime;
};

#endif // STATE_COOK_H
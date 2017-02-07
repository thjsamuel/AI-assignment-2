#ifndef STATE_CLEANER_GO_TOILET_H
#define STATE_CLEANER_GO_TOILET_H

#include "../State.h"

class CCleaner;

class CState_Cleaner_GoToilet : public CState<CCleaner>
{
public:
	static CState_Cleaner_GoToilet* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_Cleaner_GoToilet();
};

#endif // STATE_CLEANER_GO_TOILET_H
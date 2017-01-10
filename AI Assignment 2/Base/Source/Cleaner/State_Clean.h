#ifndef STATE_CLEAN_H
#define STATE_CLEAN_H

#include "../State.h"

class CCleaner;

class CState_Clean : public CState<CCleaner>
{
public:
	static CState_Clean* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_Clean();
};

#endif // STATE_CLEAN_H
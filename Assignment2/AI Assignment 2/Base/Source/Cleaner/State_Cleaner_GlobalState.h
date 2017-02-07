#ifndef STATE_CLEANER_GLOBAL_STATE_H
#define STATE_CLEANER_GLOBAL_STATE_H
			  
#include "../State.h"

class CCleaner;

class CState_Cleaner_GlobalState : public CState<CCleaner>
{
public:
	static CState_Cleaner_GlobalState* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_Cleaner_GlobalState();
};

#endif // STATE_CLEANER_GLOBAL_STATE_H
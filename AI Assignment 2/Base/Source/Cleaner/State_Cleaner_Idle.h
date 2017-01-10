#ifndef STATE_CLEANER_IDLE
#define STATE_CLEANER_IDLE

#include "../State.h"

class CCleaner;

class CState_Cleaner_Idle : public CState<CCleaner>
{
public:
	static CState_Cleaner_Idle* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_Cleaner_Idle();
};

#endif // STATE_CLEANER_IDLE
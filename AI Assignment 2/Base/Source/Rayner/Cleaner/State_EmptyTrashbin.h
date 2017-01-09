#ifndef STATE_EMPTY_TRASHBIN_H
#define STATE_EMPTY_TRASHBIN_H

#include "../State.h"

class CCleaner;

class CState_EmptyTrashbin : public CState<CCleaner>
{
public:
	static CState_EmptyTrashbin* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_EmptyTrashbin();
};

#endif // STATE_EMPTY_TRASHBIN_H
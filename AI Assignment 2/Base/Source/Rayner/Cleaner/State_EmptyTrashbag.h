#ifndef STATE_EMPTY_TRASHBAG_H
#define STATE_EMPTY_TRASHBAG_H

#include "../State.h"

class CCleaner;

class CState_EmptyTrashbag : public CState<CCleaner>
{
public:
	static CState_EmptyTrashbag* GetInstance();

	virtual void Enter(CCleaner* cleaner, double dt);
	virtual void Execute(CCleaner* cleaner, double dt);
	virtual void Exit(CCleaner* cleaner, double dt);
	virtual bool OnMessage(CCleaner* cleaner, const Telegram& telegram);

private:
	CState_EmptyTrashbag();
};

#endif // STATE_EMPTY_TRASHBAG_H
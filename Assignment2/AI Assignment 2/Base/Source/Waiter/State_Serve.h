#ifndef STATE_SERVE_H
#define STATE_SERVE_H

#include "../State.h"

class CWaiter;
struct Telegram;

class CState_Serve : public CState<CWaiter>
{
public:
	static CState_Serve* GetInstance();

	virtual void Enter(CWaiter* waiter, double dt);
	virtual void Execute(CWaiter* waiter, double dt);
	virtual void Exit(CWaiter* waiter, double dt);
	virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
	CState_Serve();
};

#endif // STATE_SERVE_H
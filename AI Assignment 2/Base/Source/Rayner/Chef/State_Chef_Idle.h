#ifndef STATE_CHEF_IDLE
#define STATE_CHEF_IDLE

#include "../State.h"

class CChef;
struct Telegram;

class CState_Chef_Idle : public CState<CChef>
{
public:
	static CState_Chef_Idle* GetInstance();

	virtual void Enter(CChef* chef, double dt);
	virtual void Execute(CChef* chef, double dt);
	virtual void Exit(CChef* chef, double dt);
	virtual bool OnMessage(CChef* chef, const Telegram& telegram);

private:
	CState_Chef_Idle();
};

#endif // STATE_CHEF_IDLE
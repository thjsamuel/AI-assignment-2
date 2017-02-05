#ifndef STATE_ARRANGE_H
#define STATE_ARRANGE_H

#include "../State.h"

class CWaiter;
struct Telegram;

class CState_Arrange : public CState<CWaiter>
{
public:
    static CState_Arrange* GetInstance();

    virtual void Enter(CWaiter* waiter, double dt);
    virtual void Execute(CWaiter* waiter, double dt);
    virtual void Exit(CWaiter* waiter, double dt);
    virtual bool OnMessage(CWaiter* waiter, const Telegram& telegram);

private:
    CState_Arrange();

	void AddSeats(CWaiter* waiter, double dt);
	void RemoveSeats(CWaiter* waiter, double dt);

	bool bSettingTable;
	bool bEmptyTable;
	bool bAtTable;
};

#endif // STATE_ARRANGE_H
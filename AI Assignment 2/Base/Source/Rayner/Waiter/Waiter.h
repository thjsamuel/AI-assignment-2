#ifndef WAITER_H
#define WAITER_H

#include "../BaseGameEntity.h"
#include "State_Waiter_Idle.h"
#include "State_TakeOrder.h"
#include "State_Serve.h"
#include "State_Waiter_GoToilet.h"
#include "State_Waiter_GlobalState.h"
#include "../StateMachine.h"

#include <queue>

class CWaiter : public CBaseGameEntity
{
public:
	enum SIDE
	{
		SIDE_TOP,
		SIDE_BOTTOM,
	};

	CWaiter(int ID);
	~CWaiter();

	virtual void Update(double dt);
	virtual bool HandleMessage(const Telegram& msg);
	virtual std::string GetStateInText();

	CStateMachine<CWaiter>* GetFSM() const;
	std::queue <Telegram>* GetMsgQueue() const;
	SIDE GetSide();

	void SetInToiletStatus(bool bInToilet);
	bool GetInToiletStatus();

	unsigned int GetUnservedCount();
	void IncreaseUnservedCount();
	void DecreaseUnservedCount();

	void SetServedLateStatus(bool bServedLate);
	bool GetServedLateStatus();

	unsigned int GetLateOrderCount();
	void IncreaseLateOrderCount();
	void DecreaseLateOrderCount();

	void SetTakeOrderLateStatus(bool bTakeOrderLate);
	bool GetTakeOrderLateStatus();

private:
	CStateMachine<CWaiter>* m_pStateMachine;
	std::queue <Telegram>* msgQueue;

	SIDE side;
	bool bInToilet;
	unsigned int unservedCount;
	bool bServedLate;

	unsigned int lateOrderCount;
	bool bTakeOrderLate;
};

#endif // WAITER_H
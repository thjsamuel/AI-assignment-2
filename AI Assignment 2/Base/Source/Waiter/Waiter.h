#ifndef WAITER_H
#define WAITER_H

#include "../BaseGameEntity.h"
#include "State_Waiter_Idle.h"
#include "State_TakeOrder.h"
#include "State_Serve.h"
#include "State_Arrange.h"
#include "State_Usher.h"
#include "State_Waiter_GoToilet.h"
#include "State_Waiter_GlobalState.h"
#include "../StateMachine.h"
#include "../SeatArranger.h"

#include <queue>

//class CCustomer;
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
	std::queue <unsigned int>* GetNumCustomersInGrp() const;
	SeatArranger* GetSeatArranger() const;
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

    int tables_left;
    int chairs;
    bool need_help; // if too many customers need to be served, waiter is overwhelmed and asks for help
    int current_serving; // Which customer waiter is currently serving
private:
	CStateMachine<CWaiter>* m_pStateMachine;
	std::queue <Telegram>* msgQueue;
	std::queue <unsigned int>* numCustomersInGrp;

	SeatArranger* seatArranger;

	SIDE side;
	bool bInToilet;
	unsigned int unservedCount;
	bool bServedLate;

	unsigned int lateOrderCount;
	bool bTakeOrderLate;
};

#endif // WAITER_H
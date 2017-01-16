#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "../BaseGameEntity.h"
#include "State_Customer_Idle.h"
#include "State_QueueUp.h"
#include "State_Flock.h"
#include "State_FindSeat.h"
#include "State_OrderFood.h"
#include "State_Eat.h"
#include "State_ReturnTray.h"
#include "State_Leave.h"
#include "../StateMachine.h"

struct Telegram;

class CCustomer : public CBaseGameEntity
{
public:
	CCustomer(int ID, Vector3 seatPos, bool bIsLeader = false);
	~CCustomer();

	virtual void Update(double dt);
	virtual bool HandleMessage(const Telegram& msg);
	virtual std::string GetStateInText();

	CStateMachine<CCustomer>* GetFSM() const;

	void SetLeaderStatus(bool bIsLeader);
	bool GetLeaderStatus() const;
    Vector3 centre_of_mass; // This are the positions where the flock members are supposed to go to achieve cohesion, aka their destination
    Vector3 m_repelVec; // The direction and distance vector to repel away from another flock member
private:
	CStateMachine<CCustomer>* m_pStateMachine;
	bool bIsLeader;
};

#endif // CUSTOMER_H
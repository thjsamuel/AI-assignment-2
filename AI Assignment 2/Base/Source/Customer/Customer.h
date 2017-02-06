#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>

#include "../BaseGameEntity.h"
#include "State_Customer_Idle.h"
#include "State_QueueUp.h"
#include "State_Flock.h"
#include "State_FindSeat.h"
#include "State_OrderFood.h"
#include "State_Eat.h"
#include "State_ReturnTray.h"
#include "State_Leave.h"
#include "State_Pay.h"
#include "../StateMachine.h"

struct Telegram;

class CCustomer : public CBaseGameEntity
{
public:
	CCustomer(int ID, Vector3 seatPos, bool bIsLeader = false, Vector3 pos = Vector3(145, 10, 0), bool bInGroup = false);
	~CCustomer();

	virtual void Update(double dt);
	virtual bool HandleMessage(const Telegram& msg);
	virtual std::string GetStateInText();

	CStateMachine<CCustomer>* GetFSM() const;

	void SetLeaderStatus(bool bIsLeader);
	bool GetLeaderStatus() const;

	void SetInGroupStatus(bool bInGroup);
	bool GetInGroupStatus();

	void SetHasSeatStatus(bool bHasSeat);
	bool GetHasSeatStatus();

	void AddMember(CCustomer* customer);
	std::vector<CCustomer*>* GetMembers();

    Vector3 centre_of_mass; // This are the positions where the flock members are supposed to go to achieve cohesion, aka their destination
    Vector3 m_repelVec; // The direction and distance vector to repel away from another flock member
    Group group_num; // Flock this entity belongs to
    float speed;
    int num_in_group; // Number of customers in the flock that he belongs in
    bool move_to_front;
private:
	CStateMachine<CCustomer>* m_pStateMachine;
	bool bIsLeader;
	bool bInGroup;
	bool bHasSeat;
	std::vector<CCustomer*>* groupMembers;
};

#endif // CUSTOMER_H

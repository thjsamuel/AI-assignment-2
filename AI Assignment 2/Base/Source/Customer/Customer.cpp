#include "Customer.h"
#include "../Messaging/Telegram.h"

CCustomer::CCustomer(int ID, Vector3 seatPos, bool bIsLeader, Vector3 pos, bool bInGroup)
: CBaseGameEntity(ID)
, bIsLeader(bIsLeader)
, bInGroup(bInGroup)
, bHasSeat(false)
, bOrdered(false)
, tableID(0)
, group_num(Group::GROUP_MAX)
, speed(25)
, num_in_group(0)
{
	m_pStateMachine = new CStateMachine<CCustomer>(this);
    //if (bIsLeader == false)
	    m_pStateMachine->SetCurrentState(CState_Flock::GetInstance());
    //else
        //m_pStateMachine->SetCurrentState(CState_QueueUp::GetInstance());
	SetSeatPosition(seatPos);
    position = pos;

	groupMembers = new std::vector<CCustomer*>();
}

CCustomer::~CCustomer()
{
	if (m_pStateMachine)
		delete m_pStateMachine;

	for (std::vector<CCustomer*>::iterator it = groupMembers->begin(); it != groupMembers->end(); it++)
	{
		delete *it;
		groupMembers->erase(it);
	}
}

void CCustomer::Update(double dt)
{
	m_pStateMachine->Update(dt);
}

bool CCustomer::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

std::string CCustomer::GetStateInText()
{
	if (m_pStateMachine->GetCurrentState() == CState_Customer_Idle::GetInstance())
		return "Idle";
	else if (m_pStateMachine->GetCurrentState() == CState_QueueUp::GetInstance())
		return "Queue up";
    else if (m_pStateMachine->GetCurrentState() == CState_Flock::GetInstance())
        return "Group up";
	else if (m_pStateMachine->GetCurrentState() == CState_FindSeat::GetInstance())
		return "Find seat";
	else if (m_pStateMachine->GetCurrentState() == CState_OrderFood::GetInstance())
		return "Order food";
	else if (m_pStateMachine->GetCurrentState() == CState_Eat::GetInstance())
		return "Eat";
	else if (m_pStateMachine->GetCurrentState() == CState_ReturnTray::GetInstance())
		return "Return tray";
	else if (m_pStateMachine->GetCurrentState() == CState_Leave::GetInstance())
		return "Leave";
	else if (m_pStateMachine->GetCurrentState() == CState_Pay::GetInstance())
		return "Pay";

	return "No state";
}

CStateMachine<CCustomer>* CCustomer::GetFSM() const
{
	return m_pStateMachine;
}

void CCustomer::SetLeaderStatus(bool bIsLeader)
{
	this->bIsLeader = bIsLeader;
}

bool CCustomer::GetLeaderStatus() const
{
	return bIsLeader;
}

void CCustomer::SetInGroupStatus(bool bInGroup)
{
	this->bInGroup = bInGroup;
}

bool CCustomer::GetInGroupStatus()
{
	return bInGroup;
}

void CCustomer::SetHasSeatStatus(bool bHasSeat)
{
	this->bHasSeat = bHasSeat;
}

bool CCustomer::GetHasSeatStatus()
{
	return bHasSeat;
}

void CCustomer::SetOrderedStatus(bool bOrdered)
{
	this->bOrdered = bOrdered;
}

bool CCustomer::GetOrderedStatus()
{
	return bOrdered;
}

void CCustomer::SetTableID(unsigned int tableID)
{
	this->tableID = tableID;
}

unsigned int CCustomer::GetTableID()
{
	return tableID;
}

void CCustomer::AddMember(CCustomer* customer)
{
	groupMembers->push_back(customer);
}

std::vector<CCustomer*>* CCustomer::GetMembers()
{
	return groupMembers;
}
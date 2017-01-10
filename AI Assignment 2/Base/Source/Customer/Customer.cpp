#include "Customer.h"
#include "../Messaging/Telegram.h"

CCustomer::CCustomer(int ID, Vector3 seatPos, bool bIsLeader)
: CBaseGameEntity(ID)
, bIsLeader(bIsLeader)
{
	m_pStateMachine = new CStateMachine<CCustomer>(this);
	m_pStateMachine->SetCurrentState(CState_QueueUp::GetInstance());

	SetSeatPosition(seatPos);
	SetPosition(145, 10, 0);
}

CCustomer::~CCustomer()
{
	if (m_pStateMachine)
		delete m_pStateMachine;
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
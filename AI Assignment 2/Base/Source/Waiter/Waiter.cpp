#include "Waiter.h"
#include "../Messaging/MessageTypes.h"

CWaiter::CWaiter(int ID)
	: CBaseGameEntity(ID)
	, bInToilet(false)
	, unservedCount(0)
	, bServedLate(false)
	, lateOrderCount(0)
	, bTakeOrderLate(false)
    , tables_left(0)
{
	m_pStateMachine = new CStateMachine<CWaiter>(this);
	m_pStateMachine->SetCurrentState(CState_Waiter_Idle::GetInstance());
	m_pStateMachine->SetGlobalState(CState_Waiter_GlobalState::GetInstance());

	msgQueue = new std::queue<Telegram>();

	SetPosition(50, 50, 0);
}

CWaiter::~CWaiter()
{
	if (m_pStateMachine)
		delete m_pStateMachine;
	if (msgQueue)
		delete msgQueue;
}

void CWaiter::Update(double dt)
{
	//if (!msgQueue->empty())
	{
		m_pStateMachine->Update(dt);

	
		/*switch (msgQueue->front().msg)
		{
			case MSG_ORDER_FOOD_1:
			{
				m_pStateMachine->ChangeState(CState_TakeOrder::GetInstance());
				break;
			}

			case MSG_COLLECT_ORDER:
			{
				m_pStateMachine->ChangeState(CState_Serve::GetInstance());
				break;
			}
		}*/
	}

	if (position.y >= 50.f)
		side = SIDE_TOP;
	else if (position.y < 50.f)
		side = SIDE_BOTTOM;
}

bool CWaiter::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

std::string CWaiter::GetStateInText()
{
	if (m_pStateMachine->GetCurrentState() == CState_Waiter_Idle::GetInstance())
		return "Idle";
	else if (m_pStateMachine->GetCurrentState() == CState_TakeOrder::GetInstance())
		return "Take Order";
	else if (m_pStateMachine->GetCurrentState() == CState_Serve::GetInstance())
		return "Serve food";
	else if (m_pStateMachine->GetCurrentState() == CState_Waiter_GoToilet::GetInstance())
		return "Go Toilet";
    else if (m_pStateMachine->GetCurrentState() == CState_Arrange::GetInstance())
        return "Arrange";
}

CStateMachine<CWaiter>* CWaiter::GetFSM() const
{
	return m_pStateMachine;
}

std::queue <Telegram>* CWaiter::GetMsgQueue() const
{
	return msgQueue;
}

CWaiter::SIDE CWaiter::GetSide()
{
	return side;
}

void CWaiter::SetInToiletStatus(bool bInToilet)
{
	this->bInToilet = bInToilet;
}

bool CWaiter::GetInToiletStatus()
{
	return bInToilet;
}

unsigned int CWaiter::GetUnservedCount()
{
	return unservedCount;
}

void CWaiter::IncreaseUnservedCount()
{
	unservedCount++;
}

void CWaiter::DecreaseUnservedCount()
{
	unservedCount--;
}

void CWaiter::SetServedLateStatus(bool bServedLate)
{
	this->bServedLate = bServedLate;
}

bool CWaiter::GetServedLateStatus()
{
	return bServedLate;
}

unsigned int CWaiter::GetLateOrderCount()
{
	return lateOrderCount;
}

void CWaiter::IncreaseLateOrderCount()
{
	lateOrderCount++;
}

void CWaiter::DecreaseLateOrderCount()
{
	lateOrderCount--;
}

void CWaiter::SetTakeOrderLateStatus(bool bTakeOrderLate)
{
	this->bTakeOrderLate = bTakeOrderLate;
}

bool CWaiter::GetTakeOrderLateStatus()
{
	return bTakeOrderLate;
}
#include "Chef.h"

CChef::CChef(int ID)
: CBaseGameEntity(ID)
{
	m_pStateMachine = new CStateMachine<CChef>(this);
	m_pStateMachine->SetCurrentState(CState_Chef_Idle::GetInstance());

	orderList = new std::queue<ORDER_INFO>();

	SetPosition(40, 85, 0);
}

CChef::~CChef()
{
	if (m_pStateMachine)
		delete m_pStateMachine;
	if (orderList)
		delete orderList;
}

void CChef::Update(double dt)
{
	m_pStateMachine->Update(dt);
}

bool CChef::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

std::string CChef::GetStateInText()
{
	if (m_pStateMachine->GetCurrentState() == CState_Chef_Idle::GetInstance())
		return "Idle";
	else if (m_pStateMachine->GetCurrentState() == CState_Cook::GetInstance())
		return "Cook";
	
	return "No state";
}

CStateMachine<CChef>* CChef::GetFSM() const
{
	return m_pStateMachine;
}

std::queue<CChef::ORDER_INFO>* CChef::GetOrderList() const
{
	return orderList;
}
#include "Customer.h"

#include "../Locations.h"

CState_QueueUp::CState_QueueUp()
{
}

CState_QueueUp* CState_QueueUp::GetInstance()
{
	static CState_QueueUp instance;

	return &instance;
}

void CState_QueueUp::Enter(CCustomer* customer, double dt)
{
	
}

void CState_QueueUp::Execute(CCustomer* customer, double dt)
{
	if (customer->GetPosition().x >= ENTRANCE.x)
		customer->position.x -= 25.f * dt;
	else
		customer->GetFSM()->ChangeState(CState_FindSeat::GetInstance(), dt);
}

void CState_QueueUp::Exit(CCustomer* customer, double dt)
{

}

bool CState_QueueUp::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}
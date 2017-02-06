#include "Customer.h"
#include "State_FindSeat.h"
#include "State_OrderFood.h"

CState_FindSeat::CState_FindSeat()
{
}

CState_FindSeat* CState_FindSeat::GetInstance()
{
	static CState_FindSeat instance;

	return &instance;
}

void CState_FindSeat::Enter(CCustomer* customer, double dt)
{

}

void CState_FindSeat::Execute(CCustomer* customer, double dt)
{
	static float speed = 25.f;

	Vector3 targetPos = customer->GetSeatPosition();

	if (targetPos != 0)
	{
		Vector3 dir = (targetPos - customer->GetPosition()).Normalized();

		if (targetPos.x > customer->GetPosition().x)
			customer->SetSpriteInvertStatus(true);
		else
			customer->SetSpriteInvertStatus(false);

		customer->position += dir * dt * speed;

		if (customer->GetPosition().y >= targetPos.y)
		{
			customer->SetSeatedStatus(true);
			customer->GetFSM()->ChangeState(CState_OrderFood::GetInstance(), dt);
		}
	}
}

void CState_FindSeat::Exit(CCustomer* customer, double dt)
{

}

bool CState_FindSeat::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}
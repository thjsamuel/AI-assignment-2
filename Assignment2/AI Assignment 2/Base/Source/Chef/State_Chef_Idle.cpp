#include "Chef.h"
#include "State_Chef_Idle.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Chef_Idle::CState_Chef_Idle()
{
	bAtSpot = false;
}

CState_Chef_Idle* CState_Chef_Idle::GetInstance()
{
	static CState_Chef_Idle instance;

	return &instance;
}

void CState_Chef_Idle::Enter(CChef* chef, double dt)
{
	bAtSpot = false;
}

void CState_Chef_Idle::Execute(CChef* chef, double dt)
{
	if (chef->GetPosition() != Vector3(40, 85, 0) && bAtSpot == false)
	{
		Vector3 targetPos = Vector3(40, 85, 0);
		Vector3 dir = (targetPos - chef->GetPosition()).Normalized();
		chef->position += dir * dt * 25;
	}
	else
	{
		if (bAtSpot == false)
		{
			chef->SetPosition(40, 85, 0);
			bAtSpot = true;
		}
	}
}

void CState_Chef_Idle::Exit(CChef* chef, double dt)
{

}

bool CState_Chef_Idle::OnMessage(CChef* chef, const Telegram& telegram)
{
	switch (telegram.msg)
	{
		case MSG_ORDER_FOOD_1:
		{
			chef->GetFSM()->ChangeState(CState_Cook::GetInstance());
			chef->GetOrderList()->push(CChef::ORDER_INFO(MSG_ORDER_FOOD_1, 7.0));
			break;
		}

		return true;
	}

	return false;
}
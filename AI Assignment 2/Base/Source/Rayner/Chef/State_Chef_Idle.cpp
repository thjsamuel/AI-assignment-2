#include "Chef.h"
#include "State_Chef_Idle.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Chef_Idle::CState_Chef_Idle()
{
}

CState_Chef_Idle* CState_Chef_Idle::GetInstance()
{
	static CState_Chef_Idle instance;

	return &instance;
}

void CState_Chef_Idle::Enter(CChef* chef, double dt)
{

}

void CState_Chef_Idle::Execute(CChef* chef, double dt)
{
	static bool moveLeft = true;
	static bool moveRight = false;
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
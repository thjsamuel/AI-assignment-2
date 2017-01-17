#include "Waiter.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"

CState_Waiter_Idle::CState_Waiter_Idle()
{
}

CState_Waiter_Idle* CState_Waiter_Idle::GetInstance()
{
	static CState_Waiter_Idle instance;

	return &instance;
}

void CState_Waiter_Idle::Enter(CWaiter* waiter, double dt)
{
	waiter->SetIdleState(true);
}

void CState_Waiter_Idle::Execute(CWaiter* waiter, double dt)
{
	// Move to center of restaurant
	if (waiter->GetPosition() != Vector3(50, 50, 0))
	{
		Vector3 targetPos = Vector3(50, 50, 0);
		Vector3 dir = (targetPos - waiter->GetPosition()).Normalized();

		if (targetPos.x > waiter->GetPosition().x)
			waiter->SetSpriteInvertStatus(true);
		else
			waiter->SetSpriteInvertStatus(false);

		if (waiter->GetSide() == waiter->SIDE_TOP)
		{		
			if (waiter->GetPosition().y >= 50.f)
				waiter->position += dir * dt * 25;
		}
		else if (waiter->GetSide() == waiter->SIDE_BOTTOM)
		{
			if (waiter->GetPosition().y <= 49.f)
				waiter->position += dir * dt * 25;
		}
	}
	else
	{
		waiter->SetPosition(50, 50);
	}
}

void CState_Waiter_Idle::Exit(CWaiter* waiter, double dt)
{
	//if (!waiter->GetMsgQueue()->empty())
		//waiter->GetMsgQueue()->pop();
	waiter->SetIdleState(false);
}

bool CState_Waiter_Idle::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
	// think can move all to global state
	//switch (telegram.msg)
	//{
	//	case MSG_ORDER_FOOD_1:
	//	{
	//		waiter->GetFSM()->ChangeState(CState_TakeOrder::GetInstance());
	//		break;
	//	}

	//	case MSG_COLLECT_ORDER:
	//	{
	//		waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());
	//		break;
	//	}

	//	// Successfully handled the message
	//	return true;
	//}

	return false;
}
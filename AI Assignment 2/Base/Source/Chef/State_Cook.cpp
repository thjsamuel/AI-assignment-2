#include "Chef.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"
#include "../MyTimer.h"

CMyTimer* timer = CMyTimer::GetInstance();

CState_Cook::CState_Cook()
{
	finishedCookingTime = 0.0;
	isTimerSet = false;
}

CState_Cook* CState_Cook::GetInstance()
{
	static CState_Cook instance;

	return &instance;
}

void CState_Cook::Enter(CChef* chef, double dt)
{
	chef->SetCookingState(true);
}

void CState_Cook::Execute(CChef* chef, double dt)
{
	if (!chef->GetOrderList()->empty())
	{
		if (isTimerSet == false)
		{
			finishedCookingTime = timer->GetCurrentTime_() + chef->GetOrderList()->front().cookingTime;

			isTimerSet = true;
		}
		else
		{
			if (timer->GetCurrentTime_() >= finishedCookingTime)
			{
				chef->GetOrderList()->pop();

				// Send message to waiter to serve customer
				CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
					chef->GetID(),
					ENT_WAITER,
					MSG_COLLECT_ORDER,
					NO_EXTRA_INFO);

				isTimerSet = false;
			}
		}
	}

	else
		chef->GetFSM()->ChangeState(CState_Chef_Idle::GetInstance());
}

void CState_Cook::Exit(CChef* chef, double dt)
{
	chef->SetCookingState(false);
}

bool CState_Cook::OnMessage(CChef* chef, const Telegram& telegram)
{
	switch (telegram.msg)
	{
		case MSG_ORDER_FOOD_1:
		{
			chef->GetOrderList()->push(CChef::ORDER_INFO(MSG_ORDER_FOOD_1, 3.0));
			break;
		}

		return true;
	}

	return false;
}

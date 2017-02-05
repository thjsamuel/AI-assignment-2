#include "Waiter.h"

#include <iostream>
#include "../Application.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Waiter_GlobalState::CState_Waiter_GlobalState()
{
}

CState_Waiter_GlobalState* CState_Waiter_GlobalState::GetInstance()
{
	static CState_Waiter_GlobalState instance;

	return &instance;
}

void CState_Waiter_GlobalState::Enter(CWaiter* waiter, double dt)
{

}

void CState_Waiter_GlobalState::Execute(CWaiter* waiter, double dt)
{
	//std::cout << waiter->GetUnservedCount() << std::endl;
	if ((rand() % 5000 + 1) == 1 &&
		!waiter->GetFSM()->IsInState(*CState_Waiter_GoToilet::GetInstance()) &&
		!waiter->GetFSM()->IsInState(*CState_TakeOrder::GetInstance()) &&
		!waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()))
	{
		//waiter->GetFSM()->ChangeState(CState_Waiter_GoToilet::GetInstance());
	}

	if (waiter->GetFSM()->IsInState(*CState_Waiter_Idle::GetInstance()) && waiter->GetUnservedCount() > 0)
	{
		//waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());

		// Send order to chef again?

		waiter->DecreaseUnservedCount();
	}

	if (waiter->GetID() == ENT_WAITER)
	{
		if (waiter->need_help)
		{
			/*CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER_OUTSIDE,
				MSG_HELP_INSIDE,
				NO_EXTRA_INFO);*/ // take out first cuz a second waiter might screw things up
		}
	}
    else
    {
        /*CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
        waiter->GetID(),
        ENT_WAITER_OUTSIDE,
        MSG_HELP_USHER,
        NO_EXTRA_INFO);*/
    }

	if (Application::IsKeyPressed('V')) // if 10 secs past and still no group customers
	{
		//waiter->GetSeatArranger()->AddSeat(1, 1);
		waiter->SetTableID(1);
		waiter->SetRemoveSeatStatus(true);
		waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
	}

	/*if (waiter->GetUnservedCount() > 0 && waiter->GetInToiletStatus() == false &&
	!waiter->GetFSM()->IsInState(*CState_TakeOrder::GetInstance()))
	{
	waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());
	}
	if (waiter->GetLateOrderCount() > 0 && waiter->GetInToiletStatus() == false &&
	!waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()))
	{
	waiter->GetFSM()->ChangeState(CState_TakeOrder::GetInstance());
	}
	if (waiter->GetUnservedCount() <= 0)
	{
	waiter->SetServedLateStatus(false);
	}
	if (waiter->GetLateOrderCount() <= 0)
	{
	waiter->SetTakeOrderLateStatus(false);
	}*/
}

void CState_Waiter_GlobalState::Exit(CWaiter* waiter, double dt)
{

}

bool CState_Waiter_GlobalState::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
	if (waiter->GetID() == ENT_WAITER)
	{
		if (waiter->GetInToiletStatus() == false)
		{
			switch (telegram.msg)
			{
			case MSG_ORDER_FOOD_1:
			{
				// Can take order when waiter is NOT currently serving 
				if (!waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()))
				{
					waiter->GetFSM()->ChangeState(CState_TakeOrder::GetInstance());
					//waiter->GetMsgQueue()->push(telegram);
				}
				//else
					{
						//waiter->SetTakeOrderLateStatus(true);
						//waiter->IncreaseLateOrderCount();
					}

					break;
			}

			case MSG_COLLECT_ORDER:
			{
				//if (waiter->GetFSM()->IsInState(*CState_TakeOrder::GetInstance()))
				//{
				//	//waiter->SetServedLateStatus(true);
				//	waiter->IncreaseUnservedCount();
				//}

				// Can serve when waiter is NOT currently taking order
				//if (!waiter->GetFSM()->IsInState(*CState_TakeOrder::GetInstance()))
				waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());
				//waiter->GetMsgQueue()->push(telegram);
				break;
			}

			case MSG_2CUSTOMER:
			{
				waiter->GetNumCustomersInGrp()->push(2);
				waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
				break;
			}
			case MSG_3CUSTOMER:
			{
				waiter->GetNumCustomersInGrp()->push(3);
				waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
				break;
			}
			case MSG_4CUSTOMER:
			{
				waiter->GetNumCustomersInGrp()->push(4);
				waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
				break;
			}
			case MSG_5CUSTOMERMAX:
			{
				waiter->GetNumCustomersInGrp()->push(5);
				waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
				break;
			}

			// Successfully handled the message
			return true;
			}
		}
	}

	return false;
}

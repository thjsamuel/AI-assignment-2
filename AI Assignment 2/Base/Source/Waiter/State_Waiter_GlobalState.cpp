#include "Waiter.h"

#include <iostream>
#include "../Application.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"
#include "../EntityManager.h"

#include "../Customer/Customer.h"
#include "../Chef/Chef.h"

CState_Waiter_GlobalState::CState_Waiter_GlobalState() : bSetLateOrder(false)
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
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER_OUTSIDE,
				MSG_HELP_INSIDE,
				NO_EXTRA_INFO); // take out first cuz a second waiter might screw things up
		}
        else
        {
            CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
            waiter->GetID(),
            ENT_WAITER_OUTSIDE,
            MSG_HELP_USHER,
            NO_EXTRA_INFO);
        }
	}

	//if (Application::IsKeyPressed('V')) // if 10 secs past and still no group customers
	//{
	//	//waiter->GetSeatArranger()->AddSeat(1, 1);
	//	waiter->SetTableID(1);
	//	waiter->SetRemoveSeatStatus(true);
	//	waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
	//}

	// If 5 secs past and still no group customers
	if (waiter->GetID() == ENT_WAITER)
	{
		for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
		{
			CTable* table = CEntityManager::GetInstance()->GetTableList()->at(i);

			if (table->GetRemoveStatus() && table->GetActive())
			{
				waiter->SetTableID(table->GetID());
				waiter->SetRemoveSeatStatus(true);
				waiter->GetFSM()->ChangeState(CState_Arrange::GetInstance());
			}
		}
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

	if (/*!waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()) 
		&& !waiter->GetFSM()->IsInState(*CState_Arrange::GetInstance()*/
		waiter->GetFSM()->IsInState(*CState_Waiter_Idle::GetInstance())
		&& waiter->GetLateOrderCount() > 0)
	{
		// may have problems if doing this, since waiter will server in order of appearance, cannot go back to previous customer
		// or maybe becauses havent decrement late order count
		waiter->GetFSM()->ChangeState(CState_TakeOrder::GetInstance());
	}

	// If customer is idle and chef is also idle, order has been missed. So send order to chef again
	CBaseGameEntity* entity = CEntityManager::GetInstance()->GetEntityFromID(ENT_CHEF);
	CChef* chef = dynamic_cast<CChef*>(entity);

	for (int i = CEntityManager::GetInstance()->GetStillHereID(); i <= CEntityManager::GetInstance()->GetLatestID(); i++)
	{
		CBaseGameEntity* entity = CEntityManager::GetInstance()->GetEntityFromID(i);
		CCustomer* customer = dynamic_cast<CCustomer*>(entity);

		if (customer->GetFSM()->IsInState(*CState_Customer_Idle::GetInstance()) && chef->GetFSM()->IsInState(*CState_Chef_Idle::GetInstance()))
		{
			// Send order to chef
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_CHEF,
				MSG_ORDER_FOOD_1,
				NO_EXTRA_INFO);
		}
	}

	//std::cout << "late order count: " << waiter->GetLateOrderCount() << std::endl;
}

void CState_Waiter_GlobalState::Exit(CWaiter* waiter, double dt)
{

}

bool CState_Waiter_GlobalState::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
	if (waiter->GetID() == ENT_WAITER)
	{
        //if (telegram.extraInfo != nullptr && waiter->current_serving == -1)
            //waiter->current_serving = *((int*)telegram.extraInfo); // dont fucking take away for no reason, comment out also better
		if (waiter->GetInToiletStatus() == false)
		{
			switch (telegram.msg)
			{
			case MSG_ORDER_FOOD_1:
			{
				// Can take order when waiter is NOT currently serving 
				if (!waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()) && !waiter->GetFSM()->IsInState(*CState_Arrange::GetInstance()))
				{
					waiter->GetFSM()->ChangeState(CState_TakeOrder::GetInstance());
					//waiter->GetMsgQueue()->push(telegram);
				}

				else if (waiter->GetFSM()->IsInState(*CState_Serve::GetInstance()) || waiter->GetFSM()->IsInState(*CState_Arrange::GetInstance()))
				{
					//waiter->SetTakeOrderLateStatus(true);

					//if (!bSetLateOrder)
					{
						waiter->IncreaseLateOrderCount();
						//bSetLateOrder = true; // doesn't solve the problem of incrementing later order count by a lot
					}
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
				if (!waiter->GetFSM()->IsInState(*CState_Arrange::GetInstance()))
					waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());
				//waiter->GetMsgQueue()->push(telegram);
				break;
			}
            
            //case MSG_LEAVE:
            //{
            //    waiter->current_serving = -1;
            //    break;
            //}

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

	else if (waiter->GetID() == ENT_WAITER_OUTSIDE)
	{
		switch (telegram.msg)
		{
        case MSG_PAY:
        {
            if (telegram.extraInfo != nullptr)
                waiter->serve_list.push_back(*((int*)telegram.extraInfo)); // Tells waiter which customer he is going to have to serve
            waiter->GetFSM()->ChangeState(CState_Cashier::GetInstance());
        }
        break;
		case MSG_2CUSTOMER:
		{
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER,
				MSG_2CUSTOMER,
				NO_EXTRA_INFO);
			break;
		}
		case MSG_3CUSTOMER:
		{
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER,
				MSG_3CUSTOMER,
				NO_EXTRA_INFO);
			break;
		}
		case MSG_4CUSTOMER:
		{
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER,
				MSG_4CUSTOMER,
				NO_EXTRA_INFO);
			break;
		}
		case MSG_5CUSTOMERMAX:
		{
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_WAITER,
				MSG_5CUSTOMERMAX,
				NO_EXTRA_INFO);
			break;
		}
		}
	}

	return false;
}

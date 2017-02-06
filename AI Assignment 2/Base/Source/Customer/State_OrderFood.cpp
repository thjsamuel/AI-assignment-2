#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_OrderFood::CState_OrderFood()
{
}

CState_OrderFood* CState_OrderFood::GetInstance()
{
	static CState_OrderFood instance;

	return &instance;
}

void CState_OrderFood::Enter(CCustomer* customer, double dt)
{
	
}

void CState_OrderFood::Execute(CCustomer* customer, double dt)
{
	//if (customer->GetLeaderStatus())
	//{
    int* pass = new int();
    *pass = customer->GetID();
		CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
			customer->GetID(),
			ENT_WAITER,
			MSG_ORDER_FOOD_1,
            pass);
	//}
}

void CState_OrderFood::Exit(CCustomer* customer, double dt)
{

}

bool CState_OrderFood::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	switch (telegram.msg)
	{
		case MSG_ORDER_TAKEN:
		{
			customer->SetOrderedStatus(true);
			customer->GetFSM()->ChangeState(CState_Customer_Idle::GetInstance());
			break;
		}

		// Successfully handled the message
		return true;
	}

	return false;
}
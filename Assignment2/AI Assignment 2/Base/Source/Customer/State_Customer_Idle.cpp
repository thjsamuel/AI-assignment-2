#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"

#include "../EntityNames.h"
#include "../EntityManager.h"

CState_Customer_Idle::CState_Customer_Idle()
{
}

CState_Customer_Idle* CState_Customer_Idle::GetInstance()
{
	static CState_Customer_Idle instance;

	return &instance;
}

void CState_Customer_Idle::Enter(CCustomer* customer, double dt)
{

}

void CState_Customer_Idle::Execute(CCustomer* customer, double dt)
{
	CBaseGameEntity* chef = CEntityManager::GetInstance()->GetEntityFromID(ENT_CHEF);
	CBaseGameEntity* waiter = CEntityManager::GetInstance()->GetEntityFromID(ENT_WAITER);

	// Remind chef to cook order
	if (chef->GetCookingState() == false && waiter->GetIdleState() == true)
	{
		if (customer->GetLeaderStatus())
		{
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				customer->GetID(),
				ENT_CHEF,
				MSG_ORDER_FOOD_1,
				NO_EXTRA_INFO);
		}
	}
}

void CState_Customer_Idle::Exit(CCustomer* customer, double dt)
{

}

bool CState_Customer_Idle::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	switch (telegram.msg)
	{
		case MSG_ORDER_ARRIVED:
		{
			customer->GetFSM()->ChangeState(CState_Eat::GetInstance());
			break;
		}

		return true;
	}

	return false;
}

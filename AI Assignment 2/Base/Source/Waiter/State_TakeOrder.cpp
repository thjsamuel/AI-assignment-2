#include "Waiter.h"

#include "../EntityManager.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"


CState_TakeOrder::CState_TakeOrder()
{
}

CState_TakeOrder* CState_TakeOrder::GetInstance()
{
	static CState_TakeOrder instance;

	return &instance;
}

void CState_TakeOrder::Enter(CWaiter* waiter, double dt)
{

}

void CState_TakeOrder::Execute(CWaiter* waiter, double dt)
{
	CEntityManager* entityMgr = CEntityManager::GetInstance();

	// Cannot use size, it skips the previous customers
	// Start from 3
	static unsigned int currentSize = entityMgr->GetStartingSize();
	static CBaseGameEntity* customer = entityMgr->GetEntityFromID(currentSize);

	if (customer->GetSeatedStatus() == true)
	{
		customer = entityMgr->GetEntityFromID(currentSize);

		Vector3 targetPos = customer->GetSeatPosition();//customer->GetPosition();
		Vector3 dir = (targetPos - waiter->GetPosition()).Normalized();

		if (targetPos.x > waiter->GetPosition().x)
			waiter->SetSpriteInvertStatus(true);
		else
			waiter->SetSpriteInvertStatus(false);

		static bool bAtTable = false;
		static bool bOrderTaken = false;

		//std::cout << currentSize << std::endl;
		// If order not taken, approach customer
		if (bOrderTaken == false)
		{
			// Go to customer
			if ((targetPos - waiter->GetPosition()).LengthSquared() >= 100 && bAtTable == false)
			{
				waiter->position += dir * dt * 25;
			}
			else
			{
				bAtTable = true;
				//bOrderTaken = true;
			}
		}

		// If at customer's table, take order
		if (bAtTable == true)
		{
			// Send order to chef
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				ENT_CHEF,
				MSG_ORDER_FOOD_1,
				NO_EXTRA_INFO);

			// Tell customer order has been taken
			CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
				waiter->GetID(),
				customer->GetID(),
				MSG_ORDER_TAKEN,
				NO_EXTRA_INFO);

			// After taking order of 1 customer, check if there's another customer
			if (entityMgr->GetLatestID() - 1 > currentSize)
			{
				customer = entityMgr->GetEntityFromID(currentSize);
			}
			else if (entityMgr->GetLatestID() - 1 <= currentSize)
			{
				waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
			}

			if (waiter->GetTakeOrderLateStatus() == true)
				waiter->DecreaseLateOrderCount();

			currentSize++;
			bOrderTaken = false;
			bAtTable = false;
		}
	}
	else
	{
		waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
	}
}

void CState_TakeOrder::Exit(CWaiter* waiter, double dt)
{

}

bool CState_TakeOrder::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
	return false;
}

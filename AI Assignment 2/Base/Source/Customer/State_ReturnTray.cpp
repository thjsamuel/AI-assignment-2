#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageTypes.h"
#include "../Messaging/MessageDispatcher.h"

#include "../Locations.h"

CState_ReturnTray::CState_ReturnTray()
{
}

CState_ReturnTray* CState_ReturnTray::GetInstance()
{
	static CState_ReturnTray instance;

	return &instance;
}

void CState_ReturnTray::Enter(CCustomer* customer, double dt)
{

}

void CState_ReturnTray::Execute(CCustomer* customer, double dt)
{
	Vector3 targetPos;
	static bool bReturned = false;

	if (customer->GetPosition().y <= BEFORE_TRASHBIN.y)
	{
		targetPos = BEFORE_TRASHBIN;
	}
	else
	{
		targetPos = TRASHBIN;

		if (customer->GetPosition().y >= TRASHBIN.y)
			bReturned = true;
	}

	if (bReturned == true)
	{
		targetPos = BEFORE_TRASHBIN;

		if (customer->GetPosition().y <= BEFORE_TRASHBIN.y)
		{
            int* pass = new int();
            *pass = customer->GetID();
            CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
                customer->GetID(),
                ENT_WAITER_OUTSIDE,
                MSG_PAY,
                pass);
			customer->GetFSM()->ChangeState(CState_Pay::GetInstance());
			bReturned = false;
		}
	}

	Vector3 dir = (targetPos - customer->GetPosition()).Normalized();
	customer->position += dir * dt * 25;

	if (targetPos.x > customer->GetPosition().x)
		customer->SetSpriteInvertStatus(true);
	else
		customer->SetSpriteInvertStatus(false);
}

void CState_ReturnTray::Exit(CCustomer* customer, double dt)
{

}


bool CState_ReturnTray::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}


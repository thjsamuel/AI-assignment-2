#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageTypes.h"

#include "../EntityManager.h"
#include "../Locations.h"

CState_Leave::CState_Leave()
{
}

CState_Leave* CState_Leave::GetInstance()
{
	static CState_Leave instance;

	return &instance;
}

void CState_Leave::Enter(CCustomer* customer, double dt)
{

}

void CState_Leave::Execute(CCustomer* customer, double dt)
{
	static Vector3 targetPos = BEFORE_ENTRANCE;
	static bool bAtBeforeEntrance = false;
	static bool bAtEntrance = false;
	customer->SetSeatedStatus(false);

	float dist = (targetPos - customer->GetPosition()).LengthSquared();

	if (targetPos == BEFORE_ENTRANCE)
	{
		if (dist <= 100)
		{
			bAtBeforeEntrance = true;
			targetPos = ENTRANCE;
		}
	}
	else if (targetPos == ENTRANCE && bAtBeforeEntrance == true)
	{
		if (dist <= 100)
		{
			bAtEntrance = true;
			targetPos = EXIT;
		}
	}
	else if (targetPos == EXIT)
	{
		if (dist <= 100)
		{
			//targetPos = BEFORE_ENTRANCE;
			customer->SetExitStatus(true);
			//CEntityManager::GetInstance()->RemoveEntity(customer); // may cause problems
		}
	}

	if ((targetPos - customer->GetPosition()).Length() > Math::EPSILON/* && -d > Math::EPSILON*/)
	{
		Vector3 dir = (targetPos - customer->GetPosition()).Normalized();
		customer->position += dir * dt * 25;
	}

	if (targetPos.x > customer->GetPosition().x)
		customer->SetSpriteInvertStatus(true);
	else
		customer->SetSpriteInvertStatus(false);
}

void CState_Leave::Exit(CCustomer* customer, double dt)
{

}

bool CState_Leave::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}


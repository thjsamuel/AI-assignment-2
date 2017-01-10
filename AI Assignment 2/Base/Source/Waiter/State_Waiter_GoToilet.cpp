#include "Waiter.h"
#include "../Locations.h"

CState_Waiter_GoToilet::CState_Waiter_GoToilet()
{
}

CState_Waiter_GoToilet* CState_Waiter_GoToilet::GetInstance()
{
	static CState_Waiter_GoToilet instance;

	return &instance;
}

void CState_Waiter_GoToilet::Enter(CWaiter* waiter, double dt)
{
	waiter->SetInToiletStatus(true);
}

void CState_Waiter_GoToilet::Execute(CWaiter* waiter, double dt)
{
	Vector3 targetPos;
	static bool bAtDoor = false;
	static bool bWentToilet = false;

	if (bWentToilet == false)
	{
		if (waiter->GetPosition().x <= BEFORE_TOILET.x ||
			waiter->GetPosition().y <= BEFORE_TOILET.y)
		{
			targetPos = BEFORE_TOILET;
		}

		if (waiter->GetPosition().x >= BEFORE_TOILET.x)
		{
			bAtDoor = true;
		}

		if (bAtDoor == true)
		{
			targetPos = TOILET;

			if (waiter->GetPosition().y >= TOILET.y)
				bWentToilet = true;
		}
	}

	if (bWentToilet == true)
	{
		targetPos = BEFORE_TOILET;

		if (waiter->GetPosition().y <= BEFORE_TOILET.y)
		{
			waiter->GetFSM()->RevertToPreviousState();
			bAtDoor = false;
			bWentToilet = false;
		}
	}

	if ((targetPos - waiter->GetPosition()).LengthSquared() > 0)
	{
		Vector3 dir = (targetPos - waiter->GetPosition()).Normalized();
		waiter->position += dir * dt * 25;
	}

	if (targetPos.x > waiter->GetPosition().x)
		waiter->SetSpriteInvertStatus(true);
	else
		waiter->SetSpriteInvertStatus(false);
}

void CState_Waiter_GoToilet::Exit(CWaiter* waiter, double dt)
{
	waiter->SetInToiletStatus(false);
}

bool CState_Waiter_GoToilet::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
	return false;
}
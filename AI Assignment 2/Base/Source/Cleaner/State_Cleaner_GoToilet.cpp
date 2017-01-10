#include "Cleaner.h"
#include "../Locations.h"

CState_Cleaner_GoToilet::CState_Cleaner_GoToilet()
{
}

CState_Cleaner_GoToilet* CState_Cleaner_GoToilet::GetInstance()
{
	static CState_Cleaner_GoToilet instance;

	return &instance;
}

void CState_Cleaner_GoToilet::Enter(CCleaner* cleaner, double dt)
{
	cleaner->SetInToiletStatus(true);
}

void CState_Cleaner_GoToilet::Execute(CCleaner* cleaner, double dt)
{
	Vector3 targetPos;
	static bool bAtDoor = false;
	static bool bWentToilet = false;

	if (bWentToilet == false)
	{
		if (cleaner->GetPosition().x <= BEFORE_TOILET.x ||
			cleaner->GetPosition().y <= BEFORE_TOILET.y)
		{
			targetPos = BEFORE_TOILET;
		}
		
		if (cleaner->GetPosition().x >= BEFORE_TOILET.x)
		{
			bAtDoor = true;
		}

		if (bAtDoor == true)
		{
			targetPos = TOILET;

			if (cleaner->GetPosition().y >= TOILET.y)
				bWentToilet = true;
		}
	}

	if (bWentToilet == true)
	{
		targetPos = BEFORE_TOILET;

		if (cleaner->GetPosition().y <= BEFORE_TOILET.y)
		{
			cleaner->GetFSM()->RevertToPreviousState();
			bAtDoor = false;
			bWentToilet = false;
		}
	}

	if ((targetPos - cleaner->GetPosition()).Length() > Math::EPSILON/* && -d > Math::EPSILON*/)
	{
		Vector3 dir = (targetPos - cleaner->GetPosition()).Normalized();
		cleaner->position += dir * dt * 25;
	}

	if (targetPos.x > cleaner->GetPosition().x)
		cleaner->SetSpriteInvertStatus(true);
	else
		cleaner->SetSpriteInvertStatus(false);
}

void CState_Cleaner_GoToilet::Exit(CCleaner* cleaner, double dt)
{
	cleaner->SetInToiletStatus(false);
}

bool CState_Cleaner_GoToilet::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	return false;
}
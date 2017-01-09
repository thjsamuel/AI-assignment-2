#include "Cleaner.h"

#include "../EntityManager.h"

CState_Clean::CState_Clean()
{
}

CState_Clean* CState_Clean::GetInstance()
{
	static CState_Clean instance;

	return &instance;
}

void CState_Clean::Enter(CCleaner* cleaner, double dt)
{

}

void CState_Clean::Execute(CCleaner* cleaner, double dt)
{
	/*
	1. Push in customer's position into cleaningQueue
	2. Once cleaned, pop queue
	*/
	
	CEntityManager* entityMgr = CEntityManager::GetInstance();

	static bool bCleaned = false;

	if (!cleaner->GetUncleanedList()->empty())
	{
		Vector3 targetPos = cleaner->GetUncleanedList()->front();
		Vector3 dir = (targetPos - cleaner->GetPosition()).Normalized();

		if (targetPos.x > cleaner->GetPosition().x)
			cleaner->SetSpriteInvertStatus(true);
		else
			cleaner->SetSpriteInvertStatus(false);

		// Go to uncleaned table
		if ((targetPos - cleaner->GetPosition()).LengthSquared() >= 100 && bCleaned == false)
		{
			cleaner->position += dir * dt * 25;
		}
		else
		{
			bCleaned = true;
		}
	}

	if (bCleaned == true)
	{
		cleaner->GetUncleanedList()->pop();
		cleaner->IncrementTrayCount();

		// Cleaned more than 3 tables, empty full trashbag
		if (cleaner->GetTrayCount() >= 3)
		{
			cleaner->GetFSM()->ChangeState(CState_EmptyTrashbag::GetInstance());
		}
		else
			cleaner->GetFSM()->ChangeState(CState_Cleaner_Idle::GetInstance());

		bCleaned = false;
	}
	
}

void CState_Clean::Exit(CCleaner* cleaner, double dt)
{

}

bool CState_Clean::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	return false;
}
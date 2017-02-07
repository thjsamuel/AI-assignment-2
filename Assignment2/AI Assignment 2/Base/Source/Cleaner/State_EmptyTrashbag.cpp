#include "Cleaner.h"

#include "../Locations.h"

CState_EmptyTrashbag::CState_EmptyTrashbag()
{
	srand(time(NULL));
}

CState_EmptyTrashbag* CState_EmptyTrashbag::GetInstance()
{
	static CState_EmptyTrashbag instance;

	return &instance;
}

void CState_EmptyTrashbag::Enter(CCleaner* cleaner, double dt)
{

}

void CState_EmptyTrashbag::Execute(CCleaner* cleaner, double dt)
{
	Vector3 targetPos;
	static bool bAtDoor = false;
	static bool bEmptied = false;

	if (bEmptied == false)
	{
		if (cleaner->GetPosition().x <= BEFORE_TRASHBIN.x ||
			cleaner->GetPosition().y <= BEFORE_TRASHBIN.y)
		{
			targetPos = BEFORE_TRASHBIN;
		}

		if (cleaner->GetPosition().x >= BEFORE_TRASHBIN.x)
		{
			bAtDoor = true;
		}

		if (bAtDoor == true)
		{
			targetPos = TRASHBIN;

			if (cleaner->GetPosition().y >= TRASHBIN.y)
			{
				cleaner->ResetTrayCount();
				bEmptied = true;

				if ((rand () % 5 + 1) == 1)
					cleaner->GetFSM()->ChangeState(CState_EmptyTrashbin::GetInstance());
			}
		}
	}
	else
	{
		targetPos = BEFORE_TRASHBIN;

		if (cleaner->GetPosition().y <= BEFORE_TRASHBIN.y)
		{
			cleaner->GetFSM()->ChangeState(CState_Cleaner_Idle::GetInstance());
			bAtDoor = false;
			bEmptied = false;
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

void CState_EmptyTrashbag::Exit(CCleaner* cleaner, double dt)
{

}

bool CState_EmptyTrashbag::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	return false;
}
#include "Cleaner.h"

#include "../Messaging/MessageTypes.h"

CState_Cleaner_Idle::CState_Cleaner_Idle()
{
}

CState_Cleaner_Idle* CState_Cleaner_Idle::GetInstance()
{
	static CState_Cleaner_Idle instance;

	return &instance;
}

void CState_Cleaner_Idle::Enter(CCleaner* cleaner, double dt)
{

}

void CState_Cleaner_Idle::Execute(CCleaner* cleaner, double dt)
{
	// Move to idle position
	if (cleaner->GetPosition() != Vector3(93, 70, 0))
	{
		Vector3 targetPos = Vector3(93, 70, 0);
		Vector3 dir = (targetPos - cleaner->GetPosition()).Normalized();

		if (targetPos.x > cleaner->GetPosition().x)
			cleaner->SetSpriteInvertStatus(true);
		else
			cleaner->SetSpriteInvertStatus(false);

		if (cleaner->GetSide() == cleaner->SIDE_TOP)
		{
			if (cleaner->GetPosition().y >= 70.f)
				cleaner->position += dir * dt * 25;
		}
		else if (cleaner->GetSide() == cleaner->SIDE_BOTTOM)
		{
			if (cleaner->GetPosition().y <= 69.f)
				cleaner->position += dir * dt * 25;
		}
	}
	else
	{
		cleaner->SetPosition(93, 70);
	}
}

void CState_Cleaner_Idle::Exit(CCleaner* cleaner, double dt)
{

}

bool CState_Cleaner_Idle::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	return false;
}
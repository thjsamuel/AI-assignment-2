#include "Cleaner.h"

#include "../MyTimer.h"

CState_EmptyTrashbin::CState_EmptyTrashbin()
{
}

CState_EmptyTrashbin* CState_EmptyTrashbin::GetInstance()
{
	static CState_EmptyTrashbin instance;

	return &instance;
}

void CState_EmptyTrashbin::Enter(CCleaner* cleaner, double dt)
{

}

void CState_EmptyTrashbin::Execute(CCleaner* cleaner, double dt)
{
	CMyTimer* timer = CMyTimer::GetInstance();
	static double timeTillRevert = 0.0;

	static bool bTimerSet = false;

	if (bTimerSet == false)
	{
		timeTillRevert = timer->GetCurrentTime_() + 5.0;
		bTimerSet = true;
	}

	else
	{
		if (timer->GetCurrentTime_() > timeTillRevert)
		{
			cleaner->GetFSM()->RevertToPreviousState();
			timeTillRevert = 0.0;
			bTimerSet = false;
		}
	}
}

void CState_EmptyTrashbin::Exit(CCleaner* cleaner, double dt)
{

}

bool CState_EmptyTrashbin::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	return false;
}

#include "Cleaner.h"

#include "../Messaging/MessageTypes.h"

#include "../EntityManager.h"

CState_Cleaner_GlobalState::CState_Cleaner_GlobalState()
{
}

CState_Cleaner_GlobalState* CState_Cleaner_GlobalState::GetInstance()
{
	static CState_Cleaner_GlobalState instance;

	return &instance;
}

void CState_Cleaner_GlobalState::Enter(CCleaner* cleaner, double dt)
{

}

void CState_Cleaner_GlobalState::Execute(CCleaner* cleaner, double dt)
{
	//std::cout << "uncleaned list size" << cleaner->GetUncleanedList()->size() << std::endl;

	if ((rand() % 3000 + 1) == 1 &&
		!cleaner->GetFSM()->IsInState(*CState_Cleaner_GoToilet::GetInstance()) &&
		!cleaner->GetFSM()->IsInState(*CState_Clean::GetInstance()) &&
		!cleaner->GetFSM()->IsInState(*CState_EmptyTrashbag::GetInstance()) &&
		!cleaner->GetFSM()->IsInState(*CState_EmptyTrashbin::GetInstance()))
	{
		cleaner->GetFSM()->ChangeState(CState_Cleaner_GoToilet::GetInstance());
	}

	if (cleaner->GetUncleanedList()->size() > 0 && cleaner->GetInToiletStatus() == false /*&& !cleaner->GetFSM()->IsInState(*CState_Clean::GetInstance())*/)
	{
		cleaner->GetFSM()->ChangeState(CState_Clean::GetInstance());
	}
}

void CState_Cleaner_GlobalState::Exit(CCleaner* cleaner, double dt)
{

}

bool CState_Cleaner_GlobalState::OnMessage(CCleaner* cleaner, const Telegram& telegram)
{
	
		switch (telegram.msg)
		{
			case MSG_CLEAN:
			{
				//cleaner->SetTelegram(telegram);

				CEntityManager* entityMgr = CEntityManager::GetInstance();
				Vector3 seatPos = entityMgr->GetEntityFromID(telegram.senderID)->GetSeatPosition();
				cleaner->GetUncleanedList()->push(seatPos);
				if (cleaner->GetInToiletStatus() == false)
				{
					cleaner->GetFSM()->ChangeState(CState_Clean::GetInstance());
				}
				// 1. Push in customer's seat position into cleaningQueue
				break;
			}

			return true;
		}
	

	return false;
}
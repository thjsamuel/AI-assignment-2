#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"
#include "../EntityManager.h"

#include "../MyTimer.h"

CMyTimer* eatingTimer = CMyTimer::GetInstance();

CState_Eat::CState_Eat() 
	: bTimerSet(false)
	, timeTillDone(0.0)
	, bCoinFlipped(false)
	, coin(-1)
	, bSentMsgToCleaner(false)
	, bLeftTable(false)
{
}

CState_Eat* CState_Eat::GetInstance()
{
	static CState_Eat instance;

	return &instance;
}

void CState_Eat::Enter(CCustomer* customer, double dt)
{
	bLeftTable = false;
}

void CState_Eat::Execute(CCustomer* customer, double dt)
{
	if (bTimerSet == false)
	{
		timeTillDone = eatingTimer->GetCurrentTime_() + 10.0;
		bTimerSet = true;
	}
	
	if (bTimerSet == true)
	{
		if (eatingTimer->GetCurrentTime_() >= timeTillDone)
		{
			// Leave seat at table
			//if (!bLeftTable)
			{
				if (customer->GetInGroupStatus())
				{
					for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
					{
						if (CEntityManager::GetInstance()->GetTableList()->at(i)->GetID() == customer->GetTableID())
						{
							CTable* table = CEntityManager::GetInstance()->GetTableList()->at(i);

							for (int j = 0; j < table->GetNumSeats(); j++)
							{
								if (table->GetSeatList()->at(j)->position == customer->GetSeatPosition())
								{
									table->GetSeatList()->at(j)->bUsing = false;
									bLeftTable = true;
									break;
								}
							}

							//break;
						}
					}
				}
				else
					bLeftTable = true;
			}

			if (bLeftTable)
			{
				if (bCoinFlipped == false)
				{
					coin = rand() % 2;
					bCoinFlipped = true;
				}
			}
		}
	}

	if (bCoinFlipped == true)
	{
		if (coin == 0)
		{
			// Return tray
			customer->GetFSM()->ChangeState(CState_ReturnTray::GetInstance());
			//std::cout << "RETURN TRAY" << std::endl;
		}
		else if (coin == 1)
		{
			// Send message to cleaner
			//std::cout << "LEAVE" << std::endl;
			if (bSentMsgToCleaner == false)
			{
				//if (customer->GetLeaderStatus())
				//{
				CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
					customer->GetID(),
					ENT_CLEANER,
					MSG_CLEAN,
					NO_EXTRA_INFO);
				//}

				bSentMsgToCleaner = true;
			}

			if (bSentMsgToCleaner == true)
			{
				// Leave tray on table
				customer->GetFSM()->ChangeState(CState_Leave::GetInstance());
				//customer->GetFSM()->ChangeState(CState_Pay::GetInstance());
                /*CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
                    customer->GetID(),
                    ENT_WAITER,
                    MSG_PAY,
                    NO_EXTRA_INFO);*/
			}
		}

		coin = -1;
		timeTillDone = 0.0;
		bTimerSet = false;
		bCoinFlipped = false;
		bSentMsgToCleaner = false;
		customer->SetDone(true);
	}
}

void CState_Eat::Exit(CCustomer* customer, double dt)
{

}

bool CState_Eat::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}

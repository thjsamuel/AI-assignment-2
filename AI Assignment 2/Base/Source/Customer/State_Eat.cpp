#include "Customer.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

#include "../MyTimer.h"

CMyTimer* eatingTimer = CMyTimer::GetInstance();

CState_Eat::CState_Eat()
{
	srand(time(NULL));
}

CState_Eat* CState_Eat::GetInstance()
{
	static CState_Eat instance;

	return &instance;
}

void CState_Eat::Enter(CCustomer* customer, double dt)
{

}

void CState_Eat::Execute(CCustomer* customer, double dt)
{
	static bool bTimerSet = false;
	static double timeTillDone = 0.0;
	static bool bCoinFlipped = false;
	static int coin = -1;
	static bool bSentMsgToCleaner = false;

	if (bTimerSet == false)
	{
		timeTillDone = eatingTimer->GetCurrentTime_() + 10.0;
		bTimerSet = true;
	}
	
	if (bTimerSet == true)
	{
		if (eatingTimer->GetCurrentTime_() >= timeTillDone)
		{
			if (bCoinFlipped == false)
			{
				coin = rand() % 2;
				bCoinFlipped = true;
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
				// Leave tray on table and go pay
                int* pass = new int();
                *pass = customer->GetID();
                CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
                    customer->GetID(),
                    ENT_WAITER_OUTSIDE,
                    MSG_PAY,
                    pass);
                customer->GetFSM()->ChangeState(CState_Pay::GetInstance());
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

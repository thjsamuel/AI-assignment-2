#include "MessageDispatcher.h"
#include "../BaseGameEntity.h"
#include "../EntityManager.h"
#include "../MyTimer.h"

CEntityManager* entityMgr = CEntityManager::GetInstance();
CMyTimer* _clock = CMyTimer::GetInstance();

CMessageDispatcher::CMessageDispatcher()
{
}

CMessageDispatcher* CMessageDispatcher::GetInstance()
{
	static CMessageDispatcher instance;

	return &instance;
}

void CMessageDispatcher::DispatchMessage_(double delay,
					int senderID,
					int receiverID,
					int msg,
					void* extraInfo)
{
	CBaseGameEntity* sender = entityMgr->GetEntityFromID(senderID); // Find entity using Ent_Something
	CBaseGameEntity* receiver = entityMgr->GetEntityFromID(receiverID);

	if (receiver == nullptr)
	{
		// No receiver with 'receiverID' found
		return;
	}

	// No need to set telegram's dispatchTime yet, write 0.0
	Telegram telegram(0.0, senderID, receiverID, msg, extraInfo);

	if (delay <= 0.0)
	{
		Discharge(receiver, telegram);
	}
	else
	{
		double currentTime = _clock->GetCurrentTime_();

		telegram.dispatchTime = currentTime + delay; // time taken to dispatch telegram is calculated

		priorityQ.insert(telegram);
	}
}

void CMessageDispatcher::DispatchDelayedMessages()
{
	double currentTime = _clock->GetCurrentTime_();

	// Check if any telegrams need dispatching
	// Remove past telegrams from front of queue
	while (!priorityQ.empty() &&
		  (priorityQ.begin()->dispatchTime < currentTime) &&
		  (priorityQ.begin()->dispatchTime > 0)) // If there are messages in priorityQ that have been delayed for the required amount of time
	{
		const Telegram& telegram = *priorityQ.begin();

		// Find receiver
		CBaseGameEntity* receiver = entityMgr->GetEntityFromID(telegram.receiverID);

		// Send to receiver
		Discharge(receiver, telegram);

		// Remove from queue
		priorityQ.erase(priorityQ.begin());
	}

}

void CMessageDispatcher::Discharge(CBaseGameEntity* receiver, const Telegram& msg)
{
	if (!receiver->HandleMessage(msg))
	{
		// Message could not be handled
	}
}
#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H

#include <set>
#include "Telegram.h"

class CBaseGameEntity;

// For readability
const double	SEND_MSG_IMMEDIATELY = 0.0;
const int		NO_EXTRA_INFO = 0;

class CMessageDispatcher
{
public:
	static CMessageDispatcher* GetInstance();

	void DispatchMessage_(double delay,
						 int senderID,
						 int receiverID,
						 int msg,
						 void* extraInfo);

	void DispatchDelayedMessages();

private:
	CMessageDispatcher();

	// Set doesn't allow duplicates, so no same messages are stored
	std::set<Telegram> priorityQ;

	void Discharge(CBaseGameEntity* receiver, const Telegram& msg);
};

#endif // MESSAGE_DISPATCHER_H
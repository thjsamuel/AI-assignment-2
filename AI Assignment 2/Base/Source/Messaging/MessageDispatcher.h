#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H

#include <set>
#include "Telegram.h"

class CBaseGameEntity;

// For readability
const double	SEND_MSG_IMMEDIATELY = 0.0;
const int		NO_EXTRA_INFO = 0;
const int		MAX_NUMBER_IN_GROUP = 5;

class CMessageDispatcher
{
public:
    // Only one message board can be present at one time
	static CMessageDispatcher* GetInstance();

    // Send message, ID is EntityType Aka Entities enum
	void DispatchMessage_(double delay,
						 int senderID,
						 int receiverID,
						 int msg,
						 void* extraInfo);
    // Messages that haven't been send due to delay are sent again
	void DispatchDelayedMessages();

private:
	CMessageDispatcher(); // Makes sure another message board cannot be created without using the GetInstance() function

	// Set doesn't allow duplicates, so no same messages are stored
	std::set<Telegram> priorityQ;

	void Discharge(CBaseGameEntity* receiver, const Telegram& msg); // Send out the message to client

	bool bSent;
};

#endif // MESSAGE_DISPATCHER_H
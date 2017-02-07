#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>
#include "../EntityNames.h"
#include "MessageTypes.h"

struct Telegram
{
    // Identification of EntityType Aka Entities enum
	int senderID;
	int receiverID;
    int nextMsg;
	int msg;
	double dispatchTime; // time before sending out message
	void* extraInfo; // extra info, can be dynamically cast to any type

	Telegram(double _dispatchTime,
		int _senderID,
		int _receiverID,
		int _msg,
		void* _extraInfo = nullptr) : dispatchTime(_dispatchTime),
		senderID(_senderID),
		receiverID(_receiverID),
		msg(_msg),
		extraInfo(_extraInfo),
        nextMsg(0)
    {
    }

	std::string GetSenderIDInText() const
	{
		switch (senderID)
		{
		case ENT_WAITER:
			return "Waiter Inside";
		case ENT_WAITER_OUTSIDE:
			return "Waiter Outside";
		case ENT_CHEF:
			return "Chef";
		case ENT_CLEANER:
			return "Cleaner";
		default:
			return "Customer";
		}
	}

	std::string GetMsgInText() const
	{
		switch (msg)
		{
		case MSG_ORDER_FOOD_1:
			return "I'd would like to order";
        case MSG_GROUP_NUMBER:
            return "Msg: There is 1 customer";
		case MSG_1CUSTOMER:
			return "Msg: There is 1 customer";
		case MSG_2CUSTOMER:
			return "Msg: There are 2 customers";
		case MSG_3CUSTOMER:
			return"There are 3 customers";
		case MSG_4CUSTOMER:
			return"There are 4 customers";
		case MSG_5CUSTOMERMAX:
			return"There are 5 customers";
		case MSG_ORDER_TAKEN:
			return"Your order has been taken";
		case MSG_COLLECT_ORDER:
			return"Food is ready! Come collect";
		case MSG_ORDER_ARRIVED:
			return"Msg: Here is your food";
	    case MSG_CLEAN:
			return"Msg: Let me clean the table";
		case MSG_HELP_INSIDE:
			return"Msg: Help me, there are too many customers";
        case MSG_HELP_USHER:
            return"Msg: Thanks, you can go back to ushering now";
        case MSG_PAY:
            return"Msg: I want to pay";
        case MSG_LEAVE:
            return"Msg: Bye bye, i am leaving.";
		}
	}

};

const double smallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
    // check if both messages are dispatched around the same time with smallestDelay difference
	return (fabs(t1.dispatchTime - t2.dispatchTime) < smallestDelay) &&
		(t1.senderID == t2.senderID) &&
		(t1.receiverID == t2.receiverID) &&
		(t1.msg == t2.msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1.dispatchTime < t2.dispatchTime);
	}
}

#endif // TELEGRAM_H
#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <math.h>

struct Telegram
{
    // Identification of EntityType Aka Entities enum
	int senderID;
	int receiverID;

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
		extraInfo(_extraInfo)
    {
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
#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <math.h>

struct Telegram
{
	int senderID;
	int receiverID;

	int msg;
	double dispatchTime;
	void* extraInfo;

	Telegram(double _dispatchTime,
		int _senderID,
		int _receiverID,
		int _msg,
		void* _extraInfo = nullptr) : dispatchTime(_dispatchTime),
		senderID(_senderID),
		receiverID(_receiverID),
		msg(_msg),
		extraInfo(_extraInfo)
	{}
};

const double smallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
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
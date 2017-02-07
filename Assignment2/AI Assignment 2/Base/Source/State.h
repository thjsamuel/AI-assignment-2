#ifndef STATE_H
#define STATE_H

struct Telegram;

template <class entity_type>
class CState
{
public:
	virtual ~CState() {}

	virtual void Enter(entity_type*, double dt) = 0;
	virtual void Execute(entity_type*, double dt) = 0;
	virtual void Exit(entity_type*, double dt) = 0;
	virtual bool OnMessage(entity_type*, const Telegram&) = 0;
};

#endif // STATE_H
#ifndef CHEF_H
#define CHEF_H

#include "../BaseGameEntity.h"
#include "State_Chef_Idle.h"
#include "State_Cook.h"
#include "../StateMachine.h"

#include <queue>

struct Telegram;

class CChef : public CBaseGameEntity
{
public:
	struct ORDER_INFO
	{
		unsigned int ORDER_TYPE;
		double cookingTime;

		ORDER_INFO(unsigned int ORDER_TYPE, double cookingTime) 
		{
			this->ORDER_TYPE = ORDER_TYPE;
			this->cookingTime = cookingTime;
		}
	};

	CChef(int ID);
	~CChef();

	virtual void Update(double dt);
	virtual bool HandleMessage(const Telegram& msg);
	virtual std::string GetStateInText();

	CStateMachine<CChef>* GetFSM() const;
	std::queue<ORDER_INFO>* GetOrderList() const;

private:
	CStateMachine<CChef>* m_pStateMachine;
	std::queue<ORDER_INFO>* orderList;
};

#endif // CHEF_H
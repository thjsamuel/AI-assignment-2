#ifndef STATE_RETURN_TRAY_H
#define STATE_RETURN_TRAY_H

#include "../State.h"

class CCustomer;

class CState_ReturnTray : public CState <CCustomer>
{
public:
	static CState_ReturnTray* GetInstance();

	virtual void Enter(CCustomer* customer, double dt);
	virtual void Execute(CCustomer* customer, double dt);
	virtual void Exit(CCustomer* customer, double dt);
	virtual bool OnMessage(CCustomer* customer, const Telegram& telegram);

private:
	CState_ReturnTray();
};

#endif // STATE_RETURN_TRAY_H
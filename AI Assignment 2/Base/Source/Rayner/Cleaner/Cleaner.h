#ifndef CLEANER_H
#define CLEANER_H

#include "../BaseGameEntity.h"
#include "State_Cleaner_Idle.h"
#include "State_Clean.h"
#include "State_Cleaner_GoToilet.h"
#include "State_EmptyTrashbag.h"
#include "State_EmptyTrashbin.h"
#include "State_Cleaner_GlobalState.h"
#include "../StateMachine.h"

#include <queue>

struct Telegram;

class CCleaner : public CBaseGameEntity
{
public:
	enum SIDE
	{
		SIDE_TOP,
		SIDE_BOTTOM,
	};

	CCleaner(int ID);
	~CCleaner();

	virtual void Update(double dt);
	virtual bool HandleMessage(const Telegram& msg);
	virtual std::string GetStateInText();

	CStateMachine<CCleaner>* GetFSM() const;
	SIDE GetSide();

	void SetInToiletStatus(bool bInToilet);
	bool GetInToiletStatus();

	void SetTelegram(Telegram telegram);
	Telegram GetTelegram();

	std::queue<Vector3>* GetUncleanedList() const;

	void IncrementTrayCount();
	void ResetTrayCount();
	unsigned int GetTrayCount();

private:
	CStateMachine<CCleaner>* m_pStateMachine;
	SIDE side;
	bool bInToilet;
	Telegram theTelegram;

	std::queue<Vector3>* uncleanedList;
	unsigned int trayCount;
};

#endif // CLEANER_H
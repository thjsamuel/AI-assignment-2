#include "Cleaner.h"
#include "../Messaging/Telegram.h"

CCleaner::CCleaner(int ID)
	: CBaseGameEntity(ID)
	, bInToilet(false)
	, theTelegram(0, 0, 0, 0, 0)
	, trayCount(0)
{
	m_pStateMachine = new CStateMachine<CCleaner>(this);
	m_pStateMachine->SetCurrentState(CState_Cleaner_Idle::GetInstance());
	m_pStateMachine->SetGlobalState(CState_Cleaner_GlobalState::GetInstance());

	uncleanedList = new std::queue<Vector3>();

	SetPosition(93, 70, 0);
}

CCleaner::~CCleaner()
{
	if (m_pStateMachine)
		delete m_pStateMachine;
}

void CCleaner::Update(double dt)
{
	m_pStateMachine->Update(dt);

	if (position.y >= 70.f)
		side = SIDE_TOP;
	else if (position.y < 70.f)
		side = SIDE_BOTTOM;
}

bool CCleaner::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

std::string CCleaner::GetStateInText()
{
	if (m_pStateMachine->GetCurrentState() == CState_Cleaner_Idle::GetInstance())
		return "Idle";
	else if (m_pStateMachine->GetCurrentState() == CState_Clean::GetInstance())
		return "Clean";
	else if (m_pStateMachine->GetCurrentState() == CState_Cleaner_GoToilet::GetInstance())
		return "Go toilet";
	else if (m_pStateMachine->GetCurrentState() == CState_EmptyTrashbin::GetInstance())
		return "Empty trash bin";
	else if (m_pStateMachine->GetCurrentState() == CState_EmptyTrashbag::GetInstance())
		return "Empty trash bag";
}

CStateMachine<CCleaner>* CCleaner::GetFSM() const
{
	return m_pStateMachine;
}

CCleaner::SIDE CCleaner::GetSide()
{
	return side;
}

void CCleaner::SetInToiletStatus(bool bInToilet)
{
	this->bInToilet = bInToilet;
}

bool CCleaner::GetInToiletStatus()
{
	return bInToilet;
}

void CCleaner::SetTelegram(Telegram telegram)
{
	this->theTelegram = telegram;
}

Telegram CCleaner::GetTelegram()
{
	return theTelegram;
}

std::queue<Vector3>* CCleaner::GetUncleanedList() const
{
	return uncleanedList;
}

void CCleaner::IncrementTrayCount()
{
	trayCount++;
}

void CCleaner::ResetTrayCount()
{
	trayCount = 0;
}

unsigned int CCleaner::GetTrayCount()
{
	return trayCount;
}
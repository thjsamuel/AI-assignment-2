#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include "Messaging\Telegram.h"

template <class entity_type>
class CStateMachine
{
private:
	entity_type*				m_pOwner;
	CState<entity_type>*		m_pCurrentState;
	CState<entity_type>*		m_pPreviousState;
	CState<entity_type>*		m_pGlobalState;

public:
	CStateMachine(entity_type* owner) 
		: m_pOwner(owner)
		, m_pCurrentState(NULL)
		, m_pPreviousState(NULL)
		, m_pGlobalState(NULL)
	{
	}

	virtual ~CStateMachine()
	{
	}

	void SetPreviousState(CState <entity_type>* state)
	{
		m_pPreviousState = state;
	}

	void SetCurrentState(CState <entity_type>* state)
	{
		m_pCurrentState = state;
	}

	void SetGlobalState(CState <entity_type>* state)
	{
		m_pGlobalState = state;
	}

	void Update(double dt)
	{
		if (m_pGlobalState)
			m_pGlobalState->Execute(m_pOwner, dt);

		if (m_pCurrentState)
			m_pCurrentState->Execute(m_pOwner, dt);
	}

	bool HandleMessage(const Telegram& msg) const
	{
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
			return true;

		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
			return true;

		return false;
	}

	void ChangeState(CState <entity_type>* pNewState, double dt = 0.0)
	{
		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->Exit(m_pOwner, dt);
		m_pCurrentState = pNewState;
		m_pCurrentState->Enter(m_pOwner, dt);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	bool IsInState(const CState<entity_type>& state) const
	{
		if (typeid(*m_pCurrentState) == typeid(state))
			return true;

		return false;
	}

	CState<entity_type>*  GetCurrentState()  const 
	{ 
		return m_pCurrentState; 
	
	}

	CState<entity_type>*  GetGlobalState()   const
	{ 
		return m_pGlobalState;
	}

	CState<entity_type>*  GetPreviousState() const
	{ 
		return m_pPreviousState; 
	}
};

#endif // STATE_MACHINE_H
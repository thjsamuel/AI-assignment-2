#include "Waiter.h"

#include "../EntityManager.h"
#include "../Locations.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Arrange::CState_Arrange() : bAtTable(false), bEmptyTable(false), bSettingTable(false)
{
}

CState_Arrange* CState_Arrange::GetInstance()
{
    static CState_Arrange instance;

    return &instance;
}

void CState_Arrange::Enter(CWaiter* waiter, double dt)
{

}

void CState_Arrange::Execute(CWaiter* waiter, double dt)
{
    //// There are still tables left
	Vector3 des;
    if (waiter->chairs >= 0)
    {
        des = waiter->waypoints[0]; // set destination to the random coordinates, i just realised if each waiter has their own tables_left, that's trouble since one storeroom. So need to make tables_left static later
        if (waiter->position != des)
        {
            Vector3 direction = (waiter->position - des).Normalized();
            waiter->position -= direction * (float)(25 * dt);
            //reachDes[tables - 1] = true; // to check whether waiter has reached destination in the future
        }
        else
            --waiter->chairs;
    }
    /*if (waiter->chairs == 0)
        waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());*/

	if (!bSettingTable)
	{
		for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
		{
			CTable* table = CEntityManager::GetInstance()->GetTableList()->at(i);
			if (table->GetActive() && !table->GetUsingState())
			{
				bEmptyTable = true;
				break;
			}
			else if (i == CEntityManager::GetInstance()->GetTableList()->size() - 1)
			{
				bEmptyTable = false;
			}
		}
	}

	/*if (CEntityManager::GetInstance()->GetTableList()->size() <= 0)
		bEmptyTable = true;*/

	if (!waiter->GetRemoveSeatStatus() && !waiter->GetAddSeatStatus() && !bEmptyTable)
	{
		bSettingTable = true;
		if (waiter->GetSeatArranger()->ArrangeSeats(waiter->GetNumCustomersInGrp()->front(), des, dt))
		{
			bSettingTable = false;
			waiter->GetNumCustomersInGrp()->pop(); // must also pop when this group of customers leave?
		}
	}

	// can only either be add or remove at a time
	else if (waiter->GetAddSeatStatus())
	{
		AddSeats(waiter, dt);
	}

	else if (waiter->GetRemoveSeatStatus())
	{
		RemoveSeats(waiter, dt);
	}

	// If there are no more groups, change to idle state
	if (waiter->GetNumCustomersInGrp()->size() <= 0)
	{
		waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
	}
}

void CState_Arrange::AddSeats(CWaiter* waiter, double dt)
{
	CTable* theTable;

	// Find table position
	for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
	{
		CTable* table = CEntityManager::GetInstance()->GetTableList()->at(i);

		if (table->GetID() == waiter->GetTableID())
		{
			theTable = table;
			break;
		}
	}

	// Move to the table
	if ((theTable->GetPos() - waiter->position).LengthSquared() > 4.f && !bAtTable)
	{
		Vector3 dir = (theTable->GetPos() - waiter->position).Normalized();
		waiter->position += dir * (float)(25 * dt);
	}
	else
	{
		bAtTable = true;

		// Remove the table and seats
		unsigned int numSeats = waiter->GetNumCustomersInGrp()->front() - theTable->GetNumSeats();
		if (waiter->GetSeatArranger()->AddSeat(theTable->GetID(), 1)) //numSeats
		{
			waiter->GetNumCustomersInGrp()->pop();

			// Put table and seats in the store room
			if ((STORE_ROOM - waiter->position).LengthSquared() > 4.f)
			{
				Vector3 dir = (STORE_ROOM - waiter->position).Normalized();
				waiter->position += dir * (float)(25 * dt);
			}
			else
			{
				bAtTable = false;
				waiter->SetAddSeatStatus(false);
				waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
			}
		}
	}
}

void CState_Arrange::RemoveSeats(CWaiter* waiter, double dt)
{
	Vector3 tablePos;
	CTable* table;

	// Find table position
	for (int i = 0; i < CEntityManager::GetInstance()->GetTableList()->size(); i++)
	{
		table = CEntityManager::GetInstance()->GetTableList()->at(i);

		if (table->GetID() == waiter->GetTableID())
		{
			tablePos = table->GetPos();
			break;
		}
	}

	// Move to the table
	if ((tablePos - waiter->position).LengthSquared() > 4.f && !bAtTable)
	{
		Vector3 dir = (tablePos - waiter->position).Normalized();
		waiter->position += dir * (float)(25 * dt);
	}
	else
	{
		bAtTable = true;

		// Remove the table and seats
		if (waiter->GetSeatArranger()->RemoveSeats(table->GetID()))
		{
			// Put table and seats in the store room
			if ((STORE_ROOM - waiter->position).LengthSquared() > 4.f)
			{
				Vector3 dir = (STORE_ROOM - waiter->position).Normalized();
				waiter->position += dir * (float)(25 * dt);
			}
			else
			{
				bAtTable = false;
				waiter->SetRemoveSeatStatus(false);
				waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
			}
		}
	}
}

void CState_Arrange::Exit(CWaiter* waiter, double dt)
{
    //if (!waiter->GetMsgQueue()->empty())
    //waiter->GetMsgQueue()->pop();
}

bool CState_Arrange::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
    switch (telegram.msg)
    {
    case MSG_2CUSTOMER:
        waiter->chairs = 2;
        return true;
        break;
    case MSG_5CUSTOMERMAX:
    {
        waiter->chairs = 5;
    }
    break;
    // Successfully handled the message
    return true;
    }
    return false;
}
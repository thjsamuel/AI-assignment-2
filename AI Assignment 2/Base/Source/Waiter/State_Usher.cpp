#include "Waiter.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"

CState_Usher::CState_Usher() 
	: send_group(true)
{
}

CState_Usher* CState_Usher::GetInstance()
{
    static CState_Usher instance;

    return &instance;
}

// What happens at call of ushering state
void CState_Usher::Enter(CWaiter* waiter, double dt)
{

}

// What is updated every frame for this state
void CState_Usher::Execute(CWaiter* waiter, double dt)
{
    // send the number of customers in the group to waiter inside
    if (send_group)
    {
		CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY, waiter->GetID(), ENT_WAITER, MSG_2CUSTOMER, NO_EXTRA_INFO);
        send_group = false;
    }

    // Move to usher spot
    Vector3 usher_spot = waiter->waypoints[0];
    if (waiter->GetPosition() != usher_spot)
    {
        Vector3 dir = (usher_spot - waiter->GetPosition()).Normalized();
        
        // Check where sprite is facing
        if (usher_spot.x > waiter->GetPosition().x)
            waiter->SetSpriteInvertStatus(true);
        else
            waiter->SetSpriteInvertStatus(false);

        waiter->position += dir * (float)(25 * dt);
    }
}

void CState_Usher::Exit(CWaiter* waiter, double dt)
{
    //if (!waiter->GetMsgQueue()->empty())
    //waiter->GetMsgQueue()->pop();
    waiter->SetIdleState(false);
}

bool CState_Usher::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
    switch (telegram.msg)
    {
    	case MSG_HELP_INSIDE:
        {
            waiter->GetFSM()->ChangeState(CState_Serve::GetInstance());
    		break;
    	}
    	// Successfully handled the message
    	return true;
    }

    return false;
}
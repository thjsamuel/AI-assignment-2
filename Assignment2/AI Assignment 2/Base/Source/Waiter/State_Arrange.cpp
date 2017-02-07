#include "Waiter.h"

#include "../EntityManager.h"
#include "../Locations.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Arrange::CState_Arrange()
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
    // There are still tables left
    if (waiter->chairs >= 0)
    {
        Vector3 des = waiter->waypoints[waiter->chairs - 1]; // set destination to the random coordinates, i just realised if each waiter has their own tables_left, that's trouble since one storeroom. So need to make tables_left static later
        if (waiter->position != des)
        {
            Vector3 direction = (waiter->position - des).Normalized();
            waiter->position -= direction * (float)(25 * dt);
            //reachDes[tables - 1] = true; // to check whether waiter has reached destination in the future
        }
        else
            --waiter->chairs;
    }
    if (waiter->chairs == 0)
        waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
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
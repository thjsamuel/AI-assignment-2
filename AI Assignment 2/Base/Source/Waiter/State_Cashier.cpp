#include "Waiter.h"

#include "../EntityManager.h"
#include "../Locations.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageDispatcher.h"
#include "../Messaging/MessageTypes.h"
#include "../EntityNames.h"

CState_Cashier::CState_Cashier()
{
}

CState_Cashier* CState_Cashier::GetInstance()
{
    static CState_Cashier instance;

    return &instance;
}

void CState_Cashier::Enter(CWaiter* waiter, double dt)
{

}

void CState_Cashier::Execute(CWaiter* waiter, double dt)
{
    CEntityManager* entityMgr = CEntityManager::GetInstance();
    static unsigned int currentSize = entityMgr->GetStartingSize();
    static CBaseGameEntity* customer = entityMgr->GetEntityFromID(currentSize);

    Vector3 targetPos = COUNTER;
    Vector3 dir = (targetPos - waiter->GetPosition()).Normalized();
    float dist = (targetPos - waiter->GetPosition()).LengthSquared();

    static bool bCollectedOrder = false;
    static bool bCashierd = false;
    static bool bAtTable = false;

    //std::cout << currentSize << std::endl;
    // If order not collected, collect order
    if (bCollectedOrder == false)//(bCashierd == false)
    {
        // Go to counter
        if (waiter->GetPosition().y <= targetPos.y && bCollectedOrder == false)
            waiter->position += dir * dt * 25;
        else
            bCollectedOrder = true;
    }

    /*if (bCashierd == true)
    {
    if (entityMgr->GetEntityMap().size() - 1 > currentSize)
    {
    if (customer->GetSeatedStatus() == true)
    customer = entityMgr->GetEntityFromID(currentSize);
    bCashierd = false;
    }
    }*/

    if (bCollectedOrder == true)
    {
        customer = entityMgr->GetEntityFromID(currentSize);

        targetPos = customer->GetSeatPosition();
        dir = (targetPos - waiter->GetPosition()).Normalized();
        dist = (targetPos - waiter->GetPosition()).LengthSquared();

        // If not Cashierd, Cashier customer
        if (customer->GetPosition().x < waiter->GetPosition().x)
        {
            if (dist >= 100 && bAtTable == false)
                waiter->position += dir * dt * 25;
            else
                bAtTable = true;
        }
        else
        {
            if (dist >= 100 && bAtTable == false)
                waiter->position += dir * dt * 25;
            else
                bAtTable = true;
        }

        if (bAtTable == true)
        {
            // Send message to customer
            CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
                waiter->GetID(),
                customer->GetID(),
                MSG_ORDER_ARRIVED,
                NO_EXTRA_INFO);

            // After serving to 1 customer, check if there's another customer
            if (entityMgr->GetLatestID() - 1 > currentSize)
            {
                //currentSize++;
                //customer = entityMgr->GetEntityFromID(currentSize);
                targetPos = COUNTER;
                //bCashierd = false;

                waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
            }
            else if (entityMgr->GetLatestID() - 1 <= currentSize)
            {
                waiter->GetFSM()->ChangeState(CState_Waiter_Idle::GetInstance());
                //bCashierd = true;
            }

            //if (waiter->GetCustomerLateStatus() == true)
               // waiter->DecreaseUnCashierdCount();

            currentSize++;
            //bCashierd = false; // true;
            bCashierd = true;
            bAtTable = false;
        }

        if (bCashierd == true)
        {
            bCollectedOrder = false;
            bCashierd = false;
        }
    }

    if (targetPos.x > waiter->GetPosition().x)
        waiter->SetSpriteInvertStatus(true);
    else
        waiter->SetSpriteInvertStatus(false);
}

void CState_Cashier::Exit(CWaiter* waiter, double dt)
{
    //if (!waiter->GetMsgQueue()->empty())
    //waiter->GetMsgQueue()->pop();
}

bool CState_Cashier::OnMessage(CWaiter* waiter, const Telegram& telegram)
{
    switch (telegram.msg)
    {
    case MSG_HELP_USHER:
    {
        if (waiter->GetID() == ENT_WAITER_OUTSIDE)
            waiter->GetFSM()->ChangeState(CState_Usher::GetInstance());
        break;
    }
    return true;
    }
    return false;
}

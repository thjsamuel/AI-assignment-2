#include "Waiter.h"

#include "../EntityManager.h"
#include "../Locations.h"

#include "../Messaging/Telegram.h"
#include "../Messaging/MessageTypes.h"
#include "../Messaging/MessageDispatcher.h"
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

    Vector3 targetPos = CASHIER;
    if (waiter->GetPosition() != targetPos)
    {
        Vector3 dir = (targetPos - waiter->GetPosition()).Normalized();
        waiter->position += dir * (25 * dt);
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
    case MSG_LEAVE:
    {
        if (telegram.extraInfo != nullptr)
        {
            for (int i = 0; i < waiter->serve_list.size(); ++i)
            {
                if (waiter->serve_list[i] == *((int*)telegram.extraInfo))
                    CMessageDispatcher::GetInstance()->DispatchMessage_(SEND_MSG_IMMEDIATELY,
                    waiter->GetID(),
                    *((int*)telegram.extraInfo),
                    MSG_LEAVE,
                    NO_EXTRA_INFO);
                waiter->serve_list[i] = -1;
            }
        }
        waiter->GetFSM()->ChangeState(CState_Usher::GetInstance());
        break;
    }
    return true;
    }
    return false;
}

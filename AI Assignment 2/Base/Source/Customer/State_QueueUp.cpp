#include "Customer.h"

#include "../Locations.h"

CState_QueueUp::CState_QueueUp()
{
}

CState_QueueUp* CState_QueueUp::GetInstance()
{
	static CState_QueueUp instance;

	return &instance;
}

void CState_QueueUp::Enter(CCustomer* customer, double dt)
{
	
}

void CState_QueueUp::Execute(CCustomer* customer, double dt)
{
	//if (customer->GetPosition().x >= ENTRANCE.x)
	//	customer->position.x -= 25.f * dt;
	//else
	//	customer->GetFSM()->ChangeState(CState_FindSeat::GetInstance(), dt);

    Vector3 des = customer->waypoints[0]; // First waypoint in the map is always customer's queue up spot 

    // position is not equal to entrance of restaurant, form a line behind entrance
    if (customer->position != des)
    {
        Vector3 direction = (customer->position - des).Normalized(); // direction towards destination with magnitude of 1
        customer->position -= direction * (float)(25 * dt); // move towards destination
    }

    //if (customer->position == des)
        //customer->GetFSM()->ChangeState(CState_FindSeat::GetInstance(), dt);
}

void CState_QueueUp::Exit(CCustomer* customer, double dt)
{

}

bool CState_QueueUp::OnMessage(CCustomer* customer, const Telegram& telegram)
{
	return false;
}
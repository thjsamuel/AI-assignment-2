#include "Customer.h"

#include "../Locations.h"

CState_Flock::CState_Flock()
{
}

CState_Flock* CState_Flock::GetInstance()
{
    static CState_Flock instance;

    return &instance;
}

void CState_Flock::Enter(CCustomer* customer, double dt)
{

}

void CState_Flock::Composite(CCustomer* customer, double dt)
{
    Vector3 des = customer->centre_of_mass; // First waypoint in the map is always customer's queue up spot 

    // position is not equal to entrance of restaurant, form a line behind entrance
    if (customer->position != des)
    {
        Vector3 direction = (des - customer->position).Normalized(); // direction towards destination with magnitude of 1
        customer->position += direction * (float)(25 * dt); // move towards destination
    }
    else if (customer->m_repelVec == Vector3(0, 0, 0))
    {
        //customer->GetFSM()->ChangeState(CState_QueueUp::GetInstance(), dt);
    }
}

void CState_Flock::Seperate(CCustomer* customer, double dt)
{
    float detect_radius = 5;
    if (customer->m_repelVec != Vector3(0, 0, 0))
    {
        float dist_away = customer->m_repelVec.Length(); // how far is the distance of this entity away from the entity in its detection radius
        float ratio = dist_away / detect_radius; // get how dist_away compares to the detection radius
        //ratio = 1 - ratio;
        //Vector3 repel_force = customer->m_repelVec * (1 / dist_away); // basically normalizing the m_repelVec
        Vector3 repel_force = customer->m_repelVec.Normalized();
// When the m_repelVec normalized is divided by the ratio,
// We get forcefullness of the repel force according to how close the intruding entity is, the closer the intruder, the larger the repel force to drive him away
        repel_force *= (1 / ratio);//* (float)(25 * dt)/* * (1 / ratio)*/;
        customer->m_repelVec.SetZero();
        customer->position += repel_force * (float)(25 * dt);
        // Important: must find a trajectory to move towards that wont result in another collision, maybe can use neural network/a star
        // Important: calculate composition only if leader move
    }
}


//void CState_Flock::Seperate(CCustomer* customer, double dt)
//{
//    float detect_radius = 5;
//    if (customer->m_repelVec != Vector3(0, 0, 0))
//    {
//        float dist_away = customer->m_repelVec.Length(); // how far is the distance of this entity away from the entity in its detection radius
//        float ratio = dist_away / (detect_radius * detect_radius); // get how dist_away compares to the detection radius
//        //ratio = 1 - ratio;
//        //Vector3 repel_force = customer->m_repelVec * (1 / dist_away); // basically normalizing the m_repelVec
//        Vector3 repel_force = customer->m_repelVec.Normalized();
//        // When the m_repelVec normalized is divided by the ratio,
//        // We get forcefullness of the repel force according to how close the intruding entity is, the closer the intruder, the larger the repel force to drive him away
//        repel_force *= (1 / ratio);//* (float)(25 * dt)/* * (1 / ratio)*/;
//        customer->m_repelVec.SetZero();
//        customer->position += repel_force * (float)(20 * dt);
//        // Important: must find a trajectory to move towards that wont result in another collision, maybe can use neural network/a star
//        // Important: calculate composition only if leader move
//    }
//}

void CState_Flock::Execute(CCustomer* customer, double dt)
{
    Composite(customer, dt);
    Seperate(customer, dt);
}

void CState_Flock::Exit(CCustomer* customer, double dt)
{

}

bool CState_Flock::OnMessage(CCustomer* customer, const Telegram& telegram)
{
    return false;
}
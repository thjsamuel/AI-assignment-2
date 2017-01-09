#include "..\Samuel\S_Chef.h"

Chef::Chef() : Character(), hasCollected(true), hasOrder(false), cookTime(3.f)
{
    
}

Character::~Character()
{

}

Chef::~Chef()
{

}

void Chef::init()
{
    currState = IDLE;
}

void Chef::runFSM()
{
    if (position != waypoints[0])
        currState = WALK;
    else
        currState = IDLE;

    if (currState == WAIT)
    {
        if (hasCollected == true)
            currState = IDLE;
    }

    if (currState == IDLE)
        if (hasOrder)
            currState = COOK;
        else
            currState = IDLE;
}

void Chef::update(double dt)
{
    if (position != waypoints[0])
    {
        //if (position.x < waypoints[0].x)
        //{
        //    ++position.x;
        //    if (position.x + 1 == waypoints[0].x)
        //        ++position.x;
        //}
        //else if (position.x > waypoints[0].x)
        //{
        //    --position.x;
        //    if (position.x - 1 == waypoints[0].x)
        //        --position.x;
        //}

        //if (position.y < waypoints[0].y)
        //{
        //    ++position.y;
        //    if (position.y + 1 == waypoints[0].y)
        //        ++position.y;
        //}
        //else if (position.y > waypoints[0].y)
        //{
        //    --position.y;
        //    if (position.y - 1 == waypoints[0].y)
        //        --position.y;
        //}
        MyVector direction = (position - waypoints[0]).Normalize();
        position = position + direction * (float)(35 * dt);
    }
    else if (currState == COOK)
    {
        cookTime -= 5.f * dt;
        if (cookTime < 0)
        {
            cookTime = 0.f;
            hasOrder = false;
        }
    }
}

void Chef::setHasOrder(bool has)
{
    hasOrder = has;
}

string Chef::stateToString(Chef::ChefStates state)
{
	switch (state)
	{
	case WALK:
		return "Walk";
		break;
	case IDLE:
		return "Idle";
		break;
	case COOK:
		return "Cook";
		break;
    case WAIT:
        return "Waiting";
        break;
	}
}

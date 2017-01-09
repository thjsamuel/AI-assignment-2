#include "..\Samuel\S_Customer.h"

S_Customer::S_Customer() : Character(), state(IDLE), hungryChance(75), tableNum(-1), status(ANGRY), color(1, 0, 1), patience(100.f), isReady(false), getAngry(false)
{
}

S_Customer::~S_Customer()
{

}

void S_Customer::init()
{
    //state = WALK
    state = LINEUP;
}

void S_Customer::runFSM()
{
    if (state == IDLE && position != waypoints[0] == false && position != waypoints[1] && tableNum > -1) // if queueing up and has table
    {
        state = ORDER;
    }
    else if (position != waypoints[1] == false)
    {
        //state = IDLE;
        if (patience <= 0)
        {
            state = LEAVE;
            if (getAngry == false)
            {
                status = ANGRY;
                getAngry = true;
            }
        }
    }
    if (position != waypoints[0] == false && state == LINEUP)
        state = IDLE;
}

void S_Customer::runNotHungryFSM()
{
    if (position != waypoints[0] == false && state == LINEUP)
        state = IDLE;
    if (state == IDLE && position != waypoints[0] == false) // if queueing up and has table
    {
        state = LEAVE;
    }
}

void S_Customer::renderFSM()
{
    switch (status)
    {
    case YES:
        color = Color(0, 1, 0);
        break;
    case NO:
        color = Color(1, 1, 0);
        break;
    case ANGRY:
        color = Color(1, 0, 0);
    default:
        color = Color(0, 1, 0);
        break;
    }
}

void S_Customer::update(double dt)
{
    if (position != waypoints[0] && state == LINEUP)
    {
        MyVector direction = (position - waypoints[0]).Normalize();
        position = position + direction * (float)(15 * dt);
    }

    if (state == ORDER && position != waypoints[1])
    {
        MyVector direction = (position - waypoints[1]).Normalize();
        position = position + direction * (float)(15 * dt);
    }
    else if (state == ORDER)
    {
        if (patience > 0)
            patience -= 2.5f * dt;
        else
            patience = 0;
    }
    else if (state == LEAVE && position != waypoints[0])
    {
        MyVector direction = (position - waypoints[0]).Normalize();
        position = position + direction * (float)(15 * dt);
        //isReady = false; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    if (position != waypoints[1] == false)
        isReady = true;

    renderFSM();
}

int S_Customer::getTableNum()
{
    return tableNum;
}

void S_Customer::setTableNum(int num)
{
    tableNum = num;
}

void S_Customer::setPatience(float patience)
{
    this->patience = patience;
}

float S_Customer::getPatience()
{
    return patience;
}

string S_Customer::stateToString(S_Customer::S_CustomerStates state)
{
	switch (state)
	{
	case LINEUP:
		return "Line Up";
		break;
    case ORDER:
        return "Moving";
        break;
	case IDLE:
		return "Idle";
		break;
    case LEAVE:
        return "Leaving";
        break;
	}
}

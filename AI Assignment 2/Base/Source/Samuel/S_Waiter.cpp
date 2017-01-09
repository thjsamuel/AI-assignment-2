#include "..\Samuel\S_Waiter.h"

//int Waiter::serving_num = 0;

Waiter::Waiter() : Character(), tables(7), max(MyVector(0, 0)), min(MyVector(0, 0)), num_serve(0), collect(false), collectArea(0, 0), isCustomerReady(true)
{
    reachDes = new bool[tables];
    position = MyVector(-2.f, -1.f);
}

Waiter::Waiter(MyVector min, MyVector max) : Character(), tables(4), max(MyVector(0, 0)), min(MyVector(0, 0)), num_serve(0), collect(false), collectArea(0, 0), isCustomerReady(false), serving_num(0)
{
    this->min = min;
    this->max = max;
    reachDes = new bool[tables];
    for (int i = 0; i < reachDes[tables]; ++i)
        reachDes[i] = false;
    servingOrder = new int[tables];
    for (int i = 0; i < tables; ++i)
        servingOrder[i] = -1;
    position = MyVector(-2.f, -1.f);
}

Waiter::~Waiter()
{

}

void Waiter::init()
{
    //state = WALK
    state = IDLE;
}


const int Waiter::getTables()
{
    return tables;
}

bool hasToServe(int* serve, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (serve[i] > -1)
            return true;
    }
    return false;
}

void Waiter::runFSM()
{
    // if take too long,  go away
    if (state == IDLE && tables > 0)
    {
        state = ARRANGE;
    }
    else if (tables <= 0 && isCustomerReady == false)
        state = IDLE;
    else if (isCustomerReady && state != COLLECT && state != SERVE)
    {
        state = TAKE_ORDER;
    }
    //if (state == TAKE_ORDER/* && hasToServe(servingOrder, num_serve) == false*/)
    //{
    //    //isCustomerReady = false;
    //    //state = IDLE;
    //}

    if (collect)
    {
        state = COLLECT;
    }
    else if (collect == false && state == COLLECT)
    {
        state = SERVE;
        //state = IDLE;
    }
}

void Waiter::update(double dt)
{
    MyVector origin = MyVector(-2.f, 1.6f);
    if (tables > 0 && state == ARRANGE)
    {
        MyVector direction = (position - waypoints[tables - 1]).Normalize();
        position = position + direction * (float)(35 * dt);
        /*if ((position.x == waypoints[tables - 1].x  && position.x == waypoints[tables - 1].x) && position.y == waypoints[tables - 1].y)*/
        if (position != waypoints[tables - 1] == false)
        {
            reachDes[tables - 1] = true;
            --tables;
        }
    }
    if (state == IDLE && position != origin)
    {
        MyVector direction = (position - origin).Normalize();
        position = position + direction * (float)(35 * dt);
    }
    if (state == TAKE_ORDER)
    {
        if (servingOrder[serving_num] != -1)
        {
            if (position != waypoints[servingOrder[serving_num]] && !collect)
            {
                MyVector direction = (position - waypoints.at(servingOrder[serving_num])).Normalize();
                position = position + direction * (float)(35 * dt);
            }
            //else if (position != waypoints[servingOrder[serving_num]] == false)
            //{
                //collect = true; // should be in waiter/chef cuz chef cook liao then can collect
                //servingOrder[serving_num] = -1;
            //}
        }
    }

    if (state == COLLECT)
    {
        if (position != collectArea)
        {
            MyVector direction = (position - collectArea).Normalize();
            position = position + direction * (float)(35 * dt);
        }
        else
        {
            collect = false;
        }
    }

    if (state == SERVE)
    {
        if (position != waypoints[servingOrder[serving_num]])
        {
            MyVector direction = (position - waypoints.at(servingOrder[serving_num])).Normalize();
            float speed;
            if (goFast == false)
                speed = (float)(35 * dt);
            else
                speed = (float)(50 * dt);
            position = position + direction * speed;
        }
        if (position != waypoints[servingOrder[serving_num]] == false)
        {
            ++serving_num;
            if (serving_num >= num_serve)
                serving_num = 0;
            state = TAKE_ORDER; // i broke my dont change state inside Update and only inside runfsm rule, can be easily solved using another bool i think
        //:(
        }
    }
}

string Waiter::stateToString(Waiter::WaiterStates state)
{
    switch (state)
    {
    case ARRANGE:
        return "Arranging";
        break;
    case IDLE:
        return "Idle";
        break;
    case TAKE_ORDER:
        return "Take Order";
        break;
    case COLLECT:
        return "Collecting";
        break;
    case SERVE:
        return "Serving";
        break;
    }
}

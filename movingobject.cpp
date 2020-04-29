#include "movingobject.h"
#include <QtGlobal>
#include <math.h>

MovingObject::MovingObject() :
    x_coord(360), y_coord(240), x_velocity(900), y_velocity(900)
{
    qsrand(time(0));
    int randval = qrand() % 501;
    x_velocity = randval + 400;
    y_velocity = 900 - randval;

    int x_vel = x_velocity;
    int y_vel = y_velocity;

    switch(qrand() % 4)
    {
    case 0:
        break;
    case 1:
        x_velocity = -x_velocity;
        break;
    case 2:
        y_velocity = -y_velocity;
        break;
    case 3:
        x_velocity = -x_velocity;
        y_velocity = -y_velocity;
        break;
    default:
        perror("qrand() betrayed us.");
    }

    x_timer.setTimerSize(x_vel);
    x_timer.setTokenTime(10);
    x_timer.resetTimer();

    y_timer.setTimerSize(y_vel);
    y_timer.setTokenTime(10);
    y_timer.resetTimer();
}

MovingObject::MovingObject(int _x_coord, int _y_coord, int _x_velocity, int _y_velocity) :
    x_coord(_x_coord), y_coord(_y_coord), x_velocity(_x_velocity), y_velocity(_y_velocity)
{
    x_timer.setTimerSize(x_velocity);
    x_timer.setTokenTime(1);
    x_timer.resetTimer();

    y_timer.setTimerSize(y_velocity);
    y_timer.setTokenTime(1);
    y_timer.resetTimer();
}

void MovingObject::collideWall()
{
    y_velocity = -y_velocity;
}

pair<int,int> MovingObject::getLocation()
{
    return make_pair(x_coord, y_coord);
}

void MovingObject::setLocation(int x, int y)
{
    x_coord = x;
    y_coord = y;
}

int MovingObject::getXVelocity()
{
    return x_velocity;
}

void MovingObject::setXVelocity(int x_vel)
{
    x_velocity = x_vel;
}

int MovingObject::getYVelocity()
{
    return y_velocity;
}

void MovingObject::setYVelocity(int y_vel)
{
    y_velocity = y_vel;
}

void MovingObject::move()
{
    if(x_timer.processTimer()){
        if(x_velocity > 0)
        {
            x_coord++;
        }
        else
        {
            x_coord--;
        }
    }
    if(y_timer.processTimer()){
        if(y_velocity > 0)
        {
            y_coord++;
        }
        else
        {
            y_coord--;
        }
    }
}

#include "movingobject.h"
#include <math.h>

MovingObject::MovingObject() :
    x_coord(360), y_coord(240), /*angle(35), velocity(5)*/x_velocity(20), y_velocity(20)
{
    x_timer.setTimerSize(x_velocity);
    x_timer.setTokenTime(1);
    x_timer.resetTimer();

    y_timer.setTimerSize(y_velocity);
    y_timer.setTokenTime(1);
    y_timer.resetTimer();
}

MovingObject::MovingObject(int _x_coord, int _y_coord, /*int _angle, int _velocity*/int _x_velocity, int _y_velocity) :
    x_coord(_x_coord), y_coord(_y_coord), /*angle(_angle), velocity(_velocity)*/x_velocity(_x_velocity), y_velocity(_y_velocity)
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

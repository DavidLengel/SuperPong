#include "movingobject.h"
#include <math.h>

MovingObject::MovingObject() :
    x_coord(360), y_coord(240), angle(35), velocity(5) {}

MovingObject::MovingObject(int _x_coord, int _y_coord, int _angle, int _velocity) :
    x_coord(_x_coord), y_coord(_y_coord), angle(_angle), velocity(_velocity) {}

void MovingObject::collideWall()
{

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

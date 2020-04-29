#include "powerup.h"

Powerup::Powerup():MovingObject()
{}

Powerup::Powerup(int _x_coord, int _y_coord, /*int _angle, int _velocity*/int _x_velocity, int _y_velocity) :
    MovingObject(_x_coord, _y_coord, /*_angle, _velocity*/_x_velocity, _y_velocity)
{}

void Powerup::collidePaddle()
{

}

void Powerup::collideGoal()
{
    x_velocity = -x_velocity;
}

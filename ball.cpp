#include "ball.h"

Ball::Ball():MovingObject()
{}

Ball::Ball(int _x_coord, int _y_coord, int _angle, int _velocity) :
    MovingObject(_x_coord, _y_coord, _angle, _velocity)
{}

void Ball::collidePaddle() {

}

void Ball::collideGoal() {

}

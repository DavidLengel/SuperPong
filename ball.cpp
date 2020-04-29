#include "ball.h"
#include "mainwindow.h"

Ball::Ball():MovingObject()
{}

Ball::Ball(int _x_coord, int _y_coord, int _x_velocity, int _y_velocity) :
    MovingObject(_x_coord, _y_coord, _x_velocity, _y_velocity)
{}

void Ball::collidePaddle() {
    x_velocity = -x_velocity;
}

void Ball::collideGoal() {

}

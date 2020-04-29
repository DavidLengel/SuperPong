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

void Ball::increaseSpeed() {

    if(x_velocity > 1) {
        --x_velocity;
        x_timer.setTimerSize(x_velocity);
    }
    else if(x_velocity < -1) {
        ++x_velocity;
        x_timer.setTimerSize(std::abs(x_velocity));
    }

    if(y_velocity > 1) {
        --y_velocity;
        y_timer.setTimerSize(y_velocity);
    }
    else if(y_velocity < -1) {
        ++y_velocity;
        y_timer.setTimerSize(std::abs(y_velocity));
    }
    //cout << "xv: " << x_velocity << " yv: " << y_velocity << endl;
}

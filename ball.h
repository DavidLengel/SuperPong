#ifndef BALL_H
#define BALL_H

#include "movingobject.h"

class Ball : public MovingObject
{
public:
    Ball();
    Ball(int,int,int,int);
    void collidePaddle() override;
    void collideGoal() override;
};

#endif // BALL_H

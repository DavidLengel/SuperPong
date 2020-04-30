#ifndef POWERUP_H
#define POWERUP_H

#include "movingobject.h"

class Powerup : public MovingObject
{
public:
    Powerup();
    Powerup(int,int,int,int);
    void collidePaddle() override;
    void collideGoal() override;
};

#endif // POWERUP_H

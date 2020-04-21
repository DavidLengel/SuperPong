#ifndef POWERUP_H
#define POWERUP_H

#include "movingobject.h"

class powerup : public MovingObject
{
public:
    powerup();
    powerup(int,int,int,int);
    void collidePaddle() override;
    void collideGoal() override;
};

#endif // POWERUP_H

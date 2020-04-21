#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include <utility>
#include "tokentimer.h"

using namespace std;

class MovingObject
{
    protected:
        // current x-positions
        int x_coord;
        // current y-position
        int y_coord;
        // speed that object is traveling horizontally
        int x_velocity;
        // speed that object is traveling vertically
        int y_velocity;
        // 0-359 representing the degree of the angle to move at
        //int angle;
        // number of milliseconds per move
        //int velocity;
        TokenTimer x_timer;
        TokenTimer y_timer;

    public:
        MovingObject();
        MovingObject(int,int,int,int);
        void collideWall();
        virtual void collidePaddle() = 0;
        virtual void collideGoal() = 0;
        pair<int,int> getLocation();
        void setLocation(int,int);
        int getXVelocity();
        void setXVelocity(int);
        int getYVelocity();
        void setYVelocity(int);
        void move();
};

#endif // MOVINGOBJECT_H

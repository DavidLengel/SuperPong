#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include <utility>
#include "tokentimer.h"

using namespace std;

class MovingObject
{
    private:
        // current x-positions
        int x_coord;
        // current y-position
        int y_coord;
        // 0-359 representing the degree of the angle to move at
        int angle;
        // number of milliseconds per move
        int velocity;
        TokenTimer timer;

    public:
        MovingObject();
        MovingObject(int,int,int,int);
        void collideWall();
        virtual void collidePaddle() = 0;
        virtual void collideGoal() = 0;
        pair<int,int> getLocation();
        void setLocation(int,int);

};

#endif // MOVINGOBJECT_H

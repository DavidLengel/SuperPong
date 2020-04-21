#ifndef PADDLE_H
#define PADDLE_H

class Paddle
{
    protected:
        int y_coord;
    public:
        Paddle();
        Paddle(int);
        int getYLocation();
        void setYLocation(int);
};

#endif // PADDLE_H

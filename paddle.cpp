#include "paddle.h"

Paddle::Paddle() : y_coord(200)
{}

Paddle::Paddle(int _y_coord) : y_coord(_y_coord)
{}

int Paddle::getYLocation()
{
    return y_coord;
}

void Paddle::setYLocation(int y)
{
    y_coord = y;
}

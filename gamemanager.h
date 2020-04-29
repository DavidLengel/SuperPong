#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "powerup.h"
#include "paddle.h"
#include <unistd.h>
#include <pthread.h>

class GameManager
{
public:
    GameManager();
    int run(MainWindow&);
};

#endif // GAMEMANAGER_H

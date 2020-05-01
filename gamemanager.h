#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "powerup.h"
#include <unistd.h>
#include <pthread.h>

#define SPEED_OFFSET 15

class GameManager
{
public:
    GameManager();
    int run(MainWindow&);
    TokenTimer powerup_spawn_timer;
    TokenTimer powerup_active_timer;
};

#endif // GAMEMANAGER_H

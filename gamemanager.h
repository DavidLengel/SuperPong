#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "powerup.h"
#include <unistd.h>
#include <pthread.h>

class GameManager
{
public:
    GameManager();
    int run(MainWindow&);
    TokenTimer pup_spawn_timer;
    TokenTimer pup_active_timer;
};

#endif // GAMEMANAGER_H

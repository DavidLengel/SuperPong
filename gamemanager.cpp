#include "gamemanager.h"
#include "mainwindow.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int gameNumber = 1;

GameManager::GameManager()
{

}

int GameManager::run()
{
    qsrand(time(0));
    cout << "Game " << gameNumber << " Started!" << endl;
    sleep(5);
    cout << "Game " << gameNumber++ << " Ended!" << endl;

    return (qrand() % (2) + 1);
}

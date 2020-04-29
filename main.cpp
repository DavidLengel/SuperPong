//#include "mainwindow.h"
//#include "message.h"
//#include "ball.h"
//#include "paddle.h"
//#include <unistd.h>
//#include <pthread.h>
#include "gamemanager.h"

#include <QApplication>

void *game_thread_fn(void *);

//static const int sleep_time = 5000;

//typedef struct thread_arguments
//{
//    Message<Ball> *ballMessage_p;
//    Message<int> *gameMessage_p;
//} thread_arguments_t;

//static thread_arguments_t arguments;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    pthread_t game_thread;
    //cout << "Game returned " << gm.run() << endl;

    if(pthread_create(&game_thread, NULL, game_thread_fn, &w) != 0)
    {
        perror("Play thread creation failed.");
        exit(1);
    }

    a.exec();

    pthread_join(game_thread, NULL);

    cout << "Returned to main" << endl;

    return 0;
}

void *game_thread_fn(void *args)
{
    MainWindow *w = (MainWindow *)args;

    GameManager gm;

    while(1) {
        cout << "Starting a game" << endl;
        int winner = gm.run(*w);
        cout << "Winner: " << winner << endl;
    }

    pthread_exit(NULL);
}

//void *thread_consumer_fn(void *args)
//{
//    MainWindow *window = (MainWindow *)args;

//    Message<Ball> *ballMessage = arguments.ballMessage_p;
//    Message<int> *gameMessage = arguments.gameMessage_p;

//    Ball ball;
//    int winner;

//    while(true) {
//        if(ballMessage->getMessage(ball))
//        {
//            window->moveBall(ball.getLocation().first, ball.getLocation().second);
//        }
//        if(gameMessage->getMessage(winner))
//        {
//            window->gameOver(winner);
//            pthread_exit(NULL);
//        }
//        usleep(sleep_time);
//    }
//}

#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "paddle.h"
#include "gamemanager.h"
#include <unistd.h>
#include <pthread.h>

#include <QApplication>

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);
void *thread_test_fn(void *);

static const int sleep_time = 5000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    Message<int> *gameMessage_p;
} thread_arguments_t;

static thread_arguments_t arguments;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    pthread_t thread_test;
    if(pthread_create(&thread_test, NULL, thread_test_fn, &w) != 0)
    {
        perror("Thread test creations failed.");
        exit(1);
    }

    // threads
    //pthread_t thread_consumer, thread_producer;
//    Message<Ball> ballMessage;
//    Message<int> gameMessage;
//    arguments.ballMessage_p = &ballMessage;
//    arguments.gameMessage_p = &gameMessage;

//    if(pthread_create(&thread_producer, NULL, thread_producer_fn, &w) != 0)
//    {
//        perror("Thread producer creation failed.");
//        exit(1);
//    }
//    if(pthread_create(&thread_consumer, NULL, thread_consumer_fn, &w) != 0)
//    {
//        perror("Thread consumer creation failed.");
//        exit(1);
//    }


    return a.exec();
}

void *thread_test_fn(void *args)
{
    MainWindow *w = (MainWindow *)args;

    int matchInSession = true;
    int winner;
    int maxScore = 7;
    int p1Score = 0;
    int p2Score = 0;

    cout << "Match Started!" << endl;

    while (matchInSession){
        GameManager *gm = new GameManager();
        winner = gm->run();

        if(winner == 1)
            p1Score++;
        else
            p2Score++;

        w->gameOver(winner);

        if(p1Score >= maxScore || p2Score >= maxScore)
        {
            matchInSession = false;
            w->matchOver(winner);
            cout << "Match Ended!" << endl;
        }
        else
        {
            w->nextGame();
        }
        free(gm);
    }

    pthread_exit(NULL);
}

void *thread_producer_fn(void *args)
{
    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<int> *gameMessage = arguments.gameMessage_p;

    Ball ball(100, 50, 10, 10);
    Paddle leftPaddle(0);
    Paddle rightPaddle(1);
    int lastWallCollided = 0;
    int lastPaddleCollided = 0;

    //char printme = 'a';
    while(true) {
        // update ball variables
        ball.move();
        ballMessage->putMessage(ball);

        // check wall collision
        int currentWallCollision = window->checkWallCollision();
        if (lastWallCollided != currentWallCollision && currentWallCollision != 0)
        {
            ball.collideWall();
            lastWallCollided = currentWallCollision;
        }
        int currentPaddleCollision = window->checkPaddleCollision();
        if (lastPaddleCollided != currentPaddleCollision && currentPaddleCollision != 0)
        {
            ball.collidePaddle();
            lastPaddleCollided = currentPaddleCollision;
        }
        int collidedGoal = window->checkGoalCollision();
        if (collidedGoal != 0)
        {
            gameMessage->putMessage(collidedGoal);
            pthread_exit(NULL);
        }

        //usleep(sleep_time);
    }
}

void *thread_consumer_fn(void *args)
{
    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<int> *gameMessage = arguments.gameMessage_p;

    Ball ball;
    int winner;

    while(true) {
        if(ballMessage->getMessage(ball))
        {
            window->moveBall(ball.getLocation().first, ball.getLocation().second);
        }
        if(gameMessage->getMessage(winner))
        {
            window->gameOver(winner);
            pthread_exit(NULL);
        }
        usleep(sleep_time);
    }
}

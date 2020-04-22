#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "paddle.h"
#include <unistd.h>
#include <pthread.h>

#include <QApplication>

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 5000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    //Message<Paddle> *leftPaddleMessage_p;
    //Message<Paddle> *rightPaddleMessage_p;
    Message<int> *gameMessage_p;
} thread_arguments_t;

static thread_arguments_t arguments;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<Ball> ballMessage;
    //Message<Paddle> leftPaddleMessage;
    //Message<Paddle> rightPaddleMessage;
    Message<int> gameMessage;
    arguments.ballMessage_p = &ballMessage;
    //arguments.leftPaddleMessage_p = &leftPaddleMessage;
    //arguments.rightPaddleMessage_p = &rightPaddleMessage;
    arguments.gameMessage_p = &gameMessage;

    if(pthread_create(&thread_producer, NULL, thread_producer_fn, &w) != 0)
    {
        perror("Thread producer creation failed.");
        exit(1);
    }
    if(pthread_create(&thread_consumer, NULL, thread_consumer_fn, &w) != 0)
    {
        perror("Thread consumer creation failed.");
        exit(1);
    }

    return a.exec();
}

void *thread_producer_fn(void *args)
{
    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    //Message<Paddle> *leftPaddleMessage = arguments.leftPaddleMessage_p;
    //Message<Paddle> *rightPaddleMessage = arguments.rightPaddleMessage_p;
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

        // update left paddle variables
        // int new_y = rand() % 100;
        // leftPaddle.setYLocation(new_y);
        // leftPaddleMessage->putMessage(leftPaddle);
        // update right paddle variables
        // new_y = rand() % 100;
        // rightPaddle.setYLocation(new_y);
        // rightPaddleMessage->putMessage(rightPaddle);

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
    //Message<Paddle> *leftPaddleMessage = arguments.leftPaddleMessage_p;
    //Message<Paddle> *rightPaddleMessage = arguments.rightPaddleMessage_p;
    Message<int> *gameMessage = arguments.gameMessage_p;

    Ball ball;
    //Paddle leftPaddle;
    //Paddle rightPaddle;
    int winner;

    while(true) {
        if(ballMessage->getMessage(ball))
        {
            //cout << "Received (X: " << ball.getLocation().first << ", Y: " << ball.getLocation().second << ", X_Vel: " <<
                    //ball.getXVelocity() << ", Y_Vel: " << ball.getYVelocity() << endl;
            window->moveBall(ball.getLocation().first, ball.getLocation().second);
        }
        //if(leftPaddleMessage->getMessage(leftPaddle))
        //{
        //    //cout << "Received (" << leftPaddle.getYLocation() << ")." << endl;
        //    window->moveLeftPaddle(leftPaddle.getYLocation());
        //}
        //if(rightPaddleMessage->getMessage(rightPaddle))
        //{
        //    //cout << "Received (" << rightPaddle.getYLocation() << ")." << endl;
        //    window->moveRightPaddle(rightPaddle.getYLocation());
        //}
        if(gameMessage->getMessage(winner))
        {
            window->gameOver(winner);
            pthread_exit(NULL);
        }
        usleep(sleep_time);
    }
}

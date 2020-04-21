#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include "paddle.h"
#include <unistd.h>
#include <pthread.h>

#include <QApplication>

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 1000000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    Message<Paddle> *leftPaddleMessage_p;
    Message<Paddle> *rightPaddleMessage_p;
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
    Message<Paddle> leftPaddleMessage;
    Message<Paddle> rightPaddleMessage;
    arguments.ballMessage_p = &ballMessage;
    arguments.leftPaddleMessage_p = &leftPaddleMessage;
    arguments.rightPaddleMessage_p = &rightPaddleMessage;

    if(pthread_create(&thread_producer, NULL, thread_producer_fn, NULL) != 0)
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
    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<Paddle> *leftPaddleMessage = arguments.leftPaddleMessage_p;
    Message<Paddle> *rightPaddleMessage = arguments.rightPaddleMessage_p;

    Ball ball;
    Paddle leftPaddle(0);
    Paddle rightPaddle(1);

    //char printme = 'a';
    while(true) {
        // update ball variables
        int new_x = rand() % 100;
        int new_y = rand() % 100;
        ball.setLocation(new_x, new_y);
        ballMessage->putMessage(ball);
        // update left paddle variables
        new_y = rand() % 100;
        leftPaddle.setYLocation(new_y);
        leftPaddleMessage->putMessage(leftPaddle);
        // update right paddle variables
        new_y = rand() % 100;
        rightPaddle.setYLocation(new_y);
        rightPaddleMessage->putMessage(rightPaddle);

        usleep(sleep_time);
    }
}

void *thread_consumer_fn(void *args)
{
    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<Paddle> *leftPaddleMessage = arguments.leftPaddleMessage_p;
    Message<Paddle> *rightPaddleMessage = arguments.rightPaddleMessage_p;

    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;

    while(true) {
        if(ballMessage->getMessage(ball))
        {
            cout << "Received (" << ball.getLocation().first << ", " << ball.getLocation().second << ")." << endl;
            window->moveBall(ball.getLocation().first, ball.getLocation().second);
        }
        if(leftPaddleMessage->getMessage(leftPaddle))
        {
            cout << "Received (" << leftPaddle.getYLocation() << ")." << endl;
            window->moveLeftPaddle(leftPaddle.getYLocation());
        }
        if(rightPaddleMessage->getMessage(rightPaddle))
        {
            cout << "Received (" << rightPaddle.getYLocation() << ")." << endl;
            window->moveRightPaddle(rightPaddle.getYLocation());
        }
        usleep(sleep_time);
    }
}

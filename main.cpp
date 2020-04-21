#include "mainwindow.h"
#include "message.h"
#include "ball.h"
#include <unistd.h>
#include <pthread.h>

#include <QApplication>

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 1000000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
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
    arguments.ballMessage_p = &ballMessage;

    if(pthread_create(&thread_producer, NULL, thread_producer_fn, NULL) != 0)
    {
        perror("Thread producer creation failed.");
        exit(1);
    }
    if(pthread_create(&thread_consumer, NULL, thread_consumer_fn, NULL) != 0)
    {
        perror("Thread consumer creation failed.");
        exit(1);
    }

    return a.exec();
}

void *thread_producer_fn(void *args)
{
    Message<Ball> *ballMessage = arguments.ballMessage_p;

    Ball ball;

    //char printme = 'a';
    while(true) {
        // update ball variables
        int new_x = rand() % 100;
        int new_y = rand() % 100;
        ball.setLocation(new_x, new_y);
        ball.setXVelocity(2);
        ball.setYVelocity(2);
        ballMessage->putMessage(ball);

        usleep(sleep_time);
    }
}

void *thread_consumer_fn(void *args)
{
    Message<Ball> *ballMessage = arguments.ballMessage_p;

    Ball ball;

    while(true) {
        if(ballMessage->getMessage(ball))
        {
            cout << "Received (X: " << ball.getLocation().first << ", Y: " << ball.getLocation().second << ", X_Vel: " <<
                    ball.getXVelocity() << ", Y_Vel: " << ball.getYVelocity() << endl;

        }
        usleep(sleep_time);
    }
}

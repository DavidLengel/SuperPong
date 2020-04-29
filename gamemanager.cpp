#include "gamemanager.h"
#include "mainwindow.h"
#include <unistd.h>
#include <iostream>

using namespace std;

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 5000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    Message<Powerup> * powerupMessage_p;
    MainWindow *w_p;
    bool gameActive;
    int winner;
} thread_arguments_t;

GameManager::GameManager()
{

}

int GameManager::run(MainWindow& w)
{

    thread_arguments_t arguments;

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<Ball> ballMessage;
    Message<Powerup> powerupMessage;
    Message<int> gameMessage;
    arguments.ballMessage_p = &ballMessage;
    arguments.powerupMessage_p = &powerupMessage;
    arguments.w_p = &w;
    arguments.gameActive = true;
    arguments.winner = 0;

    if(pthread_create(&thread_producer, NULL, thread_producer_fn, &arguments) != 0)
    {
        perror("Thread producer creation failed.");
        exit(1);
    }
    if(pthread_create(&thread_consumer, NULL, thread_consumer_fn, &arguments) != 0)
    {
        perror("Thread consumer creation failed.");
        exit(1);
    }

    pthread_join(thread_producer, NULL);
    pthread_join(thread_consumer, NULL);

    return arguments.winner;
}

void *thread_producer_fn(void *args)
{
    thread_arguments_t *arguments = (thread_arguments_t *)args;

    Message<Ball> *ballMessage = arguments->ballMessage_p;
    Message<Powerup> *powerupMessage = arguments->powerupMessage_p;
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;
    int *winner = &arguments->winner;

    Ball ball;
    Powerup powerup;
    Paddle leftPaddle(0);
    Paddle rightPaddle(1);
    int lastWallCollided = 0;
    int lastPaddleCollided = 0;
    //int lastSpeedSetting = window->checkSelectedGameSpeed();

    window->moveBall(ball.getLocation().first, ball.getLocation().second);
    window->movePowerup(powerup.getLocation().first, powerup.getLocation().second, 0);

    int increase_speed = 0;
    int when_increase_speed = 1000000;

    while(*gameActive) {
        // update ball variables
        ball.move();
        powerup.move();

        // increase speed gradually
        if(increase_speed == when_increase_speed-1) {
            ball.increaseSpeed();
        }
        increase_speed = (increase_speed+1)%when_increase_speed;

        ballMessage->putMessage(ball);
        powerupMessage->putMessage(powerup);

        // check if ball had a wall collision
        int currentBallWallCollision = window->checkBallWallCollision();
        if (lastWallCollided != currentBallWallCollision && currentBallWallCollision != 0)
        {
            ball.collideWall();
            lastWallCollided = currentBallWallCollision;
        }
        // check if ball had a paddle collision
        int currentBallPaddleCollision = window->checkBallPaddleCollision();
        if (lastPaddleCollided != currentBallPaddleCollision && currentBallPaddleCollision != 0)
        {
            ball.collidePaddle();
            lastPaddleCollided = currentBallPaddleCollision;
        }
        // check if ball had a goal collision
        int ballCollidedGoal = window->checkBallGoalCollision();
        if (ballCollidedGoal != 0)
        {
            *winner = (ballCollidedGoal == 1) ? 2 : 1;
            *gameActive = false;
            window->gameOver(*winner);
        }

        // check if powerup had a wall collision
        int currentPowerupWallCollision = window->checkBallWallCollision();
        if (lastWallCollided != currentPowerupWallCollision && currentPowerupWallCollision != 0)
        {
            powerup.collideWall();
            lastWallCollided = currentPowerupWallCollision;
        }
        // check if powerup had a paddle collision
        int currentPowerupPaddleCollision = window->checkBallPaddleCollision();
        if (lastPaddleCollided != currentPowerupPaddleCollision && currentPowerupPaddleCollision != 0)
        {
            powerup.collidePaddle();
        }
        // check if powerup had a goal collision
        int powerupCollidedGoal = window->checkBallGoalCollision();
        if (powerupCollidedGoal != 0)
        {
            powerup.collideGoal();
        }

//        int speed = window->checkSelectedGameSpeed();

//        if (speed != lastSpeedSetting)
//        {
//            if (ball.getXVelocity() >= 0)
//            {
//                ball.setXVelocity(ball.getXVelocity() + speed);
//            }
//            else
//            {
//                ball.setXVelocity(ball.getXVelocity() - speed);
//            }

//            if (ball.getYVelocity() >= 0)
//            {
//                ball.setYVelocity(ball.getYVelocity() + speed);
//            }
//            else
//            {
//                ball.setYVelocity(ball.getYVelocity() - speed);
//            }

//            lastSpeedSetting = speed;
//        }
    }
    usleep(100);
    pthread_exit(NULL);
}

void *thread_consumer_fn(void *args)
{
    thread_arguments_t *arguments = (thread_arguments_t *)args;

    Message<Ball> *ballMessage = arguments->ballMessage_p;
    Message<Powerup> * powerupMessage = arguments->powerupMessage_p;
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;

    Ball ball;
    Powerup powerup;

    while(*gameActive) {

        if(ballMessage->getMessage(ball))
        {
            window->moveBall(ball.getLocation().first, ball.getLocation().second);
        }
        if(powerupMessage->getMessage(powerup))
        {
            window->movePowerup(powerup.getLocation().first, powerup.getLocation().second, 0);
        }
        usleep(sleep_time);
    }
    pthread_exit(NULL);
}

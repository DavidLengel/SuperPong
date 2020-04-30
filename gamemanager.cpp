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
    MainWindow *w_p;
    bool gameActive;
    int winner;
    GameManager *game_manager;
    int powerup_state;
} thread_arguments_t;

GameManager::GameManager()
{
    // power-up spawn timer goes off after 10 seconds
    pup_spawn_timer.setTimerSize(10);
    pup_spawn_timer.setTokenTime(1000000);
    pup_spawn_timer.resetTimer();   // start this timer immediately, runs for entirety of game

    // power-up active timer goes off after 5 seconds
    pup_active_timer.setTimerSize(5);
    pup_active_timer.setTokenTime(1000000);
}

int GameManager::run(MainWindow& w)
{
    thread_arguments_t arguments;

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<Ball> ballMessage;
    Message<int> gameMessage;
    arguments.ballMessage_p = &ballMessage;
    arguments.w_p = &w;
    arguments.gameActive = true;
    arguments.winner = 0;
    arguments.game_manager = this;
    arguments.powerup_state = 1;

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
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;
    int *winner = &arguments->winner;
    GameManager *game_manager = arguments->game_manager;
    int *powerup_state = &arguments->powerup_state;

    Ball ball;
    int lastWallCollided = 0;
    int lastPaddleCollided = 0;
    //int lastSpeedSetting = window->checkSelectedGameSpeed();

    window->moveBall(ball.getLocation().first, ball.getLocation().second);

    int increase_speed = 0;
    int when_increase_speed = 1000000;

    while(*gameActive) {

        // powerups
        if(game_manager->pup_spawn_timer.processTimer()) {
            cout << "10 seconds passed." << endl;
            int state = *powerup_state;
            switch(state)
            {
                case 0:
                    cout << "Powerup state 0" << endl;
                    break;
                case 1:
                    cout << "Powerup state 1" << endl;
                    break;
                case 2:
                    cout << "Powerup state 2" << endl;
                    break;
                case 3:
                    cout << "Powerup state 3" << endl;
                    break;
                case 4:
                    cout << "Powerup state 4" << endl;
                    break;
                case 5:
                    cout << "Powerup state 5" << endl;
                    break;
                case 6:
                    cout << "Powerup state 6" << endl;
            }
            *powerup_state = (state+1)%7;
        }

        // update ball variables
        ball.move();

        // increase speed gradually
        if(increase_speed == when_increase_speed-1) {
            ball.increaseSpeed();
        }
        increase_speed = (increase_speed+1)%when_increase_speed;

        ballMessage->putMessage(ball);

        // check wall collision
        int currentWallCollision = window->checkWallCollision();
        if (lastWallCollided != currentWallCollision && currentWallCollision != 0)
        {
            ball.collideWall();
            lastWallCollided = currentWallCollision;
        }
        // check paddle collision
        int currentPaddleCollision = window->checkPaddleCollision();
        if (lastPaddleCollided != currentPaddleCollision && currentPaddleCollision != 0)
        {
            ball.collidePaddle();
            lastPaddleCollided = currentPaddleCollision;
        }
        // check goal collision
        int collidedGoal = window->checkGoalCollision();
        if (collidedGoal != 0)
        {
            *winner = (collidedGoal == 1) ? 2 : 1;
            *gameActive = false;
            window->gameOver(*winner);
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
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;
    //int *powerup_state = &arguments->powerup_state;

    Ball ball;

    while(*gameActive) {

        if(ballMessage->getMessage(ball))
        {
            window->moveBall(ball.getLocation().first, ball.getLocation().second);
        }
        usleep(sleep_time);
    }
    pthread_exit(NULL);
}

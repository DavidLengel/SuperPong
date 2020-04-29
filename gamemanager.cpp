#include "gamemanager.h"

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 5000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    MainWindow *w_p;
    bool gameActive;
    int winner;
} thread_arguments_t;

//static thread_arguments_t arguments;

GameManager::GameManager()
{

}

int GameManager::run(MainWindow& w)
{
//    MainWindow w;
//    w.show();

    thread_arguments_t arguments;

    cout << "here" << endl;

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<Ball> ballMessage;
    Message<int> gameMessage;
    arguments.ballMessage_p = &ballMessage;
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
    cout << "Producer entered" << endl;

    //MainWindow *window = (MainWindow *)args;
    thread_arguments_t *arguments = (thread_arguments_t *)args;

//    Message<Ball> *ballMessage = arguments.ballMessage_p;
//    Message<int> *gameMessage = arguments.gameMessage_p;

    Message<Ball> *ballMessage = arguments->ballMessage_p;
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;
    int *winner = &arguments->winner;

    Ball ball(100, 50, 10, 10);
    Paddle leftPaddle(0);
    Paddle rightPaddle(1);
    int lastWallCollided = 0;
    int lastPaddleCollided = 0;

    while(*gameActive) {
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
            *winner = (collidedGoal == 1) ? 2 : 1;
            *gameActive = false;
            window->gameOver(*winner);
        }
        usleep(sleep_time);
    }
    pthread_exit(NULL);
}

void *thread_consumer_fn(void *args)
{
    cout << "Consumer entered" << endl;

    //MainWindow *window = (MainWindow *)args;

    thread_arguments_t *arguments = (thread_arguments_t *)args;

//    Message<Ball> *ballMessage = arguments.ballMessage_p;
//    Message<int> *gameMessage = arguments.gameMessage_p;

    Message<Ball> *ballMessage = arguments->ballMessage_p;
    MainWindow *window = arguments->w_p;
    bool *gameActive = &arguments->gameActive;

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

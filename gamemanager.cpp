#include "gamemanager.h"

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 5000;

typedef struct thread_arguments
{
    Message<Ball> *ballMessage_p;
    Message<int> *gameMessage_p;
} thread_arguments_t;

static thread_arguments_t arguments;

GameManager::GameManager()
{

}

int GameManager::run(MainWindow& w)
{
//    MainWindow w;
//    w.show();

    cout << "here" << endl;

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<Ball> ballMessage;
    Message<int> gameMessage;
    arguments.ballMessage_p = &ballMessage;
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

//    pthread_join(thread_producer, NULL);
//    pthread_join(thread_consumer, NULL);

    return 1;
}

void *thread_producer_fn(void *args)
{
    cout << "Producer entered" << endl;

    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<int> *gameMessage = arguments.gameMessage_p;

    Ball ball(100, 50, 10, 10);
    Paddle leftPaddle(0);
    Paddle rightPaddle(1);
    int lastWallCollided = 0;
    int lastPaddleCollided = 0;

    while(true) {
        cout << "Producer updating" << endl;
        // update ball variables
        ball.move();

        cout << "Producer here" << endl;

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
        cout << "Producer end of loop" << endl;
        //usleep(sleep_time);
    }
}

void *thread_consumer_fn(void *args)
{
    cout << "Consumer entered" << endl;

    MainWindow *window = (MainWindow *)args;

    Message<Ball> *ballMessage = arguments.ballMessage_p;
    Message<int> *gameMessage = arguments.gameMessage_p;

    Ball ball;
    int winner;

    while(true) {

        cout << "Consumer updating" << endl;

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

        cout << "Consumer end of loop" << endl;
    }
}

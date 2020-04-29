#include "gamemanager.h"

#include <QApplication>

void *game_thread_fn(void *);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    pthread_t game_thread;

    if(pthread_create(&game_thread, NULL, game_thread_fn, &w) != 0)
    {
        perror("Play thread creation failed.");
        exit(1);
    }

    a.exec();

    pthread_join(game_thread, NULL);

    return 0;
}

void *game_thread_fn(void *args)
{
    MainWindow *w = (MainWindow *)args;

    int matchInSession = true;
    int winner;
    int maxScore = 7;
    int p1Score = 0;
    int p2Score = 0;

    GameManager gm;

    while (matchInSession){
        winner = gm.run(*w);
        cout << "Winner: " << winner << endl;
        sleep(2);
        if(winner == 1)
            p1Score++;
        else
            p2Score++;

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
    }

    pthread_exit(NULL);
}

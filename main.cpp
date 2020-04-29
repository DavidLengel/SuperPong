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

    GameManager gm;

    for(int game_count = 0; game_count < 3; ++game_count) {
        cout << "Starting a game " << game_count+1 << endl;
        int winner = gm.run(*w);
        cout << "Winner: " << winner << endl;
        sleep(2);
    }

    pthread_exit(NULL);
}

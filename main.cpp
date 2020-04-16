#include "mainwindow.h"
#include "message.h"
#include <unistd.h>
#include <pthread.h>

#include <QApplication>

void *thread_producer_fn(void *);
void *thread_consumer_fn(void *);

static const int sleep_time = 1000000;

typedef struct thread_arguments
{
    Message<char> *charMessage_p;
} thread_arguments_t;

static thread_arguments_t arguments;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // threads
    pthread_t thread_consumer, thread_producer;
    Message<char> charMessage;
    arguments.charMessage_p = &charMessage;

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
    Message<char> *charMessage = arguments.charMessage_p;

    char printme = 'a';
    while(true) {
        charMessage->putMessage(printme);
        printme++;
        if(printme > 'z') {
            printme = 'a';
        }
        usleep(sleep_time);
    }
}

void *thread_consumer_fn(void *args)
{
    Message<char> *charMessage = arguments.charMessage_p;
    char messageHolder;


    while(true) {
        if(charMessage->getMessage(messageHolder))
        {
            cout << "Received " << messageHolder << endl;
        }
        usleep(sleep_time);
    }
}

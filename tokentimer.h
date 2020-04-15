#ifndef TOKENTIMER_H
#define TOKENTIMER_H

#include<iostream>

class TokenTimer
{
    private:
        // the max number of tokens that can fit in the timer
        int timer_size;
        // the number of milliseconds per token
        int token_time;
        // the number of tokens currently in the timer
        int token_count;
        // the time of the last token event in system time milliseconds
        timespec *last_token_event;

    public:
        TokenTimer();
        TokenTimer(int,int);
        ~TokenTimer();
        bool processTimer();
        void resetTimer();
        void setTimerSize(int);
        void setTokenTime(int);
};

#endif // TOKENTIMER_H

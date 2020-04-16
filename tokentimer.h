#ifndef TOKENTIMER_H
#define TOKENTIMER_H

#include<iostream>

class TokenTimer
{
    private:
        // the max number of tokens that can fit in the timer
        int full_token_count;
        // the number of milliseconds per token
        int msec_per_token;
        // the number of tokens currently in the timer
        int token_count;
        // the time of the last token event in system time
        timespec last_token_event;

        static const unsigned int msec_per_sec = 1000;
        static const unsigned int nsec_per_msec = 1000000;
        static const unsigned int nsec_per_sec = 100000000;

    public:
        TokenTimer();
        TokenTimer(int,int);
        ~TokenTimer();
        bool processTimer();
        bool tokenTimeElapsed();
        void resetTimer();
        void setTimerSize(int);
        void setTokenTime(int);
};

#endif // TOKENTIMER_H

#include "tokentimer.h"

//-----------------------------------------------------------
// Function: tokenTimeElapsed
//  Purpose: To check if the elapsed time from the last token
//              event has exceeded the token time.
//   Inputs: timespec of the last token event
//           int of the token time in milliseconds
//  Outputs: bool for if the elapsed time is greater than the
//              token time
//-----------------------------------------------------------
bool tokenTimeElapsed(timespec *last_token_event, int token_time)
{
    // get current time
    timespec *current_time = new timespec();
    clock_gettime(CLOCK_MONOTONIC, current_time);

    // get time elapsed between current time and last token event time in milliseconds
    int elapsedMS = 0;
    if(current_time->tv_sec == last_token_event->tv_sec)
        elapsedMS = (current_time->tv_nsec - last_token_event->tv_nsec) / 1000000;
    else
        elapsedMS = (last_token_event->tv_nsec + (999999999 - current_time->tv_nsec)) / 1000000;

    if(elapsedMS < token_time)
        return false;
    else
        return true;
}

TokenTimer::TokenTimer()
{
    timer_size = 10;
    token_time = 10;
    last_token_event = new timespec();
}

TokenTimer::TokenTimer(int _timer_size, int _token_time)
{
    timer_size = _timer_size;
    token_time = _token_time;
    last_token_event = new timespec();
}

TokenTimer::~TokenTimer()
{
    delete(last_token_event);
}

//-----------------------------------------------------------
// Function: processTimer
//  Purpose: To process the tokens in the current timer. The
//              steps are to remove a token and refill the
//              tokens if it is empty.
//   Inputs: void
//  Outputs: bool for if the timer had to be refilled
//-----------------------------------------------------------
bool TokenTimer::processTimer()
{
    // if a token time has not elapsed since the last token event, return false
    if(!tokenTimeElapsed(last_token_event, token_time))
        return false;
    // else
    // remove a token from the timer
    --token_count;
    // update the last token event time
    clock_gettime(CLOCK_MONOTONIC, last_token_event);
    // if the timer is not empty, return false
    if(token_count != 0)
        return false;
    // else if timer empty, refill the timer with tokens and return true
    token_count = timer_size;
    return true;
}

//-----------------------------------------------------------
// Function: resetTimer
//  Purpose: To refill the timer with tokens.
//   Inputs: void
//  Outputs: void
//-----------------------------------------------------------
void TokenTimer::resetTimer()
{
    // refill the timer
    token_count = timer_size;
    // set a last_token_event
    clock_gettime(CLOCK_MONOTONIC, last_token_event);
}

void TokenTimer::setTimerSize(int _timer_size)
{
    timer_size = _timer_size;
}

void TokenTimer::setTokenTime(int _token_time)
{
    token_time = _token_time;
}

#include "tokentimer.h"

TokenTimer::TokenTimer() :
    full_token_count(10), msec_per_token(1) {}

TokenTimer::TokenTimer(int _full_token_count, int _msec_per_token) :
    full_token_count(_full_token_count), msec_per_token(_msec_per_token) {}

TokenTimer::~TokenTimer() {}

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
    if(!tokenTimeElapsed())
        return false;
    // else
    // remove a token from the timer
    --token_count;
    // update the last token event time
    clock_gettime(CLOCK_MONOTONIC, &last_token_event);
    // if the timer is not empty, return false
    if(token_count > 0)
        return false;
    // else if timer empty, refill the timer with tokens and return true
    token_count = full_token_count;
    return true;
}

//-----------------------------------------------------------
// Function: tokenTimeElapsed
//  Purpose: To check if the elapsed time from the last token
//              event has exceeded the token time.
//   Inputs: timespec of the last token event
//           int of the token time in milliseconds
//  Outputs: bool for if the elapsed time is greater than the
//              token time
//-----------------------------------------------------------
bool TokenTimer::tokenTimeElapsed()
{
    // get current time
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    // get time elapsed between current time and last token event time in milliseconds
    int elapsedMS = 0;
    if(current_time.tv_sec == last_token_event.tv_sec)
        elapsedMS = (current_time.tv_nsec - last_token_event.tv_nsec) / nsec_per_msec;
    else
    {
        elapsedMS = (last_token_event.tv_nsec + (nsec_per_sec - current_time.tv_nsec)) / nsec_per_msec;
        elapsedMS += (current_time.tv_sec - last_token_event.tv_sec) * msec_per_sec;
    }

    return (elapsedMS >= msec_per_token);
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
    token_count = full_token_count;
    // set a last_token_event
    clock_gettime(CLOCK_MONOTONIC, &last_token_event);
}

void TokenTimer::setTimerSize(int _full_token_count)
{
    full_token_count = _full_token_count;
}

void TokenTimer::setTokenTime(int _msec_per_token)
{
    msec_per_token = _msec_per_token;
}

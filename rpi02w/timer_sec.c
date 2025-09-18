/**
 * timer_sec.h
 * 
 * This file contains function definitions for the timer_sec type.
 * The timer_sec type is a timer that has second precision.
 * 
 * Author(s): Richard Gale
 * Version: 1.0
 */

#include "timer_sec.h"

/**
 * This function returns a new instance of a timer.
 */
timer_sec* timer_sec_init(log* l)
{
    time_t err;

    /* Allocating memory to a new timer. */
    timer_sec* ts = (timer_sec*) malloc(sizeof(struct timer_sec_data));

    /* Start the timer. */
    if ((err = time(&(ts->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_sec_init(): Calander time is not "
                "available\n");
	    l->out(
                l->fs,
	            "ERROR: in function timer_sec_init(): Calander time is not "
                "available\n");
    }
    else if (ts->initial == NULL)
    {
        fsout(
                stderr,
                "ERROR: In function timer_sec_init(): time() did not store "
                "the time\n");
        l->out(
                l->fs,
                "ERROR: In function timer_sec_init(): time() did not store "
                "the time\n");
    }

    /* Returning the timer. */
    return ts;
}

/**
 * This function resets a timer_sec.
 */
void timer_sec_reinit(timer_sec* ts, log* l)
{
    time_t err;

    /* Start the timer. */
    if ((err = time(&(ts->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_sec_reinit(): Calander time is not "
                "available\n");
	    l->out(
                l->fs,
	            "ERROR: in function timer_sec_reinit(): Calander time is not "
                "available\n");
    }
    else if (ts->initial == NULL)
    {
        fsout(
                stderr,
                "ERROR: In function timer_sec_reinit(): time() did not store "
                "the time\n");
        l->out(
                l->fs,
                "ERROR: In function timer_sec_reinit(): time() did not store "
                "the time\n");
    }
}

/**
 * This function will return HAS_ELAPSED upon the timer_sec passed to it
 * having been timing for longer than the wait_time parameter passed to it,
 * otherwise it will return NOT_ELAPSED;
 */
enum timer_states timer_sec_alarm(timer_sec ts, long long wait_time, log* l)
{
    time_t err;

    /* Start the timer. */
    if ((err = time(&(ts.current)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_sec_alarm(): Calander time is not "
                "available\n");
	    l->out(
                l->fs,
	            "ERROR: in function timer_sec_alarm(): Calander time is not "
                "available\n");
    }
    else if (ts.initial == NULL)
    {
        fsout(
                stderr,
                "ERROR: In function timer_sec_alarm(): time() did not store "
                "the time\n");
        l->out(
                l->fs,
                "ERROR: In function timer_sec_alarm(): time() did not store "
                "the time\n");
    }

    /* Calculating the amount of elapsed time and storing it. */
    ts.elapsed = ts.current - ts.initial;
    /* Determining whether the timer has ended (the alarm should sound). */
    if (ts.elapsed >= wait_time)
    {
        /* Returning that the timer has ended (the alarm has gone off). */
        return HAS_ELAPSED;
    }

    /* Returning that the timer has yet to end (the alarm hasn't gone off). */ 
    return NOT_ELAPSED;
}

void timer_sec_term(timer_sec* ts)
{
    free(ts);
}

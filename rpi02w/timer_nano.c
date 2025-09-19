/**
 * timer_nano.h
 * 
 * This file contains function definitions for the timer_nano type.
 * The timer_nano type is a timer that has nanosecond precision.
 * 
 * Author(s): Richard Gale
 * Version: 1.1
 */

#include "timer_nano.h"

/**
 * This function returns a new instance of a timer.
 */
timer_nano* timer_nano_init(log* l)
{
    /* Allocating memory to a new timer. */
    timer_nano* tn = (timer_nano*) malloc(sizeof(struct timer_nano_data));

    /* Start the timer. */
    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_nano_init(): %s\n", strerror(errno));
	    l->out(
                l->fs,
	            "ERROR: in function timer_nano_init(): %s\n", strerror(errno));
    }

    /* Returning the timer. */
    return tn;
}

/**
 * This function resetn a timer_nano.
 */
void timer_nano_reinit(timer_nano* tn, log* l)
{
    /* Start the timer. */
    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_nano_reinit(): %s\n", strerror(errno));
	    l->out(
                l->fs,
	            "ERROR: in function timer_nano_reinit(): %s\n", strerror(errno));
    }
}

/**
 * This function will return true upon the timer_nano passed to it
 * has elapsed its waiting time parameter, otherwise it will return false.
 */
bool timer_nano_elapsed(timer_nano tn, long long wait_time, log* l)
{
    /* Start the timer. */
    if ((clock_gettime(CLOCK_REALTIME, &(&tn)->current) == -1))
    {
        /* An error occured. Print the error. */
	    fsout(
                stderr,
	            "ERROR: in function timer_nano_elapsed(): %s\n", strerror(errno));
	    l->out(
                l->fs,
	            "ERROR: in function timer_nano_elapsed(): %s\n", strerror(errno));
    }

    /* Calculate the amount of elapsed time and store it. */
    tn.elapsed.tv_sec  = tn.current.tv_sec  - tn.initial.tv_sec;
    tn.elapsed.tv_nsec = tn.current.tv_nsec - tn.initial.tv_nsec;
    
    /* Determine whether the timer has elapsed. */
    if ((tn.elapsed.tv_sec * NANOS_PER_SEC) + tn.elapsed.tv_nsec > wait_time)
    {
        /* Returning that the timer elapsed. */
        return true;
    }
    
    /* Return that the timer hass not yet elapsed. */
    return false;
}

/**
 * This function destroys/deallocates memory from a timer_nano.
 */
void timer_nano_term(timer_nano* tn)
{
    /* Deallocate memory. */
    free(tn);
}

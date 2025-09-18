/**
 * timer_nano.h
 * 
 * This file contains function definitions for the timer_nano type.
 * The timer_nano type is a timer that has nanosecond precision.
 * 
 * Author(s): Richard Gale
 * Version: 1.0
 */

#include "timer_nano.h"

/**
 * This function returns a new instance of a timer.
 */
timer_nano* timer_nano_init()
{
    char* output;   /* Output to filestream(s). */

    /* Allocating memory to a new timer. */
    timer_nano* tn = (timer_nano*) malloc(sizeof(timer_nano));

    /* Start the timer. */
    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    fsout(stderr, strfmt(output,
	                        "ERROR: in function timer_nano_init(): %s\n",
                            strerror(errno))
                );

        /* Exiting the program. */
        exit(EXIT_FAILURE);
    }

    /* Returning the timer. */
    return tn;
}

/**
 * This function resets a timer_nano.
 */
void timer_nano_reinit(timer_nano* tn)
{
    char* output;   /* Output to filestream(s). */

    /* Resetting the timer. */
    if ((clock_gettime(CLOCK_REALTIME, &(tn->initial)) == -1))
    {
        /* An error occured. Printing the error. */
	    print(stderr, strfmt(output,
                    "ERROR: in function timer_nano_reinit(): %s\n",
                    strerror(errno))
                );

        /* Exiting the program. */
        exit(EXIT_FAILURE);
    }
}

/**
 * This function will return true upon the timer_nano passed to it
 * having elapsed the wait_time parameter passed to it, otherwise it will
 * return false;
 */
bool timer_nano_elapsed(timer_nano tn, long long wait_time)
{
    char* output;   /* Output to filestream(s). */

    /* Storing the current time. */
    if ((clock_gettime(CLOCK_REALTIME, &(&tn)->current) == -1))
    {
        /* An error occured. Printing the error. */
	    print(stderr, strfmt(output,
	                        "ERROR: in function start_timer(): %s\n",
	                        strerror(errno))
                );

        /* Exiting the program. */
        exit(EXIT_FAILURE);
    }

    /* Calculating the amount of elapsed time and storing it. */
    tn.elapsed.tv_sec  = tn.current.tv_sec  - tn.initial.tv_sec;
    tn.elapsed.tv_nsec = tn.current.tv_nsec - tn.initial.tv_nsec;
    
    /* Determining whether the timer has ended (the alarm should sound). */
    if ((tn.elapsed.tv_sec * NANOS_PER_SEC) + tn.elapsed.tv_nsec > wait_time)
    {
        /* Returning that the timer has ended (the alarm has gone off). */
        return true;
    }

    /* Returning that the timer has yet to end (the alarm hasn't gone off). */ 
    return false;
}

/**
 * This function destroys/deallocates memory from a timer_nano.
 */
void timer_nano_term(timer_nano* tn)
{
    free(tn);
}

/**
 * timer_sec.h
 * 
 * This file contains function and type declarations for the timer_sec
 * data type.
 * The timer_sec type is a timer that has second precision.
 * 
 * Author(s): Richard Gale
 * Version: 1.1
 */

#ifndef TIMER_SEC_H
#define TIMER_SEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /* strerror() */
#include <stdarg.h>
#include <time.h>
#include <errno.h>

#include "utils.h"



/**
 * The timer data structure.
 */
typedef struct timer_sec_data
{
    time_t initial;
    time_t current;
    time_t elapsed;
} timer_sec;

/**
 * This function returns a new instance of a timer_sec.
 */
timer_sec* timer_sec_init(log* l);

/**
 * This function resets a timer_sec.
 */
void timer_sec_reinit(timer_sec* ts, log* l);

/**
 * This function will return HAS_ELAPSED upon the timer_sec passed to it
 * having been timing for longer than the wait_time parameter passed to it,
 * otherwise it will return NOT_ELAPSED;
 */
bool timer_sec_elapsed(timer_sec ts, long long wait_time, log* l);

/**
 * This function destroys/deallocates memory from a timer_sec.
 */
void timer_sec_term(timer_sec* ts);

#endif /* TIMER_SEC_H */

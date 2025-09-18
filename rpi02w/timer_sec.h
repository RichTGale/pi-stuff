/**
 * timer_sec.h
 * 
 * This file contains function and type declarations for the timer_sec
 * data type.
 * The timer_sec type is a timer that has second precision.
 * 
 * Author(s): Richard Gale
 * Version: 1.0
 */

#ifndef TIMER_NANO_H
#define TIMER_NANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strerror() */
#include <stdarg.h>
#include <time.h>
#include <errno.h>

#include "utils.h"

/**
 * The timer can either have elapsed or not have elapsed.
 */
enum timer_states 
{
    NOT_ELAPSED,
    HAS_ELAPSED
};

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
enum timer_states timer_sec_alarm(timer_sec ts, long long wait_time, log* l);

void timer_sec_term(timer_sec* ts);

#endif /* TIMER_SEC_H */

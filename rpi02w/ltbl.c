#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>

#include "../mycutils/mycutils.h"
#include "../clocks/timer_sec/timer_sec.h"

int main()
{
    log* l;                         /* Program log. */
    timer_sec* wait_timer;          /* Waits for a bit before the light turns off. */
    const unsigned WAIT_TIME = 10;  /* Time to wait before turning off the light. */
    const unsigned INF_LIGHT = 4;   /* Infrared light pin. */
    int val;                        /* Function return value. */

    /* Initialise the program log. */
    l = log_init("log-ltbl.txt");

    /* Initialise waiting timer. */
    wait_timer = timer_sec_init(l);

    /* Output welcome message. */
    l->out(l->fs, "Program started.\n");
    fsout(stdout, "Program started\n");

    /* Initialise pigpio. */
    if ((val = gpioInitialise()) == PI_INIT_FAILED)
    {
        printf("gpioInitialise returned PI_INIT_FAILED\n");
    }
    else
    {
        printf("pigpio intitialised successfully\n");
    }

    /* Set the pin mode for the infrared pin. */
    gpioSetMode(INF_LIGHT, PI_OUTPUT);

    /* Turn on the infrared light. */
    gpioWrite(INF_LIGHT, 1);

    while (!timer_sec_elapsed(*wait_timer, WAIT_TIME, l)) {}

    /* Turn off the infrared light. */
    gpioWrite(INF_LIGHT, 0);
    
    /* Close pigpio. */
    gpioTerminate();

    /* End the program. */
    exit(EXIT_SUCCESS);
}


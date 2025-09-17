#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pigpio.h>

#include "timer_nano.h"



void print_help()
{
    char* buf;

    print(stdout, strfmt(
                buf, 
                "USAGE: sensor [OPTIONS]\n\n"
                "OPTIONS:\n"
                "   -h  Print this help menu.\n"
                "   -t  Length of time (in hours) to run the program (default 1).\n"
                "\n"
                "EXAMPLE:\n"
                "   sudo ./sensor -t 5\n")
         );
}


int main(int argc, char** argv)
{
    timer_nano* frm_timer;          /* Timer for framerate. */
    const int FPS = 2;       /* The number of frames per second. */
    const long long int NPS = NANOS_PER_SEC; /* The number of nanoseconds per second. Defined in "timer_nano.h". */
    const int IR_SENSE = 27;             /* Pin number of the IR sensor. */
    const int IR_LIGHT = 4;
	const int REC_TIME = 15;   /* Seconds to wait after a detection. */
    int rec_frms;
    int frms_rec;
    bool run;                    /* Whether the program is running. */
    bool rec;                  /* Whether the program is recording. */
    char* buf;                     /* Buffer to make strings. */
    int err;                       /* An error code/id. */
    int gpio_val;                       /* The valjue read from the PIR sensor. */
    unsigned int run_time;             /* The number of hours to run the program. */
    long long int frm_cnt;  /* Counts how many frames have happened. */
    long long int npf;  /* The number of nanoseconds per frame. */

    /* Initialising some variables. */
    npf = NPS / FPS;
    rec_frms = FPS * REC_TIME;
    frms_rec = 0;
    run = true;
    rec = false;
    frm_cnt = 0;
    err = true;

    for (int i = 1; i < argc; i+=2)
    {
        if (!strcmp(argv[i], "-h"))
        {
            print_help();
            exit(EXIT_FAILURE);
        }
        else if (!strcmp(argv[i], "-t"))
        {
            run_time = atoi(argv[i+1]);
            err = 0;
        }
    }

    if (err)
	{
        print_help();
        exit(EXIT_FAILURE);
    }

    /* Printing welcome message. */
    
    print(stdout, strfmt(buf, "Program started. Will run for %d hours\n", run_time));


    /* Initialising pigpio. */
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "Error initialising pigpio\n");
        exit(EXIT_FAILURE);
    }

    /* Creating gpio input to read the e18-d80nk sensor. */
    if ((err = gpioSetMode(IR_SENSE, PI_INPUT)) != 0)
    {
        if (err == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }

    /* Creating gpio input to write the IR light. */
    if ((err = gpioSetMode(IR_LIGHT, PI_OUTPUT)) != 0)
    {
        if (err == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        gpioWrite(IR_LIGHT, 0);
    }

    rec = false;

    /* Initialising timers */
    frm_timer = timer_nano_init();


    /* Running the loop. */ 
    while (run)
    {
        /* Checking if it's time to run a frame. */
        if (timer_nano_alarm(*frm_timer, npf))
        {
            if ((gpio_val = gpioRead(IR_SENSE)) == 0)
            {
                print(stdout, strfmt(buf, "Motion detected\n"));
                gpioWrite(IR_LIGHT, 1);
                rec = true;
                frms_rec = 0;
            }
            else if (rec && gpio_val == 1 && frms_rec >= rec_frms)
            {
                gpioWrite(IR_LIGHT, 0);
                rec = false;
            }
            else if (rec && frms_rec < rec_frms)
            {
                frms_rec++;
            }
            else if (gpio_val == PI_BAD_GPIO)
            {
                print(stdout, strfmt(buf, "Error reading gpio: PI_BAD_GPIO\n"));
                exit(EXIT_FAILURE);
            }
            
            /* Recording this frame. */
            frm_cnt++;

            /* Checking if we should end the program. */
            if (frm_cnt >= FPS * 60 * 60 * run_time)
                run = false;

            /* Restarting the framerate timer. */
            timer_nano_reinit(frm_timer); 
        }
    } 

    gpioTerminate();

    exit(EXIT_SUCCESS);
}

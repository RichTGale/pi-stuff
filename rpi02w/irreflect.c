#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <pigpio.h>

#include "utils.h"
//#include "timer_nano.h"
#include "timer_sec.h"

void print_help()
{
    char* buf;

    fsout(
            stdout, 
            "USAGE:\n" 
            "   sensor [OPTIONS]\n\n"
            "OPTIONS:\n"
            "   -h  | --help        Print this help menu.\n"
            "         --fps         The number of frames per second."
            "         --run-time    Length of time (in hours) to run the program (default 24).\n"
            "         --inf-time    Length of time (in seconds) to turn on the ir light after motion was detected (default 15).\n\n"
            "EXAMPLE:\n"
            "   sensor --rec-time 30 --frm-time 10 --run-time 5\n\n"
            "NOTE: Needs to be run with root privelages (i.e sudo)\n");
}


int main(int argc, char** argv)
{
    log* l;
    timer_sec* frm_timer;                           /* Timer for framerate. */
    timer_sec* run_timer;                           /* Timer for program ending. */
    timer_sec* inf_timer;                           /* Timer for infrared light. */
//    const unsigned long long NPS = NANOS_PER_SEC;   /* The number of nanoseconds per second. Defined in "timer_nano.h". */
    const unsigned int INF_SENSE = 27;              /* gpio number of the IR sensor. */
    const unsigned int INF_LIGHT = 4;               /* gpio number of the IR light. */
    unsigned long long frm_time;                    /* The number of nanoseconds per frame. */
	unsigned long run_time;                         /* Program run time in hours. */
	unsigned int inf_time;                          /* Light up time in seconds. */
    unsigned int fps;                               /* The number of frames per second. */
    int val;                                        /* A value returned by a function. */
    bool run;                                       /* Whether the program is running. */
    bool inf;                                       /* Whether the infrared light is on. */

    l = log_init("irreflect-log.txt");
    frm_timer = timer_sec_init(l);
    run_timer = timer_sec_init(l);
    inf_timer = timer_sec_init(l);
    fps = 4;
    frm_time = /* NPS* / */ fps;
    run_time = 24;
    inf_time = 15;
    val = 0;
    run = true;
    inf = false;
    
    /* Printing welcome message. */
    fsout(stdout, "Program started. Will run for %d hours\n", run_time);

    for (int i = 1; i < argc; i+=2)
    {
        if (!strcmp(argv[i], "-h"))
        {
            print_help();
            exit(EXIT_FAILURE);
        }
        else if (!strcmp(argv[i], "--fps"))
        {
            frm_time = atoi(argv[i+1]);
            if (frm_time > ULLONG_MAX)
            {
                l->out(l->fs, "ARG ERROR: --frm-time must be in range 0 --> %d.\n", UINT_MAX);
                fsout(stderr, "ARG ERROR: --frm-time must be in range 0 --> %d.\n", UINT_MAX);
                val = 1;
            }
            else
            {
                frm_time = /* NPS / */ fps;
            }
        }
        else if (!strcmp(argv[i], "--run-time"))
        {
            run_time = atoi(argv[i+1]);
            if (inf_time > ULLONG_MAX)
            {
                l->out(l->fs, "ARG ERROR: --run-time must be in range 0 --> %d.\n", ULLONG_MAX);
                fsout(stderr, "ARG ERROR: --run-time must be in range 0 --> %d.\n", ULLONG_MAX);
                val = 1;
            }
        }
        else if (!strcmp(argv[i], "--inf-time"))
        {
            inf_time = atoi(argv[i+1]);
            if (inf_time > UINT_MAX)
            {
                l->out(l->fs, "ARG ERROR: --inf-time must be in range 0 --> %d.\n", UINT_MAX);
                fsout(stderr, "ARG ERROR: --inf-time must be in range 0 --> %d.\n", UINT_MAX);
                val = 1;
            }
        }
    }

    if (val)
	{
        print_help();
        exit(EXIT_FAILURE);
    }

    /* Initialising pigpio. */
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "Error initialising pigpio\n");
        exit(EXIT_FAILURE);
    }

    /* Creating gpio input to read the e18-d80nk sensor. */
    if ((val = gpioSetMode(INF_SENSE, PI_INPUT)) != 0)
    {
        if (val == PI_BAD_GPIO)
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
    if ((val = gpioSetMode(INF_LIGHT, PI_OUTPUT)) != 0)
    {
        if (val == PI_BAD_GPIO)
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
        gpioWrite(INF_LIGHT, 0);
    }

    timer_sec_reinit(frm_timer, l);

    /* Running the loop. */ 
    while (run)
    {
        /* Checking if it's time to run a frame. */
        if (timer_sec_elapsed(*frm_timer, frm_time, l))
        {
            if ((val = gpioRead(INF_SENSE)) == 0)
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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pigpio.h>

#include "mycutils.h"

int main(int argc, char** argv)
{
    struct timespec my_timer;   /* Stores a time. */
    uint64_t nanos_per_frame;   /* The number of nanoseconds per frame. */
    bool is_running;            /* Whether the program is running. */
    char* filetext;             /* Text to write to a file. */
    char* tstamp;               /* Timestamp we need to free(). */
    int framecount;             /* Counts how many frames have happened. */
    bool is_recording;
    struct timespec rec_timer;
    struct timespec end_timer;
    char* command;
    int err_msg;
    int gpio_val;
    int gpio_pin;

    gpio_pin = 4;

    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "Error initialising pigpio\n");
        exit(EXIT_FAILURE);
    }


    if ((err_msg = gpioSetMode(gpio_pin, PI_INPUT)) != 0)
    {
        if (err_msg == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }
    
    is_recording = false;

    /* Sixty frame per second. */
    nanos_per_frame = NANOS_PER_SEC / 10;

    /* Program will loop indefinitely. */
    is_running = true;

    /* Getting the start time. */
    start_timer(&my_timer);
    start_timer(&rec_timer);
    start_timer(&end_timer);

    /* No frames have past yet. */
    framecount = 0;

    /* Running the loop. */ 
    while (is_running)
    {
//        if (check_timer(rec_timer, (uint64_t) NANOS_PER_SEC * 10))
//            is_recording = false;

        /* Checking if it's time to run a frame. */
        if (check_timer(my_timer, nanos_per_frame))
        {
            /* Recording this frame. */
            framecount++;
            
            gpio_val = gpioRead(gpio_pin);
            if (gpio_val == 1/* && !is_recording*/)
            {
               is_recording = true;
               start_timer(&rec_timer);
               fprintf(stdout, "motion detected\n");
//               strfmt(&command, "rpicam-vid -t 5000 --tuning-file /usr/share/libcamera/ipa/rpi/pisp/imx219_noir.json -o test%d.mp4", framecount);
//               system(command);
//               free(command);
            }
            else if (gpio_val == 0)
            {
               fprintf(stdout, "0\n");
            }
            else if (gpio_val == PI_BAD_GPIO)
            {
                fprintf(stdout, "Error reading gpio: PI_BAD_GPIO\n");
                exit(EXIT_FAILURE);
            }

            /* Checking if we should end the loop. */
            if (framecount >= 10 * 30)
                is_running = false;

            /* Restarting the timer. */
            start_timer(&my_timer); 
        }

    } 
	

    gpioTerminate();

    exit(EXIT_SUCCESS);
}

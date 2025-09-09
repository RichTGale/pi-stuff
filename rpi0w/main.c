#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pigpio.h>

#include "mycutils.h"

void print_help()
{
    fprintf(stdout, "USAGE: sensor [OPTIONS]\n\n"
                    "OPTIONS:\n"
                    "   -h  Print this help menu.\n"
                    "   -t  Length of time (in hours) to run the program (default 1).\n"
                    "   -l  Local temp directory.\n"
                    "   -r  Directory of this repo on the camera device.\n"
                    "   -u  The current user's username.\n"
                    "\n"
                    "EXAMPLE:\n"
                    "   sudo ./sensor -t 5 -u richard -l \"$HOME/Programming/pir-cam/sensor/temp\" "
                    "-r \"192.168.0.4:/home/<username>/Programming/pir-cam\"\n");
}

int main(int argc, char** argv)
{
    struct timespec frm_timer;          /* Timer for framerate. */
    struct timespec rec_timer;          /* Timer for camera recording. */
    struct timespec end_timer;          /* Timer for ending the program. */
    bool is_running;                    /* Whether the program is running. */
    bool is_recording;                  /* Whether the program is recording. */
    char* tstamp;                       /* A timestamp. (Need to free()). */
    char* str_buff;                     /* Buffer to make strings. */
    char* username;                     /* The username on the local machine. */
    int err_code;                       /* An error code. */
    int invalid_args;                   /* The number of invalid arguments passed to this program. */
    int gpio_val;                       /* The valjue read from the PIR sensor. */
    unsigned int run_hours;             /* The number of hours to run the program. */
    char* loc_dir;                      /* Locla temp directory. */
    char* rem_dir;                      /* Directory of this repo o the camera device. */
    long long unsigned int framecount;  /* Counts how many frames have happened. */
    long long unsigned int nanos_per_frame;  /* The number of nanoseconds per frame. */
    const int FRAMES_PER_SEC = 2;       /* The number of frames per second. */
    const long long unsigned int _NANOS_PER_SEC = NANOS_PER_SEC; /* The number of nanoseconds per second. Defined in "mycutils.h". */
    const int PIR_PIN = 27;             /* Pin number of the PIR sensor. */
    const int IR_PIN = 4;
    const int IR2_PIN = 23;
    const int RECORDING_SECONDS = 10;   /* Seconds to wait after a detection. */

    /* Initialising some variables. */
    nanos_per_frame = _NANOS_PER_SEC / FRAMES_PER_SEC;
    is_running = true;
    is_recording = false;
    framecount = 0;
    invalid_args = argc;

    /* Checking if there are the right number of arguments. */
    if (argc < 2 || argc > 9)
    {
        fprintf(stdout, "Error: Invalid number of arguments\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i+=2)
    {
        if (!strcmp(argv[i], "-h"))
        {
            print_help();
            exit(EXIT_FAILURE);
        }
        else if (!strcmp(argv[i], "-t"))
        {
            run_hours = atoi(argv[i+1]);
            invalid_args -= 2;
        }
        else if (!strcmp(argv[i], "-l"))
        {
            loc_dir = argv[i + 1];
            invalid_args -= 2;
        }
        else if (!strcmp(argv[i], "-r"))
        {
            rem_dir = argv[i + 1];
            invalid_args -= 2;
        }
        else if (!strcmp(argv[i], "-u"))
        {
            username = argv[i + 1];
            invalid_args -= 2;
        }
    }

    if (invalid_args > 1)
	{
        fprintf(stdout, "Please use the correct flags (e.g \"-x\") preceeding the corresponding argument value.\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    /* Printing welcome message. */
    tstamp = timestamp();
    strfmt(&str_buff, "Program started at %s. Will run for %d hours\n", tstamp, run_hours);
    fprintf(stdout, "%s", str_buff);
    free(tstamp);
    free(str_buff);
    
    /* Initialising pigpio. */
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "Error initialising pigpio\n");
        exit(EXIT_FAILURE);
    }

    /* Creating gpio input to read the PIR sensor. */
    if ((err_code = gpioSetMode(PIR_PIN, PI_INPUT)) != 0)
    {
        if (err_code == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }
    /* Creating gpio input to read the PIR sensor. */
    if ((err_code = gpioSetMode(IR_PIN, PI_OUTPUT)) != 0)
    {
        if (err_code == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }
    /* Creating gpio input to read the PIR sensor. */
    if ((err_code = gpioSetMode(IR2_PIN, PI_OUTPUT)) != 0)
    {
        if (err_code == PI_BAD_GPIO)
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
        }
        else
        {
            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
        }
        exit(EXIT_FAILURE);
    }
    
    /* Initialising timers */
    start_timer(&frm_timer);
    start_timer(&rec_timer);
    start_timer(&end_timer);


    /* Running the loop. */ 
    while (is_running)
    {
        /* Checking if it's time to run a frame. */
        if (check_timer(frm_timer, nanos_per_frame))
        {
            /* Recording this frame. */
            framecount++;

            if (check_timer(rec_timer, (long long unsigned) _NANOS_PER_SEC * RECORDING_SECONDS))
            {   
                /* Turn off the Ir light. */
                gpioWrite(IR_PIN, 0);
                gpioWrite(IR2_PIN, 0);
                is_recording = false;
            }
            
            gpio_val = gpioRead(PIR_PIN);
            if (gpio_val == 1 && !is_recording)
            {
                /* Send a message to the camera device so it begins recording video. */

                /* Start the recording timer and temporarily prevent more detections. */
                start_timer(&rec_timer);
                is_recording = true;

                printf("Motion detected\n");
                /* Turn on the Ir light. */
                gpioWrite(IR_PIN, 1);
                gpioWrite(IR2_PIN, 1);

//                /* Create a text file. */
//                tstamp = timestamp();
//                strfmt(&str_buff, "sudo -i -u %s touch \"%s/sensor/temp/%s.txt\"", username, loc_dir, tstamp);
//                system(str_buff);
//                free(str_buff);
//
//                /* Put the timestamp in the file. */
//                strfmt(&str_buff, "sudo -i -u %s echo \"%s\" >> \"%s/sensor/temp/%s.txt\"", username, tstamp, loc_dir, tstamp);
//                system(str_buff);
//                free(str_buff);
//
//                /* Send the file to the camera device. */
//                strfmt(&str_buff, "sudo -i -u %s scp \"%s/sensor/temp/%s.txt\" \"%s/camera/todo\"", username, loc_dir, tstamp, rem_dir);
//                system(str_buff);
//
//                /* Print the detection time to the console before freeing memory. */
//                fprintf(stdout, "%s", tstamp);
//                free(str_buff);
//                free(tstamp);
            }
            else if (gpio_val == PI_BAD_GPIO)
            {
                fprintf(stdout, "Error reading gpio: PI_BAD_GPIO\n");
                exit(EXIT_FAILURE);
            }

            /* Checking if we should end the program. */
            if (framecount >= FRAMES_PER_SEC * 60 * 60 * run_hours)
                is_running = false;

            /* Restarting the framerate timer. */
            start_timer(&frm_timer); 
        }

    } 

    gpioTerminate();

    exit(EXIT_SUCCESS);
}

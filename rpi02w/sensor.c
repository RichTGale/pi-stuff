#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pigpio.h>

#include "utils.h"


void print_help()
{
    fprintf(stdout, "USAGE: sensor [OPTIONS]\n\n"
                    "OPTIONS:\n"
                    "   -h  Print this help menu.\n"
                    "   -t  Length of time (in hours) to run the program (default 1).\n"
                    "\n"
                    "EXAMPLE:\n"
                    "   sudo ./sensor -t 5\n");
}

bool valid_args(int argc, char** argv)
{
    bool args_valid = true;
    int arg = 1; // The first arg is the number of args

    /* Checking if there are the right number of arguments. */
/*    if (argc < 2 || argc > 2)
    {
        fprintf(stdout, "Error: Invalid number of arguments\n");
        print_help();
        exit(EXIT_FAILURE);
    }
*/

    for (arg = 1; arg < argc; arg+=1)
    {
        if (!strcmp(argv[arg], "-h"))
        {
            args_valid = false;
        }
        else if (!strcmp(argv[i], "-t"))
        {
            run_hours = atoi(argv[i+1]);
            invalid_args -= 2;
        }
//        else if (!strcmp(argv[i], "-l"))
//        {
//            loc_dir = argv[i + 1];
//            invalid_args -= 2;
//        }
//        else if (!strcmp(argv[i], "-r"))
//        {
//            rem_dir = argv[i + 1];
//            invalid_args -= 2;
//        }
//        else if (!strcmp(argv[i], "-u"))
//        {
//            username = argv[i + 1];
//            invalid_args -= 2;
//        }
    }

}

/**
 * Returns 0 on success.
 */
int init_pir(int pin, FILE* log)
{
    int error;
    char* sp;

    /* Creating gpio input to read the PIR sensor. */
    if ((error = gpioSetMode(pin, PI_INPUT)) != 0)
    {
        if (error == PI_BAD_GPIO)
        {
	        print(   log, strfmt(sp, "Error in init_pir() setting pinmode for pin %d: PI_BAD_GPIO\n"));
            print(stdout, strfmt(sp, "Error in init_pir() setting pinmode for pin %d: PI_BAD_GPIO\n"));
        }
        else
        {
            print(   log, strfmt(sp, "Error in init_pir() setting pinmode for pin %d: PI_BAD_MODE\n"));
            print(stdout, strfmt(sp, "Error in init_pir() setting pinmode for pin %d: PI_BAD_MODE\n"));
        }
    }

    return error;
}

/**
 * Returns 0 on success.
 */
int init_ir(int pin, FILE* log)
{
    int error;
    char* sp;

    if ((error = gpioSetMode(pin, PI_OUTPUT)) != 0)
    {
        if (error == PI_BAD_GPIO)
        {
	        print(   log, strfmt(sp, "Error in init_ir() setting pinmode for pin %d: PI_BAD_GPIO\n", pin));
            print(stdout, strfmt(sp, "Error in init_ir() setting pinmode for pin %d: PI_BAD_GPIO\n", pin));
        }
        else
        {
            print(   log, strfmt(sp, "Error in init_ir() setting pinmode for pin %d: PI_BAD_MODE\n", pin));
            print(stdout, strfmt(sp, "Error in init_ir() setting pinmode for pin %d: PI_BAD_MODE\n", pin));
        }
    }

    return error;
}

int main(int argc, char** argv)
{
	struct {
	    
        const int FRAMES_PER_SEC;       /* The number of frames per second. */
	    const long long unsigned int _NANOS_PER_SEC; /* The number of nanoseconds per second. Defined in "mycutils.h". */
	    const int RECORDING_SECONDS;   /* Seconds to wait after a detection. */

		const int PIR_PIN;             /* Pin number of the PIR sensor. */
	    const int IR1_PIN;
        const int IR2_PIN;

        FILE* log;
        char* log_buff;

        struct timespec frm_timer;          /* Timer for framerate. */
	    struct timespec rec_timer;          /* Timer for camera recording. */
	    struct timespec end_timer;          /* Timer for ending the program. */
	    
        bool is_running;                    /* Whether the program is running. */
	    bool is_recording;                  /* Whether the program is recording. */
	    
        char* tstamp;                       /* A timestamp. (Need to free()). */
	    char* buff;                     /* Buffer to make strings. */
	    
        int err_code;                       /* An error code. */
	    int invalid_args;                   /* The number of invalid arguments passed to this program. */
	    
        int gpio_val;                       /* The valjue read from the PIR sensor. */
	    
        unsigned int run_time;             /* The number of hours to run the program. */
	    
        long long unsigned int framecount;  /* Counts how many frames have happened. */
	    long long unsigned int nanos_per_frame;  /* The number of nanoseconds per frame. */
	
	} myobj = { 2, NANOS_PER_SEC, 10, 27, 4, 23 };


    char* tstamp;

//
//    if (invalid_args > 1)
//	{
//        fprintf(stdout, "Please use the correct flags (e.g \"-x\") preceeding the corresponding argument value.\n");
//        print_help();
//        exit(EXIT_FAILURE);
//    }

    /* Printing welcome message. */
    print(myobj.log, strfmt(myobj.log_buff, "Program start time: %d\n", tstamp = timestamp()));
    print(myobj.log, strfmt(myobj.log_buff, "Projected run time: %d\n", myobj.run_time));
    print(stdout, strfmt(myobj.log_buff, "Program start time: %d\n", tstamp = timestamp()));
    print(stdout, strfmt(myobj.log_buff, "Projected run time: %d\n", myobj.run_time));
    
    /* Initialising pigpio. */
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "Error initialising pigpio\n");
        exit(EXIT_FAILURE);
    }

    init_pir(myobj.PIR_PIN, myobj.log);
    init_ir(myobj.IR1_PIN, myobj.log);
    init_ir(myobj.IR2_PIN, myobj.log);

    /* Creating gpio input to read the PIR sensor. */
    /* Creating gpio input to read the PIR sensor. */
//    if ((err_code = gpioSetMode(IR2_PIN, PI_OUTPUT)) != 0)
//    {
//        if (err_code == PI_BAD_GPIO)
//        {
//            fprintf(stdout, "Error setting pinmode: PI_BAD_GPIO\n");
//        }
//        else
//        {
//            fprintf(stdout, "Error setting pinmode: PI_BAD_MODE\n");
//        }
//        exit(EXIT_FAILURE);
//    }
    
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
//                gpioWrite(IR2_PIN, 0);
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
//                gpioWrite(IR2_PIN, 1);

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

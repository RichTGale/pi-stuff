/**
 * ardcom.c
 *
 * This file contains the internal data-structure and function definitions
 * for the ardcom type.
 * The ardcom type enables communication to an arduino serial.
 *
 * Written by Richard Gale.
 * Last edited at 11:48pm on March 23, 2024. 
 */

#include "ardcom.h"

/**
 * Internal data-structure of the ardcom type.
 */
struct ardcom_data {
    int pi_id;
    int serial_id;
};

/* Function prototype forward declarations. */
void daemon_connect(int* pi_id);
void daemon_disconnect(int pi_id);
void serial_start(ardcom* acp);
void serial_stop(ardcom ac);

/**
 * Initialise the adcom instance passed as an argument.
 */
void ardcom_init(ardcom* acp)
{
    /* Allocate memory. */
    *acp = (ardcom) malloc(sizeof(struct ardcom_data));

    /* Connect to pigpio daemon. */
    daemon_connect(&(*acp)->pi_id);
    
    /* Open the serial. */
    serial_start(acp);

}

/**
 * Destroy the ardcom instance passed as an argument.
 */
void ardcom_term(ardcom* acp)
{
    /* Close the serial. */
    serial_stop(*acp);
    
    /* Disconnect from the pigpio daemon. */
    daemon_disconnect((*acp)->pi_id);

    /* De-allocate memory. */
    free(*acp);
}

/**
 * Connect to the pigpio daemon.
 */
void daemon_connect(int* pi_id)
{
    /* Connect to pigpio daemon. */
    if ((*pi_id = pigpio_start(NULL, NULL)) < 0)
    {
        fprintf(stdout, 
                "Error in function ardcom_daemon_connect(): "
                "There was a problem connecting to the pigpio daemon\n");
    }
}

/**
 * Disconnect from the pigpio daemon.
 */
void daemon_disconnect(int pi_id)
{
    /* Disconnect from the pigpio daemon. */
    pigpio_stop(pi_id);
}

/**
 * Open the serial.
 */
void serial_start(ardcom* acp)
{
    int error;

    /* Open the serial. */
    if ((error = (*acp)->serial_id = serial_open((*acp)->pi_id, 
                    "/dev/ttyUSB0", 9600, 0)) < 0)
    {
        if (error == PI_NO_HANDLE)
        {
            fprintf(stdout,
                    "Error in function ardcom_start(): "
                    "The Pi ID was bad.\n");
        }
        else if (error = PI_SER_OPEN_FAILED)
        {
            fprintf(stdout,
                    "Error in function ardcom_start(): "
                    "The serial failed to open.\n");
        }
    }
    else
    {
        /* Allow time for the arduino to reset after the new serial connection
         * was made. */
        sleep(2);
    }
}

/**
 * Close the serial.
 */
void serial_stop(ardcom ac)
{
    int error;

    /* Close the serial. */
    if ((error = serial_close(ac->pi_id, ac->serial_id)) != 0)
    {
        if (error == PI_BAD_HANDLE)
        {
            fprintf(stdout,
                    "Error in function ardcom_stop: "
                    "The Pi ID was bad.\n");
        }
    }
}

/**
 * Pause the program until the serial has data in it.
 */
void wait_for_serial(ardcom ac)
{
    int error;

    /* Wait for serial. */
    while ((error = serial_data_available(ac->pi_id, ac->serial_id)) < 1)
    {
        if (error == PI_BAD_HANDLE)
        {
            fprintf(stdout,
                    "Error in function wait_for_serial(): "
                    "The serial ID passed to the function was bad\n");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * Read a byte from the serial.
 */
int ardcom_read_byte(ardcom ac)
{
    int b;
    int error;

    /* Wait until there is data in the serial. */
    wait_for_serial(ac);

    /* Read a byte from the serial. */
    if ((error = b = serial_read_byte(ac->pi_id, ac->serial_id)) < 0)
    {
        if (error == PI_BAD_HANDLE)
        {
            fprintf(stdout,
                    "Error in function read_byte(): "
                    "The serial ID passed to the function was bad\n");
        }
        else if (error == PI_SER_READ_NO_DATA)
        {
            fprintf(stdout,
                    "Error in function read_byte(): "
                    "The serial didn't read any data\n");
        }
        else if (error == PI_SER_READ_FAILED)
        {
            fprintf(stdout,
                    "Error in function read_byte(): "
                    "The serial failed to read the data\n");
        }
    }

    return b;
}

//void ardcom_print_next_str(ardcom ac)
//{
//    char* message;
//
//    ardcom_read_str(ac, &message);
//    fprintf(stdout, "%s\n", message);
//    free(message);
//}

/**
 * Read a string from the serial.
 */
void ardcom_read_str(ardcom ac, char** buf)
{
    int num_chars;
    int c;

    /* Get the number of characters that should be read. */
    num_chars = ardcom_read_byte(ac);

    /* Allocate memory for the string and the null character. */
    *buf = (char*) malloc(sizeof(char) * (num_chars + 1));

    /* Read the characters into the string. */
    for (c = 0 ; c < num_chars; c++)
    {
        (*buf)[c] = (char) ardcom_read_byte(ac);
    }

    /* Specify the end of the string. */
    (*buf)[c] = '\0';
}

/**
 * Write a byte to the serial. 
 */
void ardcom_write_byte(ardcom ac, unsigned b)
{
    int error;

    /* Write a byte to the serial. */
    if ((error = serial_write_byte(ac->pi_id, ac->serial_id, b)) != 0)
    {
        if (error == PI_BAD_HANDLE)
        {
            fprintf(stdout, 
                    "Error in function serial_serial_write_byte(): "
                    "The serial ID was bad.\n");
        }
        else if (error == PI_BAD_PARAM)
        {
            fprintf(stdout, 
                    "Error in function serial_serial_write_byte(): "
                    "A bad parameter was passed to the function.\n");
        }
        else if (error == PI_SER_WRITE_FAILED)
        {
            fprintf(stdout, 
                    "Error in function serial_serial_write_byte(): "
                    "Failed to write to the serial.\n");
        }
    }
}

/**
 * Write a string to the serial.
 */
void ardcom_write_str(ardcom ac, char* str)
{
    int c;

    /* Write the number of characters that are in the string to be sent to the
     * serial. */
    ardcom_write_byte(ac, strlen(str));

    /* Write the string to the serial. */
    for (c = 0; c < strlen(str); c++)
    {
        ardcom_write_byte(ac, (unsigned) str[c]);
    }
}



/**
 * ardcom.h
 *
 * This file contains the publicly available data-structure and function
 * declarations for the ardcom type.
 * The ardcom type enables communication to an arduino serial.
 *
 * Written by Richard Gale.
 * Last edited at 11:49pm on March 23, 2024. 
 */

#ifndef ARDCOM_H
#define ARDCOM_H

#include <pigpiod_if2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

/**
 * Data-structure of the ardcom type.
 */
typedef struct ardcom_data* ardcom;

/**
 * Initialise the adcom instance passed as an argument.
 */
void ardcom_init(ardcom* acp);

/**
 * Destroy the ardcom instance passed as an argument.
 */
void ardcom_term(ardcom* acp);

/**
 * Read a byte from the serial.
 */
int ardcom_read_byte(ardcom ac);

/**
 * Read a string from the serial.
 */
void ardcom_read_str(ardcom ac, char** buf);

/**
 * Print the next string in the serial.
 */
//void ardcom_print_next_str(ardcom ac);

/**
 * Write a byte to the serial. 
 */
void ardcom_write_byte(ardcom ac, unsigned b);

/**
 * Write a string to the serial.
 */
void ardcom_write_str(ardcom ac, char* str);

#endif

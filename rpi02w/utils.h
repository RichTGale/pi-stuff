
#ifndef MYCUTILS_H
#define MYCUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strerror()
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
//#include <unistd.h>
//#include <termios.h>

/**
 * There is this many nanoseconds in a second.
 */
#define NANOS_PER_SEC 1000000000

/**
 * This function opens a file that has a name that matches fname. It opens the
 * file in the mode specified by mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* openfs(char* fname, char* mode);

/**
 * This function closes the file stream provided tp it. If there is an error,
 * it is printed on stderr and the program will exit.
 */
void closefs(FILE* fs);

/**
 * This function assigns the next char in the file stream provided to it to
 * the buffer provided to it. It returns true on success or false if EOF is
 * reached. It will exit the program if an error occurs.
 */
bool readfsc(FILE* fs, char* buf);

/**
 * This function assigns the next line in the file stream provided to it to
 * the string provided to it. It returns true if the line was read successfully
 * or false if EOF was reached. If an error occurs the program will exit.
 * Make sure to free() the buffer when you're finished with it.
 */
bool readfsl(FILE* fs, char** buf);
/**
 * This function writes the char provided to it to the file stream provided to
 * it.
 */
void writefsc(FILE* fs, char ch);

/**
 * This function writes the string provided to it to the file stream provided
 * to it.
 */
void writefss(FILE* fs, char* str);

/**
 * This function returns the number of bytes a string will need to be
 * allocated based on the variable argument list and a format string that are
 * provided to this function.
 */
size_t vbytesfmt(va_list lp, char* fmt);

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
char* strfmt(char* sp, char *fmt, ...);

/**
 * This function removes the char element from the string provided to it which
 * is at the element number/index provided to it.
 */
char* sdelelem(char* sp, unsigned elem);

/**
 * This function removes all cases of the provided char from the string at the
 * provided pointer.
 */
void sdelchar(char* sp, char remove);

/**
 * This function returns a string that represent the current time.
 * For reasons detailed in a comment within this function, you must
 * free() the string that this function returns.
 */
char* timestamp();

/**
 * This function sends the return value of strfmt() (see above), which is
 * passed as print()'s second actual parameter to the file stream that is
 * passed as the first actual parameter.
 * This function adds a timestamp to the beginning of the output.
 */
void print(FILE* fs, char* (strfmt));

#endif

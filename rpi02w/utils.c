#include "utils.h"

/**
 * This function opens a file that has a name that matches fname. It opens the
 * file in the mode specified by mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* openfs(char* fname, char* mode)
{
    FILE* fs;       /* The pointer to the file stream. */
    char* tstamp;   /* A time stamp. */

    /* Opening the file. */
    if ((fs = fopen(fname, mode)) != NULL)
        return fs;

    /* An error occured so we're printing an error message. */
    fprintf(stderr, 
            "[ %s ] ERROR: In function openfs(): "
            "Could not open file %s: %s\n",
            (tstamp = timestamp()), fname, strerror(errno));

    /* De-allocating memory. */
    free(tstamp);

    /* Freeing memory. */
    exit(EXIT_FAILURE);
}

/**
 * This function closes the file stream provided tp it. If there is an error,
 * it is printed on stderr and the program will exit.
 */
void closefs(FILE* fs)
{
    char* tstamp;   /* A time stamp. */

    /* Closing the file stream. */
    if (fclose(fs) == 0)
        return;
    
    /* An error occured so we are printing an error message. */
    fprintf(stderr,
            "[ %s ] ERROR: In function closefs: %s\n", 
            (tstamp = timestamp()), strerror(errno));

    /* De-allocating memory. */
    free(tstamp);

    /* Exiting the program. */
    exit(EXIT_FAILURE);
}
/**
 * This function assigns the next char in the file stream provided to it to
 * the buffer provided to it. It returns true on success or false if EOF is
 * reached. It will exit the program if an error occurs.
 */
bool readfsc(FILE* fs, char* buf)
{
    const bool SUCCESS = true;      /* Return value if success. */
    const bool END_OF_FILE = false; /* Return value if EOF. */
    char* tstamp;

    /* Getting the next char from the file stream and checking if it was
     * successfully read. */
    if ((*buf = fgetc(fs)) != EOF) 
        return SUCCESS;

    /* Checking if EOF was reached. */
    if (!ferror(fs)) 
        return END_OF_FILE;

    /* An error occurred so we're printing an error message. */
    fprintf(stderr,
            "[ %s ] ERROR: In function readfsc(): %s\n",
            (tstamp = timestamp()), strerror(errno));

    /* De-allocating memory. */
    free(tstamp);

    /* Exiting the program. */
    exit(EXIT_FAILURE);
}


/**
 * This function assigns the next line in the file stream provided to it to
 * the string provided to it. It returns true if the line was read successfully
 * or false if EOF was reached. If an error occurs the program will exit.
 * Make sure to free() the buffer when you're finished with it.
 */
bool readfsl(FILE* fs, char** buf)
{
    const bool SUCCESS = true;      /* Return value if success. */
    const bool END_OF_FILE = false; /* Return value if EOF. */
    size_t n;                       /* Allocated size of the buffer. */
    char* tstamp;                   /* A time stamp. */

    /* Initialising how big the buffer is. */
    n = 0;
    
    /* Reading the next line from the file stream and checking if it was
     * read successfully. */
    if (getline(buf, &n, fs) != -1)
        return SUCCESS;

    /* Checking if EOF was reached. */
    if (!ferror(fs))
        return END_OF_FILE;
            
    /* An error occurred so we are printing an error message. */
    fprintf(stdout,
            "[ %s ] ERROR: In function readfsl: %s\n",
            timestamp(), strerror(errno));

    /* De-allocating memory. */
    free(tstamp);

    /* Exiting the program. */
    exit(EXIT_FAILURE);
}

/**
 * This function writes the char provided to it to the file stream provided to
 * it.
 */
void writefsc(FILE* fs, char ch)
{
    /* Writing the char to the file stream. */
    fprintf(fs, "%c", ch); 
}

/**
 * This function writes the string provided to it to the file stream provided
 * to it.
 */
void writefss(FILE* fs, char* str)
{
    int c;  /* Index of the current char in the string. */

    /* Writing the string to the file stream. */
    for (c = 0; c < strlen(str); c++)
        writefsc(fs, str[c]);
}

/**
 * This function returns the number of bytes a string will need to be
 * allocated based on the variable argument list and a format string that are
 * provided to this function.
 */
size_t vbytesfmt(va_list lp, char* fmt)
{
    va_list lp_cpy; /* A Copy of the list of arguments. */
    size_t bytes;   /* The number of bytes the string needs. */

    /* Copying the argument list. */
    va_copy(lp_cpy, lp);

    /* Getting the number of bytes the string will need. Adding an extra
     * 1 char worth of bytes for the null character. */
    bytes = vsnprintf(NULL, 0, fmt, lp_cpy) + sizeof(char);

    /* Assuring a clean finish to the copy. */
    va_end(lp_cpy);

    /* Returning the number of bytes the string will need. */
    return bytes;
}

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
char* strfmt(char* sp, char *fmt, ...)
{
    va_list lp;     /* Pointer to the list of arguments. */
    size_t bytes;   /* The number of bytes the string needs. */

    /* Pointing to the first argument. */
    va_start(lp, fmt);

    /* Getting the number of bytes the string will need to be allocated. */
    bytes = vbytesfmt(lp, fmt);

    /* Allocating memory to the string. */
    sp = (char*) malloc(bytes);

    /* Creating the string. */
    vsprintf(sp, fmt, lp);

    /* Assuring a clean finish to the argument list. */
    va_end(lp);

    return sp;
}

/**
 * This function removes the char element from the string provided to it which
 * is at the element number/index provided to it.
 */
char* sdelelem(char* sp, unsigned elem)
{
    char* to_elem;      /* Chars from start of string to element to delete. */
    char* from_elem;    /* Chars from element to delete to end of string. */
    unsigned c;         /* The current char in the string. */

    /* Allocating memory. */
    to_elem     = (char*) malloc(sizeof(char) * (elem + 1));
    from_elem   = (char*) malloc(sizeof(char) * (strlen(sp) - elem));

    /* Storing the two sections of the string. */
    for (c = 0; c < strlen(sp); c++)
    {
        if (c < elem)
            to_elem[c] = sp[c];
        if (c > elem)
            from_elem[c] = sp[c];
    }
    to_elem[elem] = '\0';
    from_elem[strlen(sp) - elem - 1] = '\0';

    /* Recreating the string. */
    free(sp);
    sp = strfmt(sp, "%s%s", to_elem, from_elem);

    /* Cleaning up. */
    free(to_elem);
    free(from_elem);

    return sp;
}

/**
 * This function removes all cases of the provided char from the string at the
 * provided pointer.
 */
void sdelchar(char* sp, char remove)
{
    unsigned c;     /* Index of current char in the string. */

    /* Overwriting the unwanted characters. */
	for (c = 0; c < strlen(sp); c++)
	{
        if (sp[c] == remove)
        {
            sp = sdelelem(sp, c);

            /* Decrementing the index so we will check the replacement 
             * character. */
            c--;
        }
    }
}

/**
 * This function returns a string that represent the current time.
 * For reasons detailed in a comment within this function, you must
 * free() the string that this function returns.
 */
char* timestamp()
{
    time_t current_time;    /* The current time. */
    char* stamp;            /* The time stamp. */
    char* stamp_cpy;        /* A Copy of the time stamp. */
    char* output;           /* Output to filestream(s). */

    /* Obtaining the current time. */
    if ((current_time = time(NULL)) == ((time_t) - 1))
    {
        /* An error occured so we're printing an error message to and exiting
         * the program. */
        print(stderr, strfmt(output,
                            "ERROR: In function timestamp(): "
                            "Calender time is not available\n")
                );
        exit(EXIT_FAILURE);
    }

    /* Converting time to local time format. */
    if ((stamp = ctime(&current_time)) == NULL)
    {
        /* An error occured converting so we're printing an error message
         * and exiting the program. */
        print(stderr, strfmt(output, 
                            "ERROR: In function timestamp(): "
                            "Failure to convert the current time to a string.\n")
                );
        exit(EXIT_FAILURE);
    }

    /* The string that ctime() returns is not one that should be be freed
     * with free() because the memory it uses was not allocated with malloc()
     * or a similar function. Because we are going to use sdelchar() to remove
     * the newline character that ctime() added to our timestamp, and
     * sdelchar() uses free() to remove chars from strings, we have to make
     * a copy of our stamp.
     * If this copy is not freed by the calling function, it will create a 
     * memory leak.
     */
    stamp_cpy = strfmt(stamp_cpy, "%s", stamp);

    /* Removing the newline character that was added by ctime(). */
    sdelchar(stamp_cpy, '\n');

    /* Returning the copy of the time stamp. */
    return stamp_cpy;
}

/**
 * This function sends the return value of strfmt() (see above), which is
 * passed as print()'s second actual parameter to the file stream that is
 * passed as the first actual parameter.
 * This function adds a timestamp to the beginning of the output.
 */
void print(FILE* fs, char* (strfmt))
{
    char* time_stamp; /* The date and time. */ 

    /* Sending the text to the file stream. */
    fprintf(fs, "[ %s ] %s", (time_stamp = timestamp()), strfmt);

    /* Freeing memory. */
    free(time_stamp);
    free(strfmt);
}

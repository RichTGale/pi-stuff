#include <stdlib.h>

#include "utils.h"

int main()
{
    log_t* log;

    clear();

    log = log_init("log.txt");
    log->out(log->fs, "Program Started.\n");



    log_term(log); 

    exit(EXIT_SUCCESS);
}

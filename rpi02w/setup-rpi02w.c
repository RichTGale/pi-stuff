
#include <stdlib.h>

#include "utils.h"


int main()
{
    log* _log;

    _log = log_init("log.txt");

    _log->out(_log->fs, "Program Started.\n");

    log_term(_log); 

    exit(EXIT_SUCCESS);
}

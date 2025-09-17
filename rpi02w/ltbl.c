#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pigpio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int error;
    int on_pin = 4;

    if ((error = gpioInitialise()) == PI_INIT_FAILED)
    {
        printf("gpioInitialise returned PI_INIT_FAILED\n");
    }
    else
    {
        printf("pigpio intitialised successfully\n");
    }

    gpioSetMode(on_pin, PI_OUTPUT);

    //if (!strcmp(argv[1], "on"))
    gpioWrite(on_pin, 1);
    //else
     //   gpioWrite(on_pin, 0);

    sleep(10);

    gpioWrite(on_pin, 0);
    

    gpioTerminate();

    exit(EXIT_SUCCESS);
}


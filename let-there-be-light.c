#include <stdlib.h>
#include <stdio.h>
#include <pigpio.h>

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

    gpioWrite(on_pin, 1);

    while (1);

    gpioTerminate();

    exit(EXIT_SUCCESS);
}

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <cerrno>
#include <queue>

#include "motor.hpp"
#include "infrared.hpp"
#include "pathfinding.hpp"

#define DELAY 0.01 * 10e6

Motor motors[2] = {
    {26, 5, 4},
    {23, 22, 21}
};

#define INFRARED_SIZE 3
int infrareds[INFRARED_SIZE] = {
    0, 2, 3
};


void gpio_init();
void gpio_cleanup(int _signum);

int main() {
    // gpio_init();

    PathFinder finder;

    while (!finder.continue_search())
    {

    }
    

    // gpio_cleanup(0);
    return 0;
}

void gpio_cleanup(int _signum) {
    motor_cleanup(motors[0]);
    motor_cleanup(motors[1]);
    exit(0);
}

void gpio_init() {
    if (wiringPiSetup()) {
        fprintf(stderr, "Error: Failed to setup wiring: %s", strerror(errno));
        exit(1);
    }

    motor_setup(motors[0]);
    motor_setup(motors[1]);
    infrared_setup(infrareds, INFRARED_SIZE);

    signal(SIGINT, gpio_cleanup);
}

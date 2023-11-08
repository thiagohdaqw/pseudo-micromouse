#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <cerrno>

#include "motor.hpp"

#define DELAY 0.01 * 10e6

Motor motors[2] = {
    {26, 5, 4},
    {23, 22, 21}
};


void gpio_init();

int main() {
    gpio_init();

    gpio_cleanup(0);
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

    signal(SIGINT, gpio_cleanup);
}
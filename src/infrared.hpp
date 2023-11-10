#ifndef __INFRARED_H_INCLUDED__
#define __INFRARED_H_INCLUDED__

void infrared_setup(int pin[], int size);
int infrared_read(int pin);

void infrared_test(int pin[], int size);

#endif  // __INFRARED_H_INCLUDED__

#ifndef __INFRARED_IMPLEMENTATION__
#define __INFRARED_IMPLEMENTATION__

#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>

void infrared_setup(int pin[], int size) {
    for (int i = 0; i < size; i++) {
        pinMode(pin[i], INPUT);
    }
}

int infrared_read(int pin) {
    return digitalRead(pin);
}

void infrared_test(int pin[], int size) {
    while (true) {
        printf("Infravermelhos: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", infrared_read(pin[i]));
        }
        printf("\n");
        sleep(1);
    }
}

#endif  //__MOTOR_IMPLEMENTATION__

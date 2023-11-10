#ifndef __INFRARED_H_INCLUDED__
#define __INFRARED_H_INCLUDED__

void infrared_setup(int pin[], int size);
int infrared_read(int pin);

#endif  // __INFRARED_H_INCLUDED__

#ifndef __INFRARED_IMPLEMENTATION__
#define __INFRARED_IMPLEMENTATION__

#include <wiringPi.h>

void infrared_setup(int pin[], int size) {
    for (int i = 0; i < size; i++) {
        pinMode(pin[i], INPUT);
    }
}

int infrared_read(int pin) {
    return digitalRead(pin);
}

#endif  //__MOTOR_IMPLEMENTATION__

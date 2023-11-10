#ifndef __MOTOR_H_INCLUDED__
#define __MOTOR_H_INCLUDED__

#define PWM_MIN 35
#define PWM_MAX_VALUE 1024

#include "types.hpp"

typedef struct motor
{
    int pin;
    int pin_in_a;
    int pin_in_b;
} Motor;


void motor_setup(Motor motor);
void motor_cleanup(Motor motor);
void motor_set_pwm_percentage(int percentage);
void motor_move(Motor motors[2], Action action);

#endif  // __MOTOR_H_INCLUDED__

#ifndef __MOTOR_IMPLEMANTATION__
#define __MOTOR_IMPLEMANTATION__

#include <wiringPi.h>

int pwm_percentage = 100;


void motor_setup(Motor motor) {
    pinMode(motor.pin, PWM_OUTPUT);
    pinMode(motor.pin_in_a, OUTPUT);
    pinMode(motor.pin_in_b, OUTPUT);
}

void motor_cleanup(Motor motor) {
    pwmWrite(motor.pin, 0);
    digitalWrite(motor.pin_in_a, LOW);
    digitalWrite(motor.pin_in_b, LOW);
}

void motor_set_pwm_percentage(int value) {
    if (value >= PWM_MIN || value <= 100) {
        pwm_percentage = value;
    }
}

void motor_pwm_write(Motor motor, int percentage) {
    pwmWrite(motor.pin, PWM_MAX_VALUE * ((float)percentage/100));
}

void motor_set_direction(Motor motor, Action action) {
    switch (action) {
        case STOP:
            digitalWrite(motor.pin_in_a, LOW);
            digitalWrite(motor.pin_in_b, LOW);
            break;
        case FRONT:
            digitalWrite(motor.pin_in_a, HIGH);
            digitalWrite(motor.pin_in_b, LOW);
            break;
        case BACK:
            digitalWrite(motor.pin_in_a, LOW);
            digitalWrite(motor.pin_in_b, HIGH);
            break;
    }
}

void motor_move(Motor motors[2], Action action) {
    motor_pwm_write(motors[0], (action != STOP) * pwm_percentage);
    motor_pwm_write(motors[1], (action != STOP) * pwm_percentage);

    if (action == RIGHT) {
        motor_set_direction(motors[0], FRONT);
        motor_set_direction(motors[1], BACK);
    } else if (action == LEFT) {
        motor_set_direction(motors[0], BACK);
        motor_set_direction(motors[1], FRONT);
    } else {
        motor_set_direction(motors[0], action);
        motor_set_direction(motors[1], action);
    }
}

#endif  //__MOTOR_IMPLEMANTATION__
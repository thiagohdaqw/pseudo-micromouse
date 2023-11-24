#ifndef __MOTOR_H_INCLUDED__
#define __MOTOR_H_INCLUDED__

#define PWM_MIN 35
#define PWM_MAX_VALUE 1024

enum MotorDirection {
    STOP,
    BACK,
    FRONT,
    LEFT,
    RIGHT,
};

typedef struct motor {
    int pin;
    int pin_in_a;
    int pin_in_b;
    int pwm_inc;
} Motor;

void motor_setup(Motor motor);
void motor_cleanup(Motor motor);
void motor_set_pwm_percentage(int percentage);
void motor_move(Motor motors[2], MotorDirection direction);

void motor_test(Motor motors[2], float delay_secs);

#endif  // __MOTOR_H_INCLUDED__

#ifndef __MOTOR_IMPLEMANTATION__
#define __MOTOR_IMPLEMANTATION__

#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

#include <unistd.h>

int pwm_percentage = 45;
float delay_secs = 1;

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
    pwmWrite(motor.pin, PWM_MAX_VALUE * ((float)percentage / 100));
}

void motor_set_direction(Motor motor, MotorDirection direction) {
    switch (direction) {
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

void motor_move(Motor motors[2], MotorDirection direction) {
    motor_pwm_write(motors[0], (direction != STOP) * (pwm_percentage + motors[0].pwm_inc));
    motor_pwm_write(motors[1], (direction != STOP) * (pwm_percentage + motors[1].pwm_inc));

    if (direction == RIGHT) {
        motor_set_direction(motors[0], FRONT);
        motor_set_direction(motors[1], BACK);
    } else if (direction == LEFT) {
        motor_set_direction(motors[0], BACK);
        motor_set_direction(motors[1], FRONT);
    } else {
        motor_set_direction(motors[0], direction);
        motor_set_direction(motors[1], direction);
    }
}

void motor_test(Motor motors[2], float delay_secs) {
    int delay = delay_secs * 1e6;
    while (true) {
        printf("Parando\n");
        motor_move(motors, STOP);
        usleep(delay);

        printf("Frente\n");
        motor_move(motors, FRONT);
        usleep(delay);

        printf("Direita\n");
        motor_move(motors, RIGHT);
        usleep(delay);

        printf("Baixo\n");
        motor_move(motors, BACK);
        usleep(delay);

        printf("Esquerda\n");
        motor_move(motors, LEFT);
        usleep(delay);
    }
}

#endif  //__MOTOR_IMPLEMANTATION__

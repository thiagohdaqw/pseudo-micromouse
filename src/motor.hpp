#ifndef __MOTOR_H_INCLUDED__
#define __MOTOR_H_INCLUDED__

#define PWM_MIN 35
#define PWM_MAX_VALUE 1024

#ifndef ROTATE_DELAY_US
#define ROTATE_DELAY_US 0.3 * 1e6
#endif

enum MotorDirection {
    STOP,
    FRONT,
    RIGHT,
    BACK,
    LEFT,
};

typedef struct motor {
    int pin;
    int pin_in_a;
    int pin_in_b;
    int pwm_inc;
} Motor;

void motor_setup(Motor[2] motor);
void motor_cleanup(Motor motor);
void motor_set_pwm_percentage(int percentage);
void motor_move(MotorDirection direction);

void motor_test(Motor motors[2], float delay_secs);

#endif  // __MOTOR_H_INCLUDED__

#ifndef __MOTOR_IMPLEMANTATION__
#define __MOTOR_IMPLEMANTATION__

#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

#include <unistd.h>

static int pwm_percentage = 45;
static float delay_secs = 1;

static Motor[2] motors;

void motor_setup(Motor[2] mtrs) {
    for (int i = 0; i < 2; i++) {
        pinMode(mtrs[i].pin, PWM_OUTPUT);
        pinMode(mtrs[i].pin_in_a, OUTPUT);
        pinMode(mtrs[i].pin_in_b, OUTPUT);
    }
    motors = mtrs;
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

void motor_move(MotorDirection direction) {
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

void motor_rotate(MotorDirection direction, MotorDirection target) {
    while (direction != target)
    {
        motor_move(MotorDirection::RIGHT);
        usleep(ROTATE_DELAY_US);
        direction = (direction + 1) % 4 + 1;
    }
    motor_move(MotorDirection::STOP);
}

void motor_test(Motor motors[2], float delay_secs) {
    int delay = delay_secs * 1e6;
    while (true) {
        printf("Parando\n");
        motor_move(STOP);
        usleep(delay);

        printf("Frente\n");
        motor_move(FRONT);
        usleep(delay);

        printf("Direita\n");
        motor_move(RIGHT);
        usleep(delay);

        printf("Baixo\n");
        motor_move(BACK);
        usleep(delay);

        printf("Esquerda\n");
        motor_move(LEFT);
        usleep(delay);
    }
}

#endif  //__MOTOR_IMPLEMANTATION__

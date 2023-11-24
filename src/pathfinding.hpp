#ifndef __INCLUDE_PATHFINDING__
#define __INCLUDE_PATHFINDING__

#include <stdio.h>
#include "ultrasonic.hpp"
#include "motor.hpp"

#define MIN_DISTANCE_CM 20
#define TURN_RIGHT_DELAY    5e5
#define TURN_LEFT_DELAY     5e5
#define BACK_DELAY          1e6

void pathfinding_test(Ultrasonic ultrasonics[3], Motor motors[2]) {
    MotorDirection direction = MotorDirection::STOP;

    sleep(1);
    motor_set_pwm_percentage(45);
    while (1) {
        if (ultrasonics[ULTRASONIC_FRONT].distance > MIN_DISTANCE_CM) {
            motor_move(motors, MotorDirection::FRONT);
            direction = MotorDirection::FRONT;
        } else {
            motor_move(motors, MotorDirection::STOP);
            direction = MotorDirection::STOP;
        }

        if (direction == MotorDirection::STOP) {
            printf("Frente = %dcm, Direita = %dcm, Esquerda = %dcm\n", ultrasonics[0].distance, ultrasonics[1].distance, ultrasonics[2].distance);
            if (ultrasonics[ULTRASONIC_RIGHT].distance > MIN_DISTANCE_CM) {
                motor_move(motors, MotorDirection::RIGHT);
                usleep(TURN_RIGHT_DELAY);
                motor_move(motors, MotorDirection::STOP);
            } else if (ultrasonics[ULTRASONIC_LEFT].distance > MIN_DISTANCE_CM) {
                motor_move(motors, MotorDirection::LEFT);
                usleep(TURN_LEFT_DELAY);
                motor_move(motors, MotorDirection::STOP);
            } else {
                motor_move(motors, MotorDirection::BACK);
                usleep(BACK_DELAY);
                motor_move(motors, MotorDirection::STOP);

                while (ultrasonics[ULTRASONIC_RIGHT].distance < MIN_DISTANCE_CM) {
                    motor_move(motors, MotorDirection::LEFT);
                    usleep(TURN_LEFT_DELAY);
                    motor_move(motors, MotorDirection::STOP);
                }
            }
        }

        usleep(5e4);
    }
}















#endif

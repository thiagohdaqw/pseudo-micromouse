#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>

#include <cerrno>

#include "goal.hpp"
#include "motor.hpp"
#include "pathfinding.hpp"
#include "ultrasonic.hpp"
#include "video.hpp"
// #include "goal_recognition.hpp"

#define DELAY 0.01 * 10e6

Motor motors[2] = {
    {23, 21, 22, 3},  // Direita
    {26, 5, 4, 0}     // Esquerda
};

Ultrasonic ultrasonics[ULTRASONIC_SIZE] = {
    {3, 12, 0},  // Frente { echo, trigger, distance}
    {0, 14, 0},  // Esquerda
    {2, 13, 0}   // Direita
};

void gpio_init();
void gpio_cleanup(int _signum);

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        printf(
            "  subcommands:\n"
            "   find\n"
            "   motor\n"
            "   sonic\n"
            "   goal\n"
            "   video CHAT_ID TOKEN OUTPUT_VIDEO_PATH\n");
        return 1;
    }

    gpio_init();

    if (strcmp(argv[1], "motor") == 0) {
        assert(argc == 3 && "Usage: ./main motor ACTION_DELAY_SECS");
        float delay_secs = atof(argv[2]);
        printf("Testando os motores. Delay = %.2f\n", delay_secs);
        motor_test(motors, delay_secs);
    } else if (strcmp(argv[1], "video") == 0) {
        assert(argc == 5 && "Usage: ./main CHAT_ID TOKEN OUTPUT_VIDEO_PATH");
        printf("Testando o video\n");
        video_make_from_images(argv[4]);
        video_send_telegram(argv[2], argv[3], argv[4]);
    } else if (strcmp(argv[1], "sonic") == 0) {
        ultrasonic_test(ultrasonics);
    } else if (strcmp(argv[1], "cam") == 0) {
        TCamArgs cam_args = {0};
        point current(1,1);

        cam_args.current = &current;

        cam_init(&cam_args);

        cam_test(&cam_args);
    } else if (strcmp(argv[1], "find") == 0) {
        printf("Starting search...\n");

        PathFinder finder;
        TCamArgs cam_args = {0};

        cam_args.current = &finder.current_position;
        cam_init(&cam_args);

        motor_set_pwm_percentage(95);

        finder.find();
    }

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

    motor_init();
    ultrasonic_init();
    signal(SIGINT, gpio_cleanup);
}

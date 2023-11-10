#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <cerrno>

#include "motor.hpp"
#include "infrared.hpp"
#include "video.hpp"

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

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        printf("  subcommands:\n"
                "   motor\n"
                "   infrared\n"
                "   goal\n"
                "   video CHAT_ID TOKEN OUTPUT_VIDEO_PATH\n"
        );
        return 1;
    }

    gpio_init();

    if (strcmp(argv[1], "motor") == 0) {
        printf("Testando os motores\n");
        motor_test(motors);
    }
    if (strcmp(argv[1], "infrared") == 0) {
        printf("Testando os infra vermelhos\n");
        infrared_test(infrareds, INFRARED_SIZE);
    }
    if (strcmp(argv[1], "goal") == 0) {
        printf("Testando os identificador de objetivo\n");
    }
    if (strcmp(argv[1], "video") == 0) {
        assert(argc == 5 && "Usage: ./main CHAT_ID TOKEN OUTPUT_VIDEO_PATH");
        printf("Testando o video\n");
        video_make_from_images(argv[4]);
        video_send_telegram(argv[2], argv[3], argv[4]);
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

    motor_setup(motors[0]);
    motor_setup(motors[1]);
    infrared_setup(infrareds, INFRARED_SIZE);

    signal(SIGINT, gpio_cleanup);
}

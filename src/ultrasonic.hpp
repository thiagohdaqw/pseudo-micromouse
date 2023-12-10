#ifndef __ULTRASONIC_IMPLEMENTATION__
#define __ULTRASONIC_IMPLEMENTATION__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include <cstring>

typedef struct ultrasonic {
    int echo;
    int trigger;
    unsigned int distance;
    pthread_t tid;
} Ultrasonic;

#define ULTRASONIC_SIZE 3
extern Ultrasonic ultrasonics[ULTRASONIC_SIZE];

enum UltrasonicDirection {
    UFRONT,
    URIGHT,
    ULEFT,
};

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define ROUNDTRIP_M 5800.0f
#define ROUNDTRIP_CM 58
#define ULTRASONIC_WATCH_DELAY 1e5
#define MAX_DISTANCE_CM 100

static pthread_t watch_tid;

unsigned int ultrasonic_measure_raw(Ultrasonic ultrasonic) {
    // Ping: Low for 2..4 us, then high 10 us
    digitalWrite(ultrasonic.trigger, 0);
    usleep(TRIGGER_LOW_DELAY);
    digitalWrite(ultrasonic.trigger, 1);
    usleep(TRIGGER_HIGH_DELAY);
    digitalWrite(ultrasonic.trigger, 0);

    // Previous ping isn't ended
    digitalRead(ultrasonic.echo);

    long int timeout = MAX_DISTANCE_CM * ROUNDTRIP_CM;
    // Wait for echo
    long int start = micros();
    long int end = micros();
    while (!digitalRead(ultrasonic.echo) && (end - start) <= timeout) {
        end = micros();
    }

    // got echo, measuring
    long int echo_start = micros();
    long int time = echo_start;
    while (digitalRead(ultrasonic.echo) && (time - start) <= timeout)
        time = micros();
    time = micros();

    return time - echo_start;
}

void ultrasonic_measure_cm(Ultrasonic *ultrasonic) {
    ultrasonic->distance = ultrasonic_measure_raw(*ultrasonic) / ROUNDTRIP_CM;
}

void *ultrasonic_watch(void *args) {
    while (1) {
        usleep(ULTRASONIC_WATCH_DELAY);

        for (int i = 0; i < ULTRASONIC_SIZE; i++) {
            ultrasonic_measure_cm(&ultrasonics[i]);
        }
    }
    return NULL;
}

void ultrasonic_init() {
    for (int i = 0; i < ULTRASONIC_SIZE; i++) {
        pinMode(ultrasonics[i].trigger, OUTPUT);
        pinMode(ultrasonics[i].echo, INPUT);

        digitalWrite(ultrasonics[i].trigger, LOW);
    }
    pthread_create(&watch_tid, NULL, &ultrasonic_watch, NULL);
}

unsigned int ultrasonic_distance_cm(UltrasonicDirection direction) {
    return ultrasonics[(int)direction].distance;
}

void ultrasonic_test(Ultrasonic ultrasonics[ULTRASONIC_SIZE]) {
    while (1) {
        sleep(1);
        printf("Frente = %dcm, Direita = %dcm, Esquerda = %dcm\n",
               ultrasonics[0].distance, ultrasonics[1].distance,
               ultrasonics[2].distance);
    }
}

#endif  //__ULTRASONIC_IMPLEMENTATION__

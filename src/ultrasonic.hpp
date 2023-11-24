#ifndef __ULTRASONIC_IMPLEMENTATION__
#define __ULTRASONIC_IMPLEMENTATION__

#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

typedef struct ultrasonic {
    int echo;
    int trigger;
} Ultrasonic;

#define TRUE (1==1)

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define PING_TIMEOUT 6000
#define ROUNDTRIP_M 5800.0f
#define ROUNDTRIP_CM 58


void ultrasonic_init(Ultrasonic *ultrasonic, int size)
{
    for (int i = 0; i < size; i++) {
        pinMode(ultrasonic[i].trigger, OUTPUT);
        pinMode(ultrasonic[i].echo, INPUT);

        digitalWrite(ultrasonic[i].trigger, LOW);
    }
}

unsigned int ultrasonic_measure_raw(Ultrasonic ultrasonic)
{
    // Ping: Low for 2..4 us, then high 10 us
    digitalWrite(ultrasonic.trigger, 0);
    usleep(TRIGGER_LOW_DELAY);
    digitalWrite(ultrasonic.trigger, 1);
    usleep(TRIGGER_HIGH_DELAY);
    digitalWrite(ultrasonic.trigger, 0);

    // Previous ping isn't ended
    digitalRead(ultrasonic.echo);

    // Wait for echo
    long int start = micros();
    while (!digitalRead(ultrasonic.echo))
    { }

    // got echo, measuring
    long int echo_start = micros();
    long int time = echo_start;
    while (digitalRead(ultrasonic.echo));
    time = micros();

    return time - echo_start;
}

unsigned int ultrasonic_measure_cm(Ultrasonic ultrasonic)
{
    return ultrasonic_measure_raw(ultrasonic) / ROUNDTRIP_CM;
}

void ultrasonic_test(Ultrasonic ultrasonics[3]) {
    unsigned int d;
    while (1) {
        sleep(1);
        unsigned int frente_cm = ultrasonic_measure_cm(ultrasonics[0]);
        unsigned int direita_cm = ultrasonic_measure_cm(ultrasonics[1]);
        unsigned int esquerda_cm = ultrasonic_measure_cm(ultrasonics[2]);

        printf("Frente = %dcm, Direita = %dcm, Esquerda = %dcm\n", frente_cm, direita_cm, esquerda_cm);
    }
}

#endif  //__ULTRASONIC_IMPLEMENTATION__

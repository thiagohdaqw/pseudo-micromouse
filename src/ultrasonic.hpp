#ifndef __ULTRASONIC_H_INCLUDED__
#define __ULTRASONIC_H_INCLUDED__

#endif  // __ULTRASONIC_H_INCLUDED__

#ifndef __ULTRASONIC_IMPLEMENTATION__
#define __ULTRASONIC_IMPLEMENTATION__

#include <wiringPi.h>
#include <unistd.h>

#define PIN_ECHO 27
#define PIN_TRIGGER 5

#define TRUE (1==1)

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define PING_TIMEOUT 6000
#define ROUNDTRIP_M 5800.0f
#define ROUNDTRIP_CM 58


void ultrasonic_init()
{
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    
    digitalWrite(PIN_TRIGGER, LOW);
}

void ultrasonic_measure_raw(unsigned int max_time_us, unsigned int *time_us)
{
    // Ping: Low for 2..4 us, then high 10 us
    digitalWrite(PIN_TRIGGER, 0);
    usleep(TRIGGER_LOW_DELAY);
    digitalWrite(PIN_TRIGGER, 1);
    usleep(TRIGGER_HIGH_DELAY);
    digitalWrite(PIN_TRIGGER, 0);

    // Previous ping isn't ended
    digitalRead(PIN_ECHO);

    // Wait for echo
    long int start = micros();
    while (!digitalRead(PIN_ECHO))
    { }

    // got echo, measuring
    long int echo_start = micros();
    long int time = echo_start;
    while (digitalRead(PIN_ECHO));
    time = micros();

    *time_us = time - echo_start;
}

void ultrasonic_measure_cm(unsigned int max_distance, unsigned int *distance)
{
    unsigned int time_us;
    ultrasonic_measure_raw(max_distance * ROUNDTRIP_CM, &time_us);
    *distance = time_us / ROUNDTRIP_CM;
}

#endif  //__ULTRASONIC_IMPLEMENTATION__

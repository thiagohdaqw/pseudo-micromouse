import RPi.GPIO as gpio
import time
import os

DELAY = float(os.getenv("DELAY", 2))
VELOCITY_INCREMENT = int(os.getenv("VELOCITY_INCREMENT", 50))

DUTY_MIN = 0
DUTY_MAX = 100

MOTOR_VELOCITY_PIN = int(os.getenv("MOTOR_VELOCITY_PIN", 12))
MOTOR_A_PIN = int(os.getenv("MOTOR_A_PIN", 23))
MOTOR_B_PIN = int(os.getenv("MOTOR_B_PIN", 24))
MOTOR_DIRECTION_PINS = [MOTOR_A_PIN, MOTOR_B_PIN]

FRONT = [gpio.HIGH, gpio.LOW]
BACk = FRONT[::-1]

gpio.setmode(gpio.BCM)
gpio.setup(MOTOR_VELOCITY_PIN, gpio.OUT)
gpio.setup(MOTOR_A_PIN, gpio.OUT)
gpio.setup(MOTOR_B_PIN, gpio.OUT)

motor_velocity = gpio.PWM(MOTOR_VELOCITY_PIN, DUTY_MAX)
motor_velocity.start(DUTY_MIN)

duty = DUTY_MIN
velocity = VELOCITY_INCREMENT
direction = FRONT

for pin, value in zip(MOTOR_DIRECTION_PINS, direction):
    gpio.output(pin, value)

try:
    while True:
        duty = max(min(duty + velocity, DUTY_MAX), DUTY_MIN)
        motor_velocity.ChangeDutyCycle(duty)

        if duty == DUTY_MAX or duty == DUTY_MIN:
            velocity *= -1

        if duty == DUTY_MIN:
            direction = FRONT if direction != FRONT else BACk
            for pin, value in zip(MOTOR_DIRECTION_PINS, direction):
                gpio.output(pin, value)

        print(f"{duty=} {direction=}")
        time.sleep(DELAY)
finally:
    gpio.cleanup()

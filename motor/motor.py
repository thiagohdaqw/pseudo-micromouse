import RPi.GPIO as gpio
import time
import os

DELAY = float(os.getenv("DELAY", 0.1))
VELOCITY_INCREMENT = int(os.getenv("VELOCITY_INCREMENT"), 10)

DUTY_MIN = 30
DUTY_MAX = 100

MOTOR_VELOCITY_PIN = int(os.getenv("MOTOR_VELOCITY_PIN", 14))
MOTOR_A_PIN = int(os.getenv("MOTOR_A_PIN", 15))
MOTOR_B_PIN = int(os.getenv("MOTOR_B_PIN", 16))
MOTOR_DIRECTION_PINS = [MOTOR_A_PIN, MOTOR_B_PIN]

FRONT = [gpio.HIGH, gpio.LOW]
BACk = FRONT[::-1]


gpio.setmode(gpio.BCM)
gpio.setup(MOTOR_VELOCITY_PIN, gpio.OUT)
gpio.setup(MOTOR_A_PIN, gpio.OUT)
gpio.setup(MOTOR_B_PIN, gpio.OUT)

motor_velocity = gpio.PWM(MOTOR_VELOCITY_PIN, DUTY_MIN)

duty = DUTY_MIN
velocity = VELOCITY_INCREMENT
direction = FRONT

for pin, value in zip(MOTOR_DIRECTION_PINS, direction):
    gpio.output(pin, value)

try:
    while True:
        print(f"{duty=}, {direction=}")

        duty = max(min(duty + velocity, DUTY_MAX), DUTY_MIN)
        motor_velocity.ChangeDutyCycle(duty)

        if duty == DUTY_MAX or duty == DUTY_MIN:
            velocity *= -1

        if duty == DUTY_MIN:
            direction = FRONT if direction != FRONT else BACk
            for pin, value in zip(MOTOR_DIRECTION_PINS, direction):
                gpio.output(pin, value)

        time.sleep(DELAY)
finally:
    gpio.cleanup()

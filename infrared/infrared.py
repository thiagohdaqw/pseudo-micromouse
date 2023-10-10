import RPi.GPIO as gpio
import time
import os

INFRARED_PIN = int(os.getenv("INFRARED_PIN", 14))
DELAY = float(os.getenv("DELAY", 0.5))


gpio.setmode(gpio.BCM)
gpio.setup(INFRARED_PIN, gpio.IN)


while True:
    time.sleep(DELAY)
    value = gpio.input(INFRARED_PIN)
    print("Infrared:", "HIGHT" if value == gpio.HIGH else "LOW")

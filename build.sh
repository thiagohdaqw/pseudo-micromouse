#!/bin/bash


g++ src/*.*pp -o main -lm -pthread -lwiringPi `pkg-config --cflags --libs opencv4`
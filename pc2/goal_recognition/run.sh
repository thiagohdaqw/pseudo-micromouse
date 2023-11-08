#!/bin/bash

set -xe

cd yolo

while true; do
    sleep 1;
    ffmpeg -i /dev/video0 -vframes 1 frame.png -y;
    ./call_yolo.out frame.png;
done

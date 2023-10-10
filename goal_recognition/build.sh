#!/bin/bash

set -xe

if ! command -v ffmpeg &> /dev/null
then
    echo "Installing dependencies..."
    sudo apt install ffmpeg
fi

cd yolo

if ! ls files/ | grep "yolov4" &> /dev/null; then
    cd files
    bash download_files.sh
    cd ..
fi

make -B -j2

cp call_yolo.out ../
cd ..

ln -s yolo/yolo_output.png frame_out.png
ln -s yolo/frame.png frame.png

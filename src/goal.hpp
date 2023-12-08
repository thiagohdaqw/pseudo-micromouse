#ifndef __GOALRECOGNITION_H_INCLUDED__
#define __GOALRECOGNITION_H_INCLUDED__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "types.hpp"

using namespace cv;
using namespace dnn;

#define THREADS_MAX 3
#define IMG_HEIGHT 416
#define IMG_WIDTH 416

typedef struct cam_args {
    point *current;
    point goal;
    std::mutex capture;
    pthread_t tid;
    VideoCapture camera;
} TCamArgs;

DetectionModel *model;
std::vector<std::string> classes;

bool goal_recognition(Mat img) {
    std::vector<int> classIds;
    std::vector<float> scores;
    std::vector<Rect> boxes;
    model->detect(img, classIds, scores, boxes, 0.6, 0.4);

    for (int i = 0; i < classIds.size(); i++) {
        if (classes[classIds[i]].compare("person") == 0) {
            return true;
        }
    }
    return false;
}

void goal_init() {
    std::ifstream file("yolo/coco.names");
    std::string line;

    while (std::getline(file, line)) {
        classes.push_back(line);
    }

    Net net = readNetFromDarknet("yolo/yolov4.cfg", "yolo/yolov4.weights");
    model = new DetectionModel(net);
    model->setInputParams(1 / 255.0, Size(IMG_WIDTH, IMG_HEIGHT), Scalar(),
                          true);
}

void *cam_watch(void *args) {
    TCamArgs *cam_args = (TCamArgs *)args;
    char capture_path[1024] = {0};

    VideoCapture camera = cam_args->camera;

    Mat frame;

    while (1) {
        cam_args->capture.lock();
        point current_position = *cam_args->current;

        printf("CAPTURAR\n");
        if (!camera.read(frame)) continue;

        snprintf(capture_path, 1024, "captures/position_%d_%d.png",
                 current_position.first, current_position.second);
        imwrite(capture_path, frame);

        // if (goal_recognition(frame)) {
        //     printf("PESSOA ENCONTRADA\n");
        //     cam_args->goal = current_position;
        //     break;
        // }
    }
    return NULL;
}

void cam_init(TCamArgs *args) {
    system("rm captures/*");

    // goal_init();

    VideoCapture camera(CAP_ANY);

    cv::setNumThreads(THREADS_MAX);
    camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    camera.set(CAP_PROP_FRAME_WIDTH, IMG_WIDTH);
    camera.set(CAP_PROP_FRAME_HEIGHT, IMG_HEIGHT);
    camera.set(CAP_PROP_POS_FRAMES, 1);

    args->camera = camera;
    args->capture.lock();
    pthread_create(&args->tid, NULL, &cam_watch, (void *)args);
}

void cam_test(TCamArgs *args) {
    for (int i = 0; i < 25; i++) {
        sleep(1);
        args->capture.unlock();
    }
}

#endif
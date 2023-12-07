#ifndef __GOALRECOGNITION_H_INCLUDED__
#define __GOALRECOGNITION_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "yolo/call_yolo.hpp"

int goal_recognition(){
    char image[1024] = {0};
    char recognition[1024] = {0};
    int person = 0;
    int i = 1;

    while(!person){
        snprintf(image, 1024, 
            "ffmpeg -i /dev/video0 -vframes 1 ../assets/imagesPath/image%d.png -y;", i++
        );

        printf("Gerando imagem %s\n", image);

        system(image);

        // snprintf(recognition, 1024, "../../assets/image%d.png", i++);

        // printf("Testando reconhecimento de pessoa na imagem %s\n", recognition);

        // person = person_recognition(recognition);

        sleep(1);
    }
    return 0;
}

#endif
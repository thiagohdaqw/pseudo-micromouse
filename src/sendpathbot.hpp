#ifndef __SENDPATHBOT_H_INCLUDED__
#define __SENDPATHBOT_H_INCLUDED__

void send_video_telegram(char *argv[]);

#endif  //__SENDPATHBOT_H_INCLUDED__

#ifndef __SENDPATHBOT_IMPLEMANTATION__
#define __SENDPATHBOT_IMPLEMANTATION__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *video_name = "assets/output.mp4";

void make_video_images(){
    char command[1024] = {0};

    snprintf(command, 1024, 
        "ffmpeg -y -framerate 1 -i assets/imagesPath/image%%d.png -c:v libx264 -r 30 %s",
        video_name
    );

    printf("Gerando o video com as imagens %s\n", command);

    system(command);

    system("rm assets/imagesPath/*");
}

void send_video_telegram(char *argv[]) {
    char *token = argv[0];
    char *chat_id = argv[1];
    char request[1024] = {0};

    make_video_images();

    printf("Enviando mensagem\n");

    snprintf(request, 1024, "curl -X POST -F 'video=@%s' 'https://api.telegram.org/bot%s/sendVideo?chat_id=%s'",
        video_name,
        token,
        chat_id
    );

    printf("Enviando request %s\n", request);

    system(request);
}

#endif  //__SENDPATHBOT_IMPLEMANTATION__
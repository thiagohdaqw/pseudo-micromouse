#ifndef __SENDPATHBOT_H_INCLUDED__
#define __SENDPATHBOT_H_INCLUDED__

void video_send_telegram(char *token, char *chat_id, char *video_path);
void video_make_from_images(char *output_path);

#endif  //__SENDPATHBOT_H_INCLUDED__

#ifndef __SENDPATHBOT_IMPLEMANTATION__
#define __SENDPATHBOT_IMPLEMANTATION__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void video_make_from_images(char *output_path){
    char command[1024] = {0};

    snprintf(command, 1024, 
        "ffmpeg -y -framerate 1 -i ../assets/imagesPath/image%%d.png -c:v libx264 -r 30 %s",
        output_path
    );

    printf("Gerando o video com as imagens %s\n", command);

    system(command);

    // system("rm assets/imagesPath/*");
}

void video_send_telegram(char *chat_id, char *token, char *video_path) {
    char request[1024] = {0};

    printf("Enviando mensagem\n");

    snprintf(request, 1024, "curl -X POST -F 'video=@%s' 'https://api.telegram.org/bot%s/sendVideo?chat_id=%s'",
        video_path,
        token,
        chat_id
    );

    printf("Enviando request %s\n", request);

    system(request);
}

#endif  //__SENDPATHBOT_IMPLEMANTATION__
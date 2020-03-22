//
// Created by acer on 2020/3/21.
//

#ifndef MY_APPLICATION_VIDEOCHANNEL_H
#define MY_APPLICATION_VIDEOCHANNEL_H

#include "BaseChannel.h"

extern "C"{
#include <libavcodec/avcodec.h>
};


class VideoChannel :BaseChannel{
public:
    VideoChannel();

    VideoChannel(int streamIndex, AVCodecContext *avCodecContext);

    ~VideoChannel();


    void start();


    void stop();

private:
    pthread_t pid_video_decode;
};


#endif //MY_APPLICATION_VIDEOCHANNEL_H

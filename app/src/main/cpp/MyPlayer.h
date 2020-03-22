//
// Created by acer on 2020/3/18.
//

#ifndef MY_APPLICATION_MYPLAYER_H
#define MY_APPLICATION_MYPLAYER_H

#include <cstring>
#include <pthread.h>
#include "VideoChannel.h"
#include "AudioChannel.h"
#include "JNICallback.h"

extern "C"{
#include <libavformat/avformat.h>
};
class MyPlayer {

public:
    MyPlayer();

    virtual ~MyPlayer();
    MyPlayer(const char *data_source,JNICallback *pCallback);

    void prepare();
    void prepare_();

    void start();
    void start_();

private:
     char* dataSource= 0;
     pthread_t pid_prepare=0;
     AVFormatContext* avFormatContext=0;
     AudioChannel* audioChannel=0;
     VideoChannel* videoChannel=0;
     JNICallback * jniCallback=0;
     pthread_t pid_start=0;
     int isPlayer=0;
};


#endif //MY_APPLICATION_MYPLAYER_H

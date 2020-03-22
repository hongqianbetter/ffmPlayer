//
// Created by acer on 2020/3/21.
//

#ifndef MY_APPLICATION_BASECHANNEL_H
#define MY_APPLICATION_BASECHANNEL_H


#include "safe_queue.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

class BaseChannel {
public:
    int stream_index;
    AVCodecContext *avCodecContext;

    BaseChannel(int stream_index, AVCodecContext *avCodecContext) {
     this->stream_index=stream_index;
     this->avCodecContext=avCodecContext;
    }
    ~BaseChannel(){

    }

    //视频aac 视频h264
    SafeQueue<AVPacket *> packets;

    //音频 pcm 视频 yuv
   SafeQueue<AVFrame*> frames;

   bool  isPlaying=1;
};


#endif //MY_APPLICATION_BASECHANNEL_H

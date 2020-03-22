//
// Created by acer on 2020/3/21.
//

#ifndef MY_APPLICATION_AUDIOCHANNEL_H
#define MY_APPLICATION_AUDIOCHANNEL_H


#include "BaseChannel.h"

class AudioChannel:BaseChannel {
public:
   AudioChannel(int stream_index,AVCodecContext* avCodecContext);

    void start();
};


#endif //MY_APPLICATION_AUDIOCHANNEL_H

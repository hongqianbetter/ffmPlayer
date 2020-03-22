//
// Created by acer on 2020/3/21.
//

#include "VideoChannel.h"

VideoChannel::VideoChannel(int streamIndex, AVCodecContext *avCodecContext)
:BaseChannel(streamIndex,avCodecContext) {

}
VideoChannel::~VideoChannel() {}

//从队列中取出,为解码的数据
//1 解码 2 播放
void VideoChannel::start() {
  //存放未解码的队列 开始工作了
  packets.setFlag(1);

  //存放已解码的队列开始工作了
  frames.setFlag(1);

  //解码的线程
  pthread_create(&pid_video_decode,0,task_vidio_decode,this);
}

void VideoChannel::stop() {

}

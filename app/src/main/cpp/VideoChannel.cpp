//
// Created by acer on 2020/3/21.
//

#include <libswscale/swscale.h>
#include "VideoChannel.h"

VideoChannel::VideoChannel(int streamIndex, AVCodecContext *avCodecContext)
        : BaseChannel(streamIndex, avCodecContext) {

}

VideoChannel::~VideoChannel() {}

void *task_video_decode(void *pVoid) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(pVoid);
    videoChannel->video_decode();
    return 0;
}

//从队列中取出,为解码的数据
//1 解码 2 播放
void VideoChannel::start() {
    //存放未解码的队列 开始工作了
    packets.setFlag(1);

    //存放已解码的队列开始工作了
    frames.setFlag(1);

    //解码的线程
    pthread_create(&pid_video_decode, 0, task_video_decode, this);
}

void VideoChannel::stop() {

}

//真正的异步解码
void VideoChannel::video_decode() {
    AVPacket *packet;
    while(isPlaying){
        int ret=this->packets.pop(packet);

        if (!isPlaying){
            break;
        }
        if (!ret){
            continue;
        }
        //走到这里,就证明去到了 为解码的视频数据包
        ret=avcodec_send_packet(this->avCodecContext,packet);
        if (ret){
            break;//失败了
        }
        AVFrame * avFrame=av_frame_alloc();
        ret=avcodec_receive_frame(this->avCodecContext,avFrame);
        if (ret == AVERROR(EAGAIN)){
            //代表取帧不完整,I帧完整的1帧,其他而是P帧,B帧
            continue;
        } else if (ret!=0){
            //TODO 以后做释放工作
            break;
        }
        //终于取到了原始数据
        this->frames.push(avFrame);

        //TODO 出了循环 释放

    }

    void VideoChannel::video_player(){

    }
}

//异步 真正的播放
void VideoChannel::video_player() {
    //YUV 原始的视频数据 ,不能显示到屏幕上
    //yuv -----rgba (才能真正的显示到屏幕上)
    SwsContext * swsContext=sws_getContext(
            //原始的宽高,格式
           this->avCodecContext->width,
           this->avCodecContext->height,
           this->avCodecContext->pix_fmt,
           //转化的宽高格式
           this->avCodecContext->width,
           this->avCodecContext->height,
           AV_PIX_FMT_RGBA,
           //播放速率
           SWS_BILINEAR,NULL,NULL,NULL
    );

    AVFrame *frame=0;
    while (isPlaying){

    }



}

//
// Created by acer on 2020/3/18.
//

#include "MyPlayer.h"

MyPlayer::MyPlayer(const char *data_source, JNICallback *jniCallback) {
    this->dataSource = new char[strlen(data_source) + 1];
    strcpy(this->dataSource, data_source);
    this->jniCallback = jniCallback;
}

MyPlayer::~MyPlayer() {
    if (this->dataSource) {
        delete this->dataSource;
        //delete 后面加指针 指回收指针指向的内存,但是指针指向的值不变
        this->dataSource = nullptr;
        delete this->jniCallback;
        this->jniCallback = nullptr;
    }
}

void *customTaskPrepareThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);
    myPlayer->prepare_();

    return 0;
}

void MyPlayer::prepare() {
    pthread_create(&this->pid_prepare, 0, customTaskPrepareThread, this);
}

void MyPlayer::prepare_() { //异步线程
//拆包裹
    this->avFormatContext = avformat_alloc_context();

    AVDictionary *avDictionary = nullptr; //此字典能够决定我们打开的参数
    av_dict_set(&avDictionary, "timeout", "500000", 0); //单位是微秒,如果手机卡尽量设置大一些
    int ret = avformat_open_input(&this->avFormatContext, this->dataSource, 0, &avDictionary);
    av_dict_free(&avDictionary);
    if (ret) {
        //些jni回调通知上层 播放流是损坏的
        this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }
//寻找多媒体格式中的 音频 视频 字幕
    avformat_find_stream_info(this->avFormatContext, 0);//为什么不给字典,不需要设置额外配置
    if (ret < 0) {
        //些jni回调通知上层
        this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        return;
    }

    //媒体上下文,就有了丰富的值了  音频压缩格式h264 视频压缩格式aac
    for (int i = 0; i < this->avFormatContext->nb_streams; ++i) {
        //获取音频流,视频流
        AVStream *stream = this->avFormatContext->streams[i];
        //得到解码器Id,为了获取解码器
        AVCodecParameters *codecParameters = stream->codecpar;
        enum AVCodecID codecId = codecParameters->codec_id;
        AVCodec *codec = avcodec_find_decoder(codecId);
        if (!codec) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }
        //为了后续解码方便,还需要解码器上下文
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }
        //些jni回调通知上层 播放流是损坏的
        ret = avcodec_parameters_to_context(codecContext, codecParameters);
        if (ret < 0) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        //打开解码器
        ret = avcodec_open2(codecContext, codec, 0);
        if (ret) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        //解码器打开成功.就能去使用解码器
        if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->videoChannel = new VideoChannel(i, codecContext);
        } else if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->audioChannel = new AudioChannel(i, codecContext);
        }

    }

    //告诉Java层,我准备好了


}

void *customTaskStartThread(void *pVoid) {
    MyPlayer *player = static_cast<MyPlayer *>(pVoid);
    player->start_();

    return 0;
}

void MyPlayer::start() {
//开始播放,需要异步
//让视频和音频两个通道运行起来
    isPlayer = 9;
    if (this->videoChannel) {
        this->videoChannel->start();
    }

    if (this->audioChannel) {
        this->audioChannel->start();
    }
    //开启线程 把未解码的流数据 音频 视频 字幕放入队列
    pthread_create(&pid_start, 0, customTaskStartThread, this);

}

void MyPlayer::start_() {
    while (this->isPlayer) {
        //为解码的格式是,保存在AVPacket里面的
        AVPacket *packet = av_packet_alloc();
        int ret = av_read_frame(this->avFormatContext, packet);
        if (!ret) { // 0 isOK
            //找到视频包
            if (videoChannel && videoChannel->stream_index == packet->stream_index) {
                this->videoChannel->packets.push(packet);
            }
            //找到视频包
            if (audioChannel && audioChannel->stream_index == packet->stream_index) {
                this->audioChannel->packets.push(packet);
            }
        } else if (ret == AVERROR_EOF){ //文件末尾 读完了的意思

        }else{
            //代表失败
            break;
        }
    }

    //释放工作
    isPlayer==0;
    videoChannel->stop();
}





package com.ldkeji.myapplication;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MyPlayer implements SurfaceHolder.Callback {
    static {
        System.loadLibrary("my-player");
    }
    private String dataSource; //网络直播流,本地流

    private SurfaceHolder surfaceHolder;

    public MyPlayer() {
    }

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void setSurfaceView(SurfaceView surfaceView){
        //每次设置surfaceView的时候,判断holder释放被清除
        if(null!=surfaceHolder) {
            surfaceHolder.removeCallback(this);
        }
        surfaceHolder= surfaceView.getHolder();
        surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            setSurfaceNative(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }


    public void prepare(){
        prepareNative(this.dataSource);
    }

    public void start(){
        startNative();
    }

    public void stop(){
        stopNative();
    }

    public void release(){
        releaseNative();
    }

    public native  void prepareNative(String dataSource);

    public native void startNative();

    public native void stopNative();

    public native  void releaseNative();

    public native void setSurfaceNative(Surface surface);

    interface OnPreparedListener{
        void onPrepared();
        void onError(String errorText);
    }

    private OnPreparedListener onPreparedListener;

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    //给native调用的函数

    public void onPrepared(){
        if(onPreparedListener!=null) {
            this.onPreparedListener.onPrepared();
        }
    }

    public void onError(int errorCode){
        if(onPreparedListener!=null) {
            String errorText =  null;
            switch (errorCode) {
                case Flags.FFMPEG_ALLOC_CODEC_CONTEXT_FAIL:
                    errorText = "无法根据解码器创建上下文";
                    break;
                case Flags.FFMPEG_CAN_NOT_FIND_STREAMS:
                    errorText = "找不到媒体流信息";
                    break;
                case Flags.FFMPEG_CAN_NOT_OPEN_URL:
                    errorText = "打不开媒体数据源";
                    break;
                case Flags.FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL:
                    errorText = "根据流信息 配置上下文参数失败";
                    break;
                case Flags.FFMPEG_FIND_DECODER_FAIL:
                    errorText = "找不到解码器";
                    break;
                case Flags.FFMPEG_NOMEDIA:
                    errorText = "没有音视频";
                    break;
                case Flags.FFMPEG_READ_PACKETS_FAIL:
                    errorText = "读取媒体数据包失败";
                    break;
                default:
                    errorText = "未知错误，自己去检测你的垃圾代码...";
                    break;
            }
            this.onPreparedListener.onError(errorText);
        }
    }
}

#include <jni.h>
#include <string>
#include "MyPlayer.h"
#include "JNICallback.h"

extern "C" {
#include <libavutil/avutil.h>
}


JavaVM *javaVm = 0;
JNICallback *jniCallback = 0;

int JNI_OnLoad(JavaVM *javaVm, void *pVoid) {
    ::javaVm = javaVm;

    return JNI_VERSION_1_6; //这里和线程一样都要返回,不然会报错
}

MyPlayer *player=NULL; //全局变量 不会被回收
extern "C"
JNIEXPORT void JNICALL
Java_com_ldkeji_myapplication_MyPlayer_prepareNative(JNIEnv *env, jobject thiz,
                                                     jstring dataSources) {
    JNICallback *jniCallback = new JNICallback(
            javaVm, env, thiz);
    const char *datasource = env->GetStringUTFChars(dataSources, NULL);
    player = new MyPlayer(datasource, jniCallback);
    player->prepare();
    env->ReleaseStringUTFChars(dataSources, datasource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ldkeji_myapplication_MyPlayer_startNative(JNIEnv *env, jobject thiz) {
    if (player!=NULL){
        player->start();
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ldkeji_myapplication_MyPlayer_stopNative(JNIEnv *env, jobject thiz) {
    // TODO: implement stopNative()
}extern "C"
JNIEXPORT void JNICALL
Java_com_ldkeji_myapplication_MyPlayer_releaseNative(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseNative()
}extern "C"
JNIEXPORT void JNICALL
Java_com_ldkeji_myapplication_MyPlayer_setSurfaceNative(JNIEnv *env, jobject thiz,
                                                        jobject surface) {
    // TODO: implement setSurfaceNative()
}
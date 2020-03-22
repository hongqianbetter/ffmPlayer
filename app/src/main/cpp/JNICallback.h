//
// Created by acer on 2020/3/21.
//

#ifndef MY_APPLICATION_JNICALLBACK_H
#define MY_APPLICATION_JNICALLBACK_H

#include <jni.h>
#include "macro.h"
class JNICallback {

public:
    JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance);

    ~JNICallback();

    //具体回调给上层
    void onPrepared(int thread_mode);

    //具体回调给上层的函数(失败)
    void onErrorAction(int threadMode, int error_code);

private:
    JavaVM *javaVm = 0;
    JNIEnv *env = 0;
    jobject instance;
    jmethodID jmd_prepared;
    jmethodID jmd_error;
};


#endif //MY_APPLICATION_JNICALLBACK_H

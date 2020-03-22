//
// Created by acer on 2020/3/21.
//

#include "JNICallback.h"

void JNICallback::onPrepared(int thread_mode) {
    if (thread_mode == THREAD_MAIN) { //主线程
        //调用上层
        this->env->CallVoidMethod(this->instance, this->jmd_prepared);
    } else if (thread_mode == THREAD_CHILD) {
        JNIEnv *jniEnv = nullptr;
        int ret = this->javaVm->AttachCurrentThread(&jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }
        this->env->CallVoidMethod(this->instance, this->jmd_error);
        this->javaVm->DetachCurrentThread();
    }
}

void JNICallback::onErrorAction(int threadMode, int error_code) {

}

JNICallback::JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance) {
    this->javaVm = javaVm;
    this->env = env; //跨线程 不能使用env
    this->instance = env->NewGlobalRef(instance); //全局引用
    jclass myPlayerClass = env->GetObjectClass(this->instance);

    const char *sig = "()V";
    this->jmd_prepared = env->GetMethodID(myPlayerClass, "onPrepared", sig);

    sig = "(I)V";
    this->jmd_error = env->GetMethodID(myPlayerClass, "onError", sig);
}

JNICallback::~JNICallback() {
    this->javaVm = 0;
    this->env->DeleteGlobalRef(this->instance);
    this->instance = 0;
    env = 0;
}

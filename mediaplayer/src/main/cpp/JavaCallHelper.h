//
// Created by Administrator on 2019/11/25.
//

#ifndef NATIVEDEMO_JAVACALLHELPER_H
#define NATIVEDEMO_JAVACALLHELPER_H

#include <jni.h>
#include "macro.h"

class JavaCallHelper{
public:
    JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject instance);

    ~JavaCallHelper();

    void onError(int thread, int code);

    void onPrepare(int thread);

    void onProgress(int thread, int progress);

private:
    JavaVM *javaVM; // 需要在子线程中，反射调用 Java 方法
    JNIEnv *env; // 主线程的 JNIEnv
    jobject jobj;
    jmethodID jmid_prepare;
    jmethodID jmid_error;
    jmethodID jmid_progress;
};

#endif //NATIVEDEMO_JAVACALLHELPER_H

//
// Created by Administrator on 2019/11/25.
//

#include "JavaCallHelper.h"

JavaCallHelper::JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject instance)
        : javaVM(_javaVM), env(_env) {

    jobj = env->NewGlobalRef(instance);
    jclass jclazz = env->GetObjectClass(jobj);

    // jmid_error  ArtMethod
    jmid_error = env->GetMethodID(jclazz, "onError", "(I)V");
    jmid_prepare = env->GetMethodID(jclazz, "onPrepare", "()V");
    jmid_progress = env->GetMethodID(jclazz, "onProgress", "(I)V");

}

JavaCallHelper::~JavaCallHelper() {
    env->DeleteGlobalRef(jobj);
}

void JavaCallHelper::onError(int thread, int code) {
    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVM->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_error, code);
        javaVM->DetachCurrentThread();
    } else if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_error, code);
    }
}

void JavaCallHelper::onPrepare(int thread) {
    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVM->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepare);
        javaVM->DetachCurrentThread();
    } else if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_prepare);
    }
}

void JavaCallHelper::onProgress(int thread, int progress){
    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVM->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_progress, progress);
        javaVM->DetachCurrentThread();
    } else if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_progress, progress);
    }
};

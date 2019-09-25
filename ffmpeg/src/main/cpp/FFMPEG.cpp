//
// Created by Administrator on 2019/9/25.
//

#include "log.h"
#include "libavcodec/avcodec.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_init(JNIEnv *env, jclass clazz) {

    LOGW("invoke init");
}

JNIEXPORT jstring JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_getVer(JNIEnv *env, jclass clazz) {
    const char *ver = "hello"; // av_version_info();
//    const char *ver = av_version_info();
    LOGI("%s", ver);
    jstring pJstring = env->NewStringUTF(ver);

    return pJstring;
}


#ifdef __cplusplus
}
#endif
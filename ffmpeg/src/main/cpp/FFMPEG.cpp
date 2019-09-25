//
// Created by Administrator on 2019/9/25.
//

#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_init(JNIEnv *env, jclass clazz) {

    LOGW("invoke init");
}


#ifdef __cplusplus
}
#endif
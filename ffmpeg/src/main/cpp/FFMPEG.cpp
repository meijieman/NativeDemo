//
// Created by Administrator on 2019/9/25.
//

#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"// 封装格式处理
#include "libswresample/swresample.h"// 重采样

JNIEXPORT void JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_init(JNIEnv *env, jclass clazz) {

    LOGW("invoke init");
}

JNIEXPORT jstring JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_getVer(JNIEnv *env, jclass clazz) {
//    const char *ver = "hello"; // av_version_info();
    const char *ver = av_version_info();
    LOGI("%s", ver);
    jstring pJstring = env->NewStringUTF(ver);

    return pJstring;
}


JNIEXPORT jint JNICALL
Java_com_major_ffmpeg_FFMPEGHelper_decodeAudio(JNIEnv *env, jclass clazz, jstring input_,
                                               jstring output_) {

    const char *input = env->GetStringUTFChars(input_, nullptr);
    const char *output = env->GetStringUTFChars(output_, nullptr);

    LOGI("input %s, output %s", input, output);

    // 1 初始化网络模块
    avformat_network_init();

    AVFormatContext *pFormatContext = avformat_alloc_context();

    // 2 打开音频
    int ret = avformat_open_input(&pFormatContext, input, nullptr, nullptr);

    if (ret < 0) {
        LOGE("无法打开音频文件");
        return ret;
    }


    return 0;

}

#ifdef __cplusplus
}
#endif
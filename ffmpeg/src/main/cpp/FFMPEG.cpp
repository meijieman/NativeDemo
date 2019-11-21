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
        LOGE("无法打开音频文件，文件存在吗？");
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return ret;
    }

    // 3 获取音频文件信息
    ret = avformat_find_stream_info(pFormatContext, nullptr);
    if (ret < 0) {
        LOGE("无法获取音频文件信息");
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return ret;
    }

    int audio_stream_idx = -1; // 音频流的索引位置

    for (int i = 0; i < pFormatContext->nb_streams; ++i) {
        if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_idx = i;
            break;
        }
    }
    LOGW("音频流的索引位置 %d ", audio_stream_idx);
    if (audio_stream_idx == -1) {
        LOGE("找不到音频流");
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    // 4 获取解码器
    AVCodecParameters *pParameters = pFormatContext->streams[audio_stream_idx]->codecpar;
    AVCodec *pCodec = avcodec_find_decoder(pParameters->codec_id);
    if (pCodec == nullptr) {
        LOGE("找不到解码器");
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    // 解码器上下文
    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec); // 释放 avcodec_free_context
    if (pCodecContext == nullptr) {
        LOGE("找不到解码器上下文");
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    // 复制解码器的参数到解码器上下文
    ret = avcodec_parameters_to_context(pCodecContext, pParameters);
    if(ret < 0) {
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);
        
        return  -1;
    }
    
    // 5 打开解码器
    ret = avcodec_open2(pCodecContext, pCodec, nullptr);
    if(ret<0){
        LOGE("打开解码器失败");
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    // 初始化数据包
    AVPacket *pPacket = av_packet_alloc(); // 释放av_packet_free
    if (pPacket == nullptr) {
        LOGE("分配 AVPacket 内存失败");
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    AVFrame *pFrame = av_frame_alloc(); // 释放 av_frame_free
    if (pFrame == nullptr) {
        LOGE("分配 AVFrame 内存失败");
        av_packet_free(&pPacket);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    SwrContext *pSwrContext = swr_alloc();
    if (pSwrContext == nullptr) {
        LOGE("分配 SwrContext 失败");
        av_frame_free(&pFrame);
        av_packet_free(&pPacket);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);
        return -1;
    }
    
    
    
    
    
    
    
    
    

    return 0;

}

#ifdef __cplusplus
}
#endif
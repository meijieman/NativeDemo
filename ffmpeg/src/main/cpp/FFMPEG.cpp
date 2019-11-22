//
// Created by Administrator on 2019/9/25.
//

#include "log.h"

#define MAX_AUDIO_FRAME_SIZE (44100 * 2)

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
    if (ret < 0) {
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    // 5 打开解码器
    ret = avcodec_open2(pCodecContext, pCodec, nullptr);
    if (ret < 0) {
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

    // 声道布局
    uint64_t in_ch_layout = pCodecContext->channel_layout;
    // 采样格式
    enum AVSampleFormat in_sample_fmt = pCodecContext->sample_fmt;
    // 采样率
    int in_sample_rate = pCodecContext->sample_rate;

    // 输出
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    const int out_sample_rate = 44100;
    swr_alloc_set_opts(pSwrContext, out_ch_layout, out_sample_fmt, out_sample_rate,
                       in_ch_layout, in_sample_fmt, in_sample_rate, 0, nullptr);

    swr_init(pSwrContext);

    auto *out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE);

    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);
    LOGD("out_channel_nb %d", out_channel_nb);

    // 输出文件
    FILE *fp_pcm = fopen(output, "wb");
    if (fp_pcm == nullptr) {
        LOGE("%s 打开失败", output);
        av_freep(out_buffer);
        swr_free(&pSwrContext);
        av_frame_free(&pFrame);
        av_packet_free(&pPacket);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);
        env->ReleaseStringUTFChars(input_, input);
        env->ReleaseStringUTFChars(output_, output);

        return -1;
    }

    int frame_count = 0;
    while (av_read_frame(pFormatContext, pPacket) >= 0) {// 返回下一帧
        if (pPacket->stream_index == audio_stream_idx) {
            // 7 解码一帧音频 AVPacket（压缩数据） -> AVFrame（未压缩数据）
            // 7.1 AVPacket 发送给解码器
            ret = avcodec_send_packet(pCodecContext, pPacket);
            if (ret < 0) {
                LOGE("将数据包发送到解码器时出错 %d ", ret);
                if (ret == AVERROR(EAGAIN)) {
                    // -11
                    LOGE("当前状态不接受输入");
                } else if (ret == AVERROR_EOF) {
                    // -541478725
                    LOGE("已刷新解码器，并且不会向其发送新数据包");
                    continue;
                } else if (ret == AVERROR(EINVAL)) {
                    // -22
                    LOGE("解码器未打开，或者它是编码器，或者需要刷新");
                    continue;
                } else if (ret == AVERROR(ENOMEM)) {
                    // -12
                    LOGE("无法将数据包添加到内部队列");
                    continue;
                } else {
                    // 第一帧会返回-1094995529，AVERROR_INVALIDDATA，因为大多是歌手信息之类
                    LOGE("其他错误");
                    continue;
                }
            }

            // 7.2 从解码器得到 AVFrame 作为输出数据，视频帧或者音频帧，取决于解码器类型
            ret = avcodec_receive_frame(pCodecContext, pFrame);// 从解码器接收帧
            if (ret < 0) {
                LOGE("从解码器接收帧时出错 %d ", ret);
                if (ret == AVERROR(EAGAIN)) {// -11
                    // 需要 avcodec_send_packet
                    LOGE("当前状态不接受输出");
                } else if (ret == AVERROR_EOF) {// -541478725
                    LOGE("解码器已完全刷新，并且将不再有输出帧");
                } else if (ret == AVERROR(EINVAL)) {// -22
                    LOGE("解码器未打开，或者它是编码器");
                } else {
                    LOGE("其它错误");
                }
                break;
            } else {
                // pFrame中就得到了一帧音频数据，存储着解码后的PCM数据
                LOGD("解码：%d", ++frame_count);

                ret = swr_convert(pSwrContext,// 重采样上下文
                                  &out_buffer,// 输出缓冲区
                                  MAX_AUDIO_FRAME_SIZE,// 每通道采样的可用空间量
                                  (const uint8_t **) pFrame->data,// 输入缓冲区
                                  pFrame->nb_samples);// 一个通道中可用的输入采样数量
                if (ret < 0) {
                    LOGE("转换时出错");
                } else {
                    // 获取给定音频参数所需的缓冲区大小
                    int out_buffer_size = av_samples_get_buffer_size(nullptr,
                                                                     out_channel_nb,// 输出的声道个数
                                                                     pFrame->nb_samples,// 一个通道中音频采样数量
                                                                     out_sample_fmt,// 输出采样格式16bit
                                                                     1);// 缓冲区大小对齐（0 = 默认值，1 = 不对齐）
                    // 10.输出PCM文件，1代表缓冲区最小的单元，音频流的单位是字节，所以是1，如果是像素，那就是4
                    fwrite(out_buffer, 1, static_cast<size_t>(out_buffer_size), fp_pcm);
                }
            }
        }
    }

    // 关闭文件
    fclose(fp_pcm);
    // 释放AVFrame
    av_frame_free(&pFrame);
    // 释放AVPacket
    av_packet_free(&pPacket);
    // 释放缓冲区
    av_freep(out_buffer);
    // 释放重采样上下文
    swr_free(&pSwrContext);
    // 关闭解码器
    avcodec_free_context(&pCodecContext);
    // 关闭AVFormatContext
    avformat_close_input(&pFormatContext);
    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);

    return 0;

}

#ifdef __cplusplus
}
#endif
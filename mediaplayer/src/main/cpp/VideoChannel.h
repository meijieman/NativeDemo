//
// Created by Administrator on 2019/11/26.
//

#ifndef NATIVEDEMO_VIDEOCHANNEL_H
#define NATIVEDEMO_VIDEOCHANNEL_H

#include "AudioChannel.h"

// 定义接口
typedef void (*RenderFrame) (uint8_t *, int, int, int);

class VideoChannel : public BaseChannel{

public:
    VideoChannel(int id, JavaCallHelper *javaCallHelper, AVCodecContext *avCodecContext,
                 AVRational time_base);

    ~VideoChannel();

    void start(); // 开启解码和播放线程

    void stop();

    void synchronizeFrame();

    void setRenderCallback(RenderFrame renderFrame);

    void setFPS(double fps);

    void setAudioChannel(AudioChannel *audioChannel);

private:
    pthread_t pid_video_decode;// 解码线程
    pthread_t pid_synchronize; // 播放线程
    RenderFrame renderFrame;
    double fps;
    AudioChannel *pAudioChannel = nullptr;
};

#endif //NATIVEDEMO_VIDEOCHANNEL_H

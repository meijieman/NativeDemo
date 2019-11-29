//
// Created by Administrator on 2019/11/26.
//

#ifndef NATIVEDEMO_MYFFMPEG_H
#define NATIVEDEMO_MYFFMPEG_H


#include <pthread.h>
#include <android/log.h>
#include "VideoChannel.h"
#include "AudioChannel.h"


extern "C" {
#include "libavformat/avformat.h"
};

/**
 * 控制层
 */
class MyFFmpeg {
public:
    MyFFmpeg(JavaCallHelper *javaCallHelper, const char *dataSource);

    ~MyFFmpeg();

    void prepare();

    void prepareFFmpeg();

    void start();

    void getPacket();

    void setRenderCallback(RenderFrame renderFrame); // 接口，将视频回调 player 中的ANativeWindow

    void stop();

    pthread_t pid_prepare; // 初始化线程，初始化完成后销毁
    pthread_t pid_decode; // 解码线程

    VideoChannel *pVideoChannel = nullptr;
    AudioChannel *pAudioChannel = nullptr;

    AVFormatContext *pFormatContext = nullptr;

private:
    bool isPlaying;
    pthread_t  pid_stop;
    char *url;
    JavaCallHelper *pJavaCallHelper = nullptr;

    RenderFrame  renderFrame;
};


#endif //NATIVEDEMO_MYFFMPEG_H

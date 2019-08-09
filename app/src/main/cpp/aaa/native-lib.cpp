#include <jni.h>
#include <string>

//{sdk-path}/ndk-bundle/sysroot/usr/include/android
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <android/bitmap.h>

#include "../bbb/stackblur.h"   // 在第一步中创建的.h头文件，下面可以调用里面的方法
#include "../bbb/stackblur.c"

#include "test.cpp"

// 定义输出的TAG
#define LOG_TAG "log_native"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace std;

int add(int a, int b) {

    return a + b;
}


extern "C" JNIEXPORT jstring
JNICALL
Java_com_major_demo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint
JNICALL
Java_com_major_demo_MainActivity_nativeAdd(JNIEnv *env, jobject instance, jint a, jint b) {

    int i = add(a, b);

    LOGI("nativeAdd 输入值 %d + %d = %d", a, b, i);

    return i;

}


extern "C" JNIEXPORT void
JNICALL
Java_com_major_demo_MainActivity_log(JNIEnv *env, jobject instance) {
    LOGV("进入 log 方法");
    LOGD("进入 log 方法");
    LOGI("进入 log 方法");
    LOGW("进入 log 方法");
    LOGE("进入 log 方法");

    Stud stud;
    stud.setAge(22);
    stud.setName("major");
    stud.getAge();
//
    stud.printStd();

    std::string name = "黎明";
    string hia = "hiahia";
//    LOGW("%s", hia);


    short int a = 0;


    
}

void *posix_run(void *arg) {
    LOGI("runing...");
//    int i = 0;
    for(int i = 0; i < 10; i++){
        sleep(1);
        LOGI("once %d", i);
    }
    LOGI("runing end...");

    return NULL;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_major_demo_MainActivity_runThread(JNIEnv *env, jobject instance) {

    pthread_t tid;

    //创建子线程
    /*
    第一个参数为指向线程标识符的指针。
    第二个参数用来设置线程属性。
    第三个参数是线程运行函数的地址。
    最后一个参数是运行函数的参数。

    若成功则返回0，否则返回出错编号
     */
    int rst = pthread_create(&tid, NULL, posix_run, NULL);
    if (rst == 0) {
        LOGI("成功");
    }
    LOGW("线程id %ld, rst %d", tid, rst);

    //阻塞主线程等待子线程结束
//    pthread_join(tid, NULL);

    LOGW("end cpp");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_major_demo_MainActivity_blurBitmap(JNIEnv *env, jclass obj, jobject bitmapIn, jint r) {

    AndroidBitmapInfo infoIn;
    void *pixels;

    // 获取bitmap的信息
    if (AndroidBitmap_getInfo(env, bitmapIn, &infoIn) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGD("AndroidBitmap_getInfo failed!");
        return;
    }
    // 检测bitmap是不是这两种格式，因为算法中只有对这两种图片会做处理
    if (infoIn.format != ANDROID_BITMAP_FORMAT_RGBA_8888 && infoIn.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGD("Only support ANDROID_BITMAP_FORMAT_RGBA_8888 and ANDROID_BITMAP_FORMAT_RGB_565");
        return;
    }
    // 锁定图片
    if (AndroidBitmap_lockPixels(env, bitmapIn, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGD("AndroidBitmap_lockPixels failed!");
        return;
    }

    // 得到宽高
    int h = infoIn.height;
    int w = infoIn.width;
    if (infoIn.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        // 调用stackblur.c中的blur_ARGB_8888()或blur_RGB_565()
        pixels = blur_ARGB_8888((int *) pixels, w, h, r);
    } else if (infoIn.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        pixels = blur_RGB_565((short *) pixels, w, h, r);
    }
    // 对应上面的AndroidBitmap_lockPixels（）
    AndroidBitmap_unlockPixels(env, bitmapIn);

}
#include <string>

//{sdk-path}/ndk-bundle/sysroot/usr/include/android
#include <pthread.h>
#include <unistd.h>
#include <android/bitmap.h>

#include "../bbb/stackblur.h"   // 在第一步中创建的.h头文件，下面可以调用里面的方法
#include "../bbb/stackblur.c"

#include "test.cpp"
#include "../log.h"

#include "HttpConnect.h"

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
    for (int i = 0; i < 10; i++) {
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
    LOGD("format %d", infoIn.format);
    if (infoIn.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        infoIn.format != ANDROID_BITMAP_FORMAT_RGB_565) {
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

extern "C"
JNIEXPORT void JNICALL
Java_com_major_demo_MainActivity_http(JNIEnv *env, jobject thiz) {

    HttpConnect *http = new HttpConnect();
//    http->getData("www.baidu.com", "/", "id=liukang&pw=123");
    // http://api.wangshuwen.com/ip2Location?ip=113.102.166.83
    http->getData("api.wangshuwen.com", "/ip2Location", "ip=113.102.166.83");

//    http->postData("127.0.0.1", "/login", "id=liukang&pw=123");
}

void test_find();

void test_split();

void test_trim();

void test_cast();

void test_strncmp();

extern "C"
JNIEXPORT void JNICALL
Java_com_major_demo_MainActivity_test(JNIEnv *env, jobject thiz) {

//    test_find();
//    test_split();
//    test_trim();
//    test_cast();
    test_strncmp();
}

void test_find() {
    // find函数返回类型 size_type
    string s("1a2b3c4d5e6f7jkg8h9i1a2b3c4d5e6f7g8ha9i");
    string flag;
    string::size_type position;
    //find 函数 返回jk 在s 中的下标位置
    position = s.find("jk");
    //如果没找到，返回一个特别的标志c++中用npos表示，我这里npos取值是4294967295，
    if (position != string::npos) {
        LOGD("position is : %d\n", position);
        LOGW("子串 %s", s.substr(position, s.size()).c_str());
    } else {
        LOGD("Not found the flag\n");
    }
}

void test_split() {
    char ip_str[] = "192.168.1.250";
    char *p = ip_str;
    char *ip_arr[4];
    char *s = strsep(&p, ".");
    int i = 0;
    while (s) {
        ip_arr[i] = s;
        s = strsep(&p, ".");
        i++;


        LOGI("%s\n", s);
    }

    for (i = 0; i < 4; i++)
        LOGD("%s\n", ip_arr[i]);
}

/**
 * 去掉 string 前后的空格
 * @return
 */
std::string &trim(std::string &);

void test_trim() {
    string hw = "  adaf ";
    LOGI("[%s]", hw.c_str());
    trim(hw);
    LOGI("trim [%s]", hw.c_str());
}

std::string &trim(std::string &s) {
    if (s.empty()) {
        return s;
    }

    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
}

// string char* char[] 相互转
void test_cast() {
    // string -> char *
    string s1 = "abcdeg";
    const char *k = s1.c_str();
    const char *t = s1.data();
    LOGD("%s%s", k, t);

    // string -> char*
    string s2 = "abcdefg";
    unsigned int len = s2.length();
    char *data = (char *) malloc((len + 1) * sizeof(char));
    s2.copy(data, len, 0);
    data[len] = '\0';
    LOGD("data %s", data);

    // string -> char[]
    string pp = "dagah";
    char p2[pp.length() + 1];
    int i;
    for (i = 0; i < pp.length(); i++)
        p2[i] = pp[i];

    p2[i] = '\0';
    LOGD("p2 %s ", p2);


    // char * -> string
    char *p = "adghrtyh";
    string s = p;
    LOGD("s %s ", s.c_str());

}

static bool startWith(string str, string match);

void test_strncmp() {
    string p = "HTTP/1.1 200 OK";
    string match = "HTTP/";

    if (startWith(p, match)) {
        LOGD("匹配");
    } else {
        LOGD("不匹配");
    }
}

static bool startWith(string str, string match) {
    return strncmp(str.c_str(), match.c_str(), match.length()) == 0;
}
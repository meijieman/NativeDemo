//
// Created by Administrator on 2019/8/8.
//

#include <android/log.h>
#include <string.h>

// 定义输出的TAG
#define LOG_TAG "log_test"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

const int NAME_LEN = 10;

class Stud {
private:
    int age;
    char name[NAME_LEN];
public:
    int getAge() {
        return age;
    }

    void setAge(int _age) {
        age = _age;
    }

    void printStd() {
        LOGW("=========age %d, name %s", age, name);
    }

    void setName(char _name[]) {
        int len = strlen(_name);
        LOGW("len %d", len);
        // 安全版 strcpy
        strlcpy(name, _name, NAME_LEN);
        // strcpy 当需要复制的长度大于可容纳的，会导致奔溃
//        strcpy(name, _name);
    }
};

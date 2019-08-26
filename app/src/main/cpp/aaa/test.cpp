//
// Created by Administrator on 2019/8/8.
//

#include <string>
#include "../log.h"

const int NAME_LEN = 10;

class Stud {
private:
    int age;
    char name[NAME_LEN];
    std::string content;
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

    void set_content(std::string _content){
        content = _content;
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

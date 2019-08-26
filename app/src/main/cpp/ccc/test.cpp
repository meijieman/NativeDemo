//
// Created by Administrator on 2019/8/25.
//

#include <jni.h>
#include <string>

using namespace std;

inline double area(int w, int h) {
    return w * h;
}


int weight(int type) {

}

// 函数指针
void calc(int type, int (*we)(int));

struct man {
    string name;
    int age;

};


man *get_man() {
    man m = {"major", 22};
    return &m;
}

void test() {

    calc(100, weight);
    man *p_man = get_man();
    string str = (*p_man).name;
    int &age = (*p_man).age;


}

void swap(int &a, int &b){
    int tmp = a;
    a = b;
    b = tmp;
}

void calc(int type, int (*we)(int)) {

    (*we)(type);

}












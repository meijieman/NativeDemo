//
// Created by Administrator on 2019/8/23.
//

#include <jni.h>
#include <string>
#include <string.h>

#include <filesystem>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>

#include "HttpConnect.h"

#include <android/log.h>
// 定义输出的TAG
#define LOG_TAG "log_http"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

HttpConnect::HttpConnect() {
#ifdef WIN32
    //此处一定要初始化一下，否则gethostbyname返回一直为空
    WSADATA wsa = { 0 };
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

HttpConnect::~HttpConnect() {

}

void parse_resp(std::string resp);
void parse_resp2(char src[]);

void HttpConnect::socketHttp(std::string host, std::string request) {
    LOGI("主机 %s, 请求 %s", host.c_str(), request.c_str());

    int sockfd;
    struct sockaddr_in address;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    server = gethostbyname(host.c_str());
    memcpy((char *) &address.sin_addr.s_addr, (char *) server->h_addr, server->h_length);

    if (-1 == connect(sockfd, (struct sockaddr *) &address, sizeof(address))) {
        std::cout << "connection error!" << std::endl;
        LOGE("connection error!");
        return;
    }

    std::cout << request << std::endl;

#ifdef WIN32
    send(sockfd, request.c_str(),request.size(),0);
#else
    write(sockfd, request.c_str(), request.size());
#endif

    char buf[1024 * 1024] = {0};
    int offset = 0;
    int rc;

#ifdef WIN32
    while(rc = recv(sockfd, buf+offset, 1024,0))
#else
    while ((rc = read(sockfd, buf + offset, 1024)))
#endif

    {
        offset += rc;
    }

#ifdef WIN32
    closesocket(sockfd);
#else
    close(sockfd);
#endif

    buf[offset] = 0;
    std::cout << buf << std::endl;
    LOGI("响应结果 %s", buf);

    std::string src_str = buf;
    auto pos = src_str.find("{");
    if (pos != std::string::npos) {
        std::string json = src_str.substr(pos, src_str.size());
        LOGW("解析后\n %s", json.c_str());
    } else {
        LOGE("not found json");
    }

//    parser_resp(buf);
    parse_resp2(buf);
}

void parse_resp(const std::string resp) {
    unsigned int pos = resp.find("\r\n");
    LOGI("获取到的pos %d", pos);
}

void parse_resp2(char src[]) {
    {
        std::string s = strtok(src, "\r\n");
        LOGI("parse_resp2 %s", s.c_str());
    }
    {
        std::string s = strtok(src, "\r\n");
        LOGI("parse_resp2 %s", s.c_str());
    }

}


void HttpConnect::postData(std::string host, std::string path, std::string post_content) {
    //POST请求方式
    std::stringstream stream;

    stream << "POST " << path;
    stream << " HTTP/1.0\r\n";
    stream << "Host: " << host << "\r\n";
    stream
            << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    stream << "Content-Type:application/x-www-form-urlencoded\r\n";
    stream << "Content-Length:" << post_content.length() << "\r\n";
    stream << "Connection:close\r\n\r\n";
    stream << post_content.c_str();

    socketHttp(host, stream.str());
}

void HttpConnect::getData(std::string host, std::string path, std::string get_content) {
    //GET请求方式
    std::stringstream stream;
    stream << "GET " << path << "?" << get_content;
    stream << " HTTP/1.0\r\n";
    stream << "Host: " << host << "\r\n";
    stream
            << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    stream << "Connection:close\r\n\r\n";

    socketHttp(host, stream.str());
}
//
// Created by Administrator on 2019/8/23.
//

#ifndef NATIVEDEMO_HTTPCONNECT_H
#define NATIVEDEMO_HTTPCONNECT_H

class HttpConnect {
public:
    HttpConnect();

    ~HttpConnect();

    void socketHttp(std::string host, std::string request);

    void postData(std::string host, std::string path, std::string post_content);

    void getData(std::string host, std::string path, std::string get_content);
};


#endif //NATIVEDEMO_HTTPCONNECT_H

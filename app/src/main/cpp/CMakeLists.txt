
cmake_minimum_required(VERSION 3.4.1)

#设置生成的so动态库最后输出的路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

message("==##CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})


add_library(native-lib SHARED
        log.h
        aaa/native-lib.cpp
        aaa/test.cpp
        aaa/HttpConnect.cpp aaa/HttpConnect.h
        bbb/stackblur.c bbb/stackblur.h
        ccc/test.cpp)

find_library(log-lib log)
target_link_libraries(native-lib jnigraphics ${log-lib})

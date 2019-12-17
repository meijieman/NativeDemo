
cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DOS_ANDROID)

#设置生成的so动态库最后输出的路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../jniLibs/${ANDROID_ABI})

# CMAKE_SOURCE_DIR 就是 CMakeLists.txt 所在的路径，在 gradle 中配置
message("==##PROJECT_DIR " ${PROJECT_DIR})
message("==##PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
message("==##CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})
message("==##CMAKE_LIBRARY_OUTPUT_DIRECTORY " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message("==##CMAKE_ANDROID_ARCH_ABI " ${CMAKE_ANDROID_ARCH_ABI})

# 设置头文件目录，C 文件中 includ e时不需要带路径了
include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置一个变量ABI_DIR，指向需要使用的第三方库的路径
set(ABI_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI})
# 设置第三方库的路径，可以引入多个库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${ABI_DIR}")


add_library(ffmpeg SHARED
        log.h
        FFMPEG.cpp)

find_library(log-lib log)

# ANativeWindow 需要
find_library(android-lib android)

find_library(z-lib z)

target_link_libraries(
        ffmpeg

        # 使用静态库时需要注意引入顺序问题，比如n.a引用m.a，那么m必须放在n之前，动态库顺序无所谓
        # 这里使用静态库，就要注意avformat要在avcodec前面，动态库顺序无所谓
        # avfilter avformat avcodec avutil swresample swscale
        # 也可以使用下面 -Wl 的方式
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group

#        -llog -lm -lz
#        -llzma

        ${log-lib}
        ${android-lib}
        ${z-lib}
)
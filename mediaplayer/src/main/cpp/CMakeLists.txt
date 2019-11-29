
cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DOS_ANDROID)

#设置生成的so动态库最后输出的路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../jniLibs/${ANDROID_ABI})


# 设置头文件目录，C 文件中 includ e时不需要带路径了
include_directories(${CMAKE_SOURCE_DIR}/include)

message("==##CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})


# 设置一个变量ABI_DIR，指向需要使用的第三方库的路径
set(ABI_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI})
# 设置第三方库的路径，可以引入多个库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${ABI_DIR}")

find_library(log-lib log)


file(GLOB SOURCE *.cpp)

add_library(player
        SHARED
        log.h
        JavaCallHelper.h
        AudioChannel.h
        VideoChannel.h
        ${SOURCE})

target_link_libraries(
        player

        # 使用静态库时需要注意引入顺序问题，比如n.a引用m.a，那么m必须放在n之前，动态库顺序无所谓
        # 这里使用静态库，就要注意avformat要在avcodec前面，动态库顺序无所谓
        # avfilter avformat avcodec avutil swresample swscale
        # 也可以使用下面 -Wl 的方式
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group

        ${log-lib}
        z # libz.so
        OpenSLES # libOpenSLES.so 播放音频
        android # libandroid.so 使用ANativeWindow需要依赖
)
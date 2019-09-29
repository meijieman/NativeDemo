#!/bin/bash

# 参考 https://blog.csdn.net/leixiaohua1020/article/details/47008825，生成的 .a 4.4 和 8.0 都运行成功

make clean

NDK=/home/major/ffmpeg/android-ndk-r17c #填写自己ndk的解压路径
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt
PLATFORM=$NDK/platforms/android-21/arch-arm
PREFIX=$(pwd)/android/arm

build_one(){
  ./configure --target-os=android --prefix=$PREFIX \
		--enable-cross-compile \
		--enable-runtime-cpudetect \
		--disable-asm \
		--arch=arm \
		--cc=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-gcc \
		--cross-prefix=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi- \
		--disable-stripping \
		--nm=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-nm \
		--sysroot=$PLATFORM \
		--enable-gpl --enable-shared --disable-static --enable-small \
		--disable-ffprobe --disable-ffplay --disable-ffmpeg --disable-ffserver --disable-debug \
		--extra-cflags="-fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm -march=armv7-a" 
}
 
build_one
 
make
make install

